import re
from textwrap import dedent, indent
import mmap

if __name__ == '__main__':
    vulkan_core_header_path = R"C:\VulkanSDK\1.3.211.0\Include\vulkan\vulkan_core.h"
    max_width = 120
    file_name = 'vulkan_api'
    device_var_name = 'device_'
    header_guard_prefix = 'PLEX_GRAPHICS_'
    namespace = 'plex::graphics::vkapi'
    output_path = '../vulkan2/'

    header_guard = header_guard_prefix + file_name.upper() + '_H'


    def clean_function(function: str):
        function = re.sub(r'\s+', ' ', function)
        function = re.sub(r'[\n\r]', '', function)
        return re.sub(r'\( ', '(', function)


    def to_dict(function: str):
        split_function = function.split('(')

        return_type_and_function_name = split_function[0].split(' ')
        return_type = return_type_and_function_name[0]
        function_name = return_type_and_function_name[1]

        parameter_list = split_function[1].split(',')
        params = []
        for parameter in parameter_list:
            parameter_name = re.sub('[);]', '', parameter.split(' ')[-1])
            parameter_type = ' '.join(parameter.split(' ')[:-1])
            params.append({'name': parameter_name.strip(), 'type': parameter_type.strip()})

        call_arguments = [parameter['name'] for parameter in params]
        call_arguments = [re.sub(r'\[\d*]', '', parameter) for parameter in call_arguments]

        return {
            'return_type': return_type,
            'function_name': function_name,
            'parameters': params,
            'call_arguments': call_arguments,
            'tags': []
        }


    with open(vulkan_core_header_path, 'r') as f:
        data = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ).read().decode('utf-8')

    # Extract functions

    function_regex = re.compile(r'VKAPI_ATTR (\w*) *VKAPI_CALL ([\n\w\s,(*\[\]]*\);)', re.MULTILINE)
    functions = [clean_function(' '.join(group)) for group in function_regex.findall(data)]
    functions = [to_dict(function) for function in functions]

    # Start header and implementation file

    header_file = dedent(f"""\
    #ifndef {header_guard}
    #define {header_guard}
    
    #include <vulkan/vulkan_core.h>
    
    #include <vector>
    
    namespace {namespace}
    {{
    
    // clang-format off
    
    template<typename T>
    struct VulkanResultWithValue
    {{
      VkResult result {{ VK_SUCCESS }};
      T value {{}};
      
      VulkanResultWithValue(T&& value = {{}}) : value(std::move(value)) {{}}
      
      operator bool() const
      {{
        return result == VK_SUCCESS;
      }}
    }};

    struct VulkanResult
    {{
      VkResult result;
      
      VulkanResult(VkResult result) : result(result) {{}}
      
      operator bool() const
      {{
        return result == VK_SUCCESS;
      }}
    }};
    
    struct VulkanFunctionPointer
    {{
      void (*callback)() {{}};

      VulkanFunctionPointer(void (*callback)()) : callback(callback) {{}}

      operator bool() const
      {{
        return callback != nullptr;
      }}
    }};
    
    void {init_function_name}(VkDevice device);
    
    """)

    implementation_file = dedent(f"""\
    #include "{file_name}.h"
    
    #include "vulkan_function_table.h"
    
    namespace {namespace}
    {{
    
    using namespace loader;
    
    // clang-format off
    
    namespace
    {{
    static VkDevice {device_var_name} = VK_NULL_HANDLE; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    }} // namespace
    
    void {init_function_name}(VkDevice device)
    {{
      {device_var_name} = device;
    }}
    
    """)

    # Create functions

    command_recorder_class = dedent(f"""\
    class VulkanCommandRecorder {{
    private:
      VkCommandBuffer _command_buffer {{ VK_NULL_HANDLE }};
        
    public:
      VulkanCommandRecorder(VkCommandBuffer command_buffer);
        
    """)

    command_recorder_implementation = "VulkanCommandRecorder::VulkanCommandRecorder(VkCommandBuffer command_buffer) " \
                                      ": _command_buffer(command_buffer) {}\n\n"

    # Command buffer related function that don't follow the vkCmd* naming convention
    special_functions_vk_cmd_functions = ['vkBeginCommandBuffer', 'vkEndCommandBuffer', 'vkResetCommandBuffer']

    for function in functions:
        parameters = function['parameters']

        parameter_types = [parameter['type'] for parameter in parameters]

        # Bind device
        if 'VkDevice' in parameter_types:
            del parameters[0]
            function['call_arguments'][0] = device_var_name
            function['tags'].append('device_bound')

        # Make count query function return vector
        if uint32_t_ptr_index := parameter_types.index('uint32_t*') if 'uint32_t*' in parameter_types else None:
            if uint32_t_ptr_index == len(parameters) - 2 and parameters[uint32_t_ptr_index]['name'].endswith('Count'):
                last_parameter = parameters[-1]
                if last_parameter['type'].endswith('*'):
                    vector_type = last_parameter['type'][:-1]
                    function['original_return_type'] = function['return_type']
                    function['return_type'] = f'std::vector<{vector_type}>'
                    parameters = parameters[:-2]
                    function['call_arguments'] = function['call_arguments'][:-2]
                    function['tags'].append('count_query')

        # Find vkCmd* or special command buffer function
        if 'vkCmd' in function['function_name'] or function['function_name'] in special_functions_vk_cmd_functions:
            if parameter_types[0] == 'VkCommandBuffer':
                del parameters[0]
                function['call_arguments'][0] = '_command_buffer'
                function['tags'].append('command_buffer')
                function['original_function_name'] = function['function_name']
                if function['function_name'] in special_functions_vk_cmd_functions:
                    function['function_name'] = function['function_name'][2:].replace('CommandBuffer', '')
                else:
                    function['function_name'] = function['function_name'].replace('vkCmd', '')

        # Convert return type
        if function['return_type'] != 'void':
            if function['return_type'] == "VkResult":
                function['return_type'] = 'VulkanResult'
            elif 'PFN' in function['return_type']:
                function['return_type'] = 'VulkanFunctionPointer'
            else:
                function['return_type'] = f'VulkanResultWithValue<{function["return_type"]}>'

        # Create function signature
        parameters_str = ', '.join([f'{parameter["type"]} {parameter["name"]}' for parameter in parameters])
        signature = f'{function["return_type"]} {function["function_name"]}({parameters_str});'
        if 'command_buffer' in function['tags']:
            command_recorder_class += f'  {signature[:-1]} const noexcept;\n\n'
        else:
            header_file += f'{signature}\n\n'

        # Create function implementation
        if 'command_buffer' not in function['tags']:
            implementation_start = f'{signature[:-1]}\n{{\n'
            implementation = ""
            if len(function['tags']) == 0 or function['tags'] == ['device_bound']:
                call_arguments_str = ', '.join(function['call_arguments'])
                implementation += f'return GetFunctionTable().{function["function_name"]}({call_arguments_str});'
            elif 'count_query' in function['tags']:
                call_arguments_str = ', '.join(function['call_arguments'])
                implementation += f'const auto& fp = GetFunctionTable().{function["function_name"]};\n'
                implementation += f'uint32_t count = 0;\n'
                implementation += f'fp({call_arguments_str}, &count, nullptr);\n'
                implementation += f'{function["return_type"]} result;\n'
                implementation += f'result.value.resize(count);\n'
                if function['original_return_type'] != 'void':
                    implementation += 'result.result = '
                implementation += f'fp({call_arguments_str}, &count, result.value.data());\n'
                implementation += f'return result;'

            implementation = indent(implementation, '  ')

            implementation_end = '\n}\n\n'
            implementation_file += implementation_start + implementation + implementation_end
        else:
            command_recorder_implementation += f'{function["return_type"]} ' \
                                               f'VulkanCommandRecorder::' \
                                               f'{function["function_name"].replace("vkCmd","")}' \
                                               f'({parameters_str}) const noexcept\n{{\n'
            call_arguments_str = ', '.join(function['call_arguments'])
            if function['return_type'] == 'void':
                command_recorder_implementation += f'  GetFunctionTable().{function["original_function_name"]}' \
                                                   f'({call_arguments_str});\n'
            else:
                command_recorder_implementation += f' return GetFunctionTable().{function["original_function_name"]}' \
                                                   f'({call_arguments_str});\n'
            command_recorder_implementation += '}\n\n'

    # Append command recorder class to header file
    command_recorder_class += '};\n\n'
    header_file += command_recorder_class
    implementation_file += command_recorder_implementation

    # End header and implementation file
    header_file += '// clang-format on\n\n'
    header_file += '} // namespace ' + namespace + '\n\n'
    header_file += '#endif // ' + header_guard + '\n'

    # End implementation file
    implementation_file += '// clang-format on\n\n'
    implementation_file += '} // namespace ' + namespace + '\n'

    # Write files
    with open(output_path + file_name + '.h', 'w') as f:
        f.write(header_file)

    with open(output_path + file_name + '.cpp', 'w') as f:
        f.write(implementation_file)
