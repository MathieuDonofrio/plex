from textwrap import dedent, indent
import mmap
import re

if __name__ == '__main__':
    vulkan_include_path = R"C:\VulkanSDK\1.3.211.0\Include\vulkan\\"
    file_name = 'vulkan_function_table'
    device_var_name = 'device_'
    header_guard_prefix = 'PLEX_GRAPHICS_'
    namespace = 'plex::graphics::vkapi::loader'
    init_function_name = 'InitVulkanApi'
    output_path = '../vulkan2/'
    headers = [
        "vulkan_core.h",
        "vulkan_win32.h",
    ]

    header_guard = header_guard_prefix + file_name.upper() + '_H'

    extension_block_start_regex = re.compile(r'#define .*_EXTENSION_NAME\s*\"(.*)\"')
    core_block_start_regex = re.compile(r'#define VK_VERSION_(1_.*) 1')
    block_end_str = '#endif'
    function_block_start_str = '#ifndef VK_NO_PROTOTYPES'
    function_regex = re.compile(r'VKAPI_ATTR (\w*) *VKAPI_CALL ([\n\w\s,(*\[\]]*\);)', re.MULTILINE)


    def clean_function(function: str):
        function = re.sub(r'\s+', ' ', function)
        function = re.sub(r'[\n\r]', '', function)
        return re.sub(r'\( ', '(', function)


    def to_dict(function: str):
        split_function = function.split('(')

        function_name = split_function[0].split(' ')[1]

        parameter_list = split_function[1].split(',')
        params = []
        for parameter in parameter_list:
            parameter_name = re.sub('[);]', '', parameter.split(' ')[-1])
            parameter_type = ' '.join(parameter.split(' ')[:-1])
            params.append({'name': parameter_name.strip(), 'type': parameter_type.strip()})

        return {
            'function_name': function_name,
            'parameters': params,
        }


    def extract_function(block_start_regex, file_data):
        extracted_functions = {}
        i = 0
        while i < len(file_data):
            extension_name = block_start_regex.findall(file_data[i])
            if len(extension_name) != 0:
                extension_section = ""

                while file_data[i].strip() != function_block_start_str:
                    i += 1
                    if i >= len(file_data):
                        break
                if i >= len(file_data):
                    break

                while file_data[i].strip() != block_end_str:
                    extension_section += file_data[i] + '\n'
                    i += 1
                    if i >= len(file_data):
                        break

                functions = [clean_function(' '.join(group)) for group in function_regex.findall(extension_section)]

                if len(functions) == 0:
                    continue

                functions = [to_dict(function) for function in functions]
                extracted_functions[extension_name[0]] = functions
            i += 1
        return extracted_functions


    extension_functions_by_extension = {}
    core_functions_by_version = {}
    for header in headers:
        with open(vulkan_include_path + header, 'r') as f:
            data = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ).read().decode('utf-8').split('\n')

            extension_functions_by_extension.update(extract_function(extension_block_start_regex, data))

            if header == 'vulkan_core.h':
                core_functions_by_version.update(extract_function(core_block_start_regex, data))

    # merge all core functions
    core_functions = [f for f in core_functions_by_version.values()]
    core_functions = [item for sublist in core_functions for item in sublist]  # flatten list of lists

    global_function_names = [
        "vkEnumerateInstanceVersion",
        "vkEnumerateInstanceExtensionProperties",
        "vkEnumerateInstanceLayerProperties",
        "vkCreateInstance"
    ]
    # find global functions from core functions
    global_functions = [f for f in core_functions if f['function_name'] in global_function_names]
    # remove global functions from core functions
    core_functions = [f for f in core_functions if f['function_name'] not in global_function_names]

    # Find and remove vkGetInstanceProcAddr function
    get_instance_proc_addr_function = None
    for i in range(len(core_functions)):
        if core_functions[i]['function_name'] == 'vkGetInstanceProcAddr':
            get_instance_proc_addr_function = core_functions.pop(i)
            break

    if get_instance_proc_addr_function is None:
        raise Exception('vkGetInstanceProcAddr function not found')

    # Make a merged list of all function names
    get_instance_proc_addr_function_name = get_instance_proc_addr_function['function_name']
    core_function_names = [f['function_name'] for f in core_functions]
    extension_functions = [functions for functions in extension_functions_by_extension.values()]
    extension_function_names = [f['function_name'] for functions in extension_functions for f in functions]
    function_names = [get_instance_proc_addr_function_name]
    function_names += global_function_names
    function_names += core_function_names
    function_names += extension_function_names

    # generate list of function names
    new_line = '\n'
    function_names_list = dedent(f'''
    constexpr const char* const function_names[] = {{
    {''.join([f'{" " * (4 + 2)}"{name}",{new_line}' for name in function_names])[4:-2]}
    }};
    ''')
    # print(function_names_list)

    # generate offset and size list
    i = 0
    offset_list = [
        f'{{"{get_instance_proc_addr_function_name}", 0, 1}}',
        f'{{"global", 1, {len(global_functions)}}}',
        f'{{"core", {len(global_functions) + 1}, {len(core_functions)}}}',
    ]  # name, offset, count
    offset = len(global_functions) + len(core_functions) + 1
    for extension_name, functions in extension_functions_by_extension.items():
        offset_list.append(f'{{"{extension_name}", {offset}, {len(functions)}}}')
        offset += len(functions)

    offset_list_str = dedent(f'''
    using FunctionName = std::string;
    const std::tuple<FunctionName, FunctionGroupStartIndex, FunctionGroupCount> offset_list[] = {{ // NOLINT(cert-err58-cpp)
    {''.join([f'{" " * (4 + 2)}{offset_str},{new_line}' for offset_str in offset_list])[4:-2]}
    }};
    ''')[1:]

    # Generate function pointer table
    function_pointers_list = []
    for function_name in function_names:
        function_pointer_type = f'PFN_{function_name}'
        function_pointers_list.append(f'{function_pointer_type} {function_name} {{ nullptr }}')

    function_pointers_list_str = "  " + f';\n{" " * (4 + 2)}'.join(function_pointers_list)
    function_pointer_table_str = dedent(f'''
    struct FunctionTable
    {{
    {function_pointers_list_str};
    }};
    ''')[1:-1]

    # Generate header file
    header_file_str = dedent(f'''
    #ifndef {header_guard}
    #define {header_guard}
    
    #define VK_NO_PROTOTYPES
    
    {"""// Definitions for WIN32 to avoid including windows.h
    using DWORD = unsigned long;
    using LPCWSTR = const wchar_t*;
    using HANDLE = void*;
    using HINSTANCE = struct HINSTANCE__*; // NOLINT
    using HWND = struct HWND__*; // NOLINT
    using HMONITOR = struct HMONITOR__*; // NOLINT
    using SECURITY_ATTRIBUTES = struct _SECURITY_ATTRIBUTES; // NOLINT""" if "vulkan_win32.h" in headers else ""}
    
    {new_line.join([f'    #include "vulkan/{header}"' for header in headers])[4:]}
    
    #include <string>
    #include <tuple>
    #include <vector>
    
    // clang-format off
    
    namespace {namespace}
    {{
    
    struct FunctionTable;
    
    ///
    /// Get the global function table.
    ///
    FunctionTable& GetFunctionTable() noexcept;
    
    using FunctionGroupStartIndex = uint32_t;
    using FunctionGroupCount = uint32_t;

    ///
    /// Finds the index and the count of the function group for the given group name.
    /// 
    /// @param group_name The name of the function group. 
    /// @return The start index and the count of the function group for the given function group name.
    std::pair<FunctionGroupStartIndex, FunctionGroupCount> GetFunctionGroupLoadInfo(const std::string& group_name) noexcept;
    
    ///
    /// Return the function name for the given index.
    ///
    /// @param index The index of the function name.
    /// @return The function name for the given index or nullptr if the index is out of bounds.
    const char* GetFunctionName(uint32_t index) noexcept;
    
    ///
    /// Set the list of extensions to load.
    ///
    /// @param extensions The list of extensions to load.
    void SetExtensions(const std::vector<const char*>& extensions);
    
    ///
    /// Get the list of extensions to load.
    ///
    /// @return The list of extensions to load.
    const std::vector<const char*>& GetExtensions() noexcept;
    
    {indent(function_pointer_table_str, '    ')[4:]}
    
    }} // namespace {namespace}
    
    // clang-format on
    
    #endif // {header_guard}
    ''')

    # Generate implementation file
    implementation_file_str = dedent(f'''
    #include "{file_name}.h"
                                     
    #include "plex/debug/logging.h"
    
    // clang-format off
                                     
    namespace {namespace}
    {{
    
    namespace
    {{
     static FunctionTable _table; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
     static std::vector<const char*> _extensions; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    }} // namespace
    
    FunctionTable& GetFunctionTable() noexcept
    {{
        return _table;
    }};
    
    void SetExtensions(const std::vector<const char*>& extensions)
    {{
        _extensions = extensions;
    }};
    
    const std::vector<const char*>& GetExtensions() noexcept
    {{
        return _extensions;
    }};
    
    namespace
    {{
    {indent(offset_list_str, '    ')[4:]}
    {indent(function_names_list, '    ')[4:]}
    }} // namespace
    
    std::pair<FunctionGroupStartIndex, FunctionGroupCount> GetFunctionGroupLoadInfo(const std::string& group_name) noexcept
    {{
        for (const auto& [name, offset, size] : offset_list)
        {{
            if (group_name == name)
            {{
                return {{offset, size}};
            }}
        }}
        return {{0, 0}};
    }}
    
    const char* GetFunctionName(uint32_t index) noexcept
    {{
        if (index >= {len(function_names)})
        {{
            LOG_ERROR("Invalid function index: {{}}", std::to_string(index));
            return nullptr;
        }}
        return function_names[index];
    }};
    
    // clang-format on    
    
    }} // namespace {namespace}
    ''')

    # Write files
    with open(output_path + file_name + '.h', 'w') as f:
        f.write(header_file_str)

    with open(output_path + file_name + '.cpp', 'w') as f:
        f.write(implementation_file_str)
