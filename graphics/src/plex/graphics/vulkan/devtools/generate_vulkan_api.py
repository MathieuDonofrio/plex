import copy
import os
import sys

from vulkan_api_utils import *

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: generate_vulkan_api.py <vulkan_core.h path>')
        print('example: generate_vulkan_api.py C:\\VulkanSDK\\1.3.211.0\\Include\\vulkan\\vulkan_core.h')
        sys.exit(1)

    if not os.path.abspath('.').endswith('devtools'):
        print('This script needs to be called from the devtools folder.')
        sys.exit(1)

    if not os.path.exists(sys.argv[1]):
        print('The file ' + sys.argv[1] + ' does not exist.')
        sys.exit(1)

    vulkan_core_header_path = sys.argv[1]
    max_width = 120
    file_name = 'vulkan_api'
    device_var_name = 'device_'
    header_guard_prefix = 'PLEX_GRAPHICS_'
    namespace = 'plex::vkapi'
    init_function_name = 'InitVulkanApi'
    output_path = '../vulkan2/'

    api_file = VulkanApiFile(vulkan_core_header_path)
    functions = api_file.get_functions()


    def get_implementation(function: VulkanFunction):
        function_without_device_parameter = copy.deepcopy(function)
        function_without_device_parameter.remove_parameter_by_type('VkDevice')

        function_with_renamed_device_parameter = copy.deepcopy(function)
        if param := function_with_renamed_device_parameter.get_parameter_by_type('VkDevice'):
            param.name = device_var_name

        implementation = function_without_device_parameter.get_signature(0, max_width)[:-1]
        implementation += '\n{\n'
        call_prefix = '    return ::'
        call = function_with_renamed_device_parameter.get_call(len(call_prefix), max_width)
        implementation += f'{call_prefix}{call}\n'
        implementation += '}\n'

        return implementation


    header_guard = header_guard_prefix + file_name.upper() + '_H'

    header_file = f'#ifndef {header_guard}\n'
    header_file += f'#define {header_guard}\n'
    header_file += '\n'
    header_file += '#include <vulkan/vulkan_core.h>\n'
    header_file += '\n'
    header_file += f'namespace {namespace}\n{{\n'
    header_file += '\n'
    header_file += '// clang-format off\n'
    header_file += '\n'

    header_file += 'void ' + init_function_name + '(VkDevice device);\n\n'

    for f in copy.deepcopy(functions):
        f.remove_parameter_by_type('VkDevice')
        header_file += f.get_signature(0, max_width) + '\n'
        header_file += '\n'
    header_file += '// clang-format on\n'
    header_file += '\n'
    header_file += f'}} // namespace {namespace}\n'
    header_file += '\n'
    header_file += f'#endif // {header_guard}\n'

    with open(output_path + file_name + '.h', 'w') as f:
        f.write(header_file)

    implementation_file = f'#include "{file_name}.h"\n'
    implementation_file += f'namespace {namespace}\n{{\n'
    implementation_file += '\n'
    implementation_file += '// clang-format off\n'
    implementation_file += '\n'
    implementation_file += f'namespace\n{{\n'
    implementation_file += f'static VkDevice {device_var_name} = VK_NULL_HANDLE; '
    implementation_file += '// NOLINT(cppcoreguidelines-avoid-non-const-global-variables)\n'
    implementation_file += '} // namespace\n'
    implementation_file += '\n'
    implementation_file += 'void ' + init_function_name + '(VkDevice device)\n'
    implementation_file += '{\n'
    implementation_file += f'    {device_var_name} = device;\n'
    implementation_file += '}\n'
    implementation_file += '\n'
    for f in copy.deepcopy(functions):
        implementation_file += get_implementation(f) + '\n'

    implementation_file += '// clang-format on\n'
    implementation_file += '\n'
    implementation_file += f'}} // namespace {namespace}\n'

    with open(output_path + file_name + '.cpp', 'w') as f:
        f.write(implementation_file)
