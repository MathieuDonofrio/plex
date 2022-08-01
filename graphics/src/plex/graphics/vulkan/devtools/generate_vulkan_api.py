import os
import sys
import textwrap

from vulkan_api_utils import *
from custom_api_wrappers import *

# TODO add command recorder object, binds all Cmd functions to a command buffer
# TODO add VulkanResult object, groups call result and data together

if __name__ == '__main__':
    # if len(sys.argv) < 2:
    #     print('Usage: generate_vulkan_api.py <vulkan_core.h path>')
    #     print('example: generate_vulkan_api.py C:\\VulkanSDK\\1.3.211.0\\Include\\vulkan\\vulkan_core.h')
    #     sys.exit(1)

    if not os.path.abspath('.').endswith('devtools'):
        print('This script needs to be called from the devtools folder.')
        sys.exit(1)

    # if not os.path.exists(sys.argv[1]):
    #     print('The file ' + sys.argv[1] + ' does not exist.')
    #     sys.exit(1)

    vulkan_core_header_path = R"C:\VulkanSDK\1.3.211.0\Include\vulkan\vulkan_core.h"
    max_width = 120
    file_name = 'vulkan_api'
    device_var_name = 'device_'
    header_guard_prefix = 'PLEX_GRAPHICS_'
    namespace = 'plex::vkapi'
    init_function_name = 'InitVulkanApi'
    output_path = '../vulkan2/'

    api_file = VulkanApiFile(vulkan_core_header_path)
    functions = api_file.get_functions()

    functions_to_replace = []
    for i, function in enumerate(functions):
        # TODO move check to respective class -> is_candidate(function)
        # if parameter := function.parameter_list.get_parameter_by_type('uint32_t*'):
        #     if parameter.name.startswith('p') and parameter.name.endswith('Count'):
        #         count_parameter_index = function.parameter_list.get_parameter_index_by_type('uint32_t*')
        #         if count_parameter_index is not None and count_parameter_index + 1 < len(function.parameter_list) - 1:
        #             data_parameter = function.parameter_list.parameters[count_parameter_index + 1]
        #             if data_parameter.name.startswith('p'):
        #                 pass

        if function.parameter_list.get_parameter_by_type('VkDevice') is not None:
            functions_to_replace.append((i, DeviceBoundFunction(function, device_var_name)))

    for i, function in functions_to_replace:
        functions[i] = function

    header_guard = header_guard_prefix + file_name.upper() + '_H'

    newline = '\n'

    header_file = textwrap.dedent(f"""\
    #ifndef {header_guard}
    #define {header_guard}
    
    #include <vulkan/vulkan_core.h>
    
    namespace {namespace}
    {{
    
    // clang-format off
    
    void {init_function_name}(VkDevice device);
    
    {newline.join([indent(f.get_signature(max_width), '    ') + newline for f in functions])[4:]}
    // clang-format on
    
    }} // namespace {namespace}
    
    #endif // {header_guard}
    """)

    with open(output_path + file_name + '.h', 'w') as f:
        f.write(header_file)

    implementation_file = textwrap.dedent(f"""\
    #include "{file_name}.h"
    namespace {namespace}
    {{
    
    // clang-format off
    
    namespace
    {{
    static VkDevice {device_var_name} = VK_NULL_HANDLE; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    }} // namespace
    
    void {init_function_name}(VkDevice device)
    {{
        {device_var_name} = device;
    }}
    
    {(''.join([indent(f.get_implementation(max_width), '    ') + newline for f in functions]))[4:]}
    // clang-format on
    
    }} // namespace {namespace}
    """)

    with open(output_path + file_name + '.cpp', 'w') as f:
        f.write(implementation_file)
