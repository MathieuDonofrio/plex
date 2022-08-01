import copy
from textwrap import indent, dedent

from vulkan_api_utils import VulkanFunction


class DeviceBoundFunction(VulkanFunction):
    def __init__(self, function: VulkanFunction, device_parameter_name: str):
        super().__init__(function.name, function.return_type, function.parameter_list)
        self.device_parameter_name = device_parameter_name

    def get_signature(self, max_width: int = 0, indentation: str = ""):
        custom_parameter_list = copy.deepcopy(self.parameter_list)
        custom_parameter_list.remove_parameter_by_type('VkDevice')
        signature = f'{self.return_type} {self.name}({custom_parameter_list.to_str(True)});'
        signature = VulkanFunction._format_function(signature, max_width)
        return VulkanFunction._indent_lines_after_first(signature, indentation)

    def get_call(self, prefix: str = "", max_width: int = 0, indentation: str = ""):
        custom_parameter_list = copy.deepcopy(self.parameter_list)
        custom_parameter_list.get_parameter_by_type('VkDevice').name = self.device_parameter_name
        prefix_length = len(prefix)
        call = f"{self.name}({custom_parameter_list.to_str(False)})"
        call = VulkanFunction._format_function(call, max_width, prefix_length + len(indentation))
        call = prefix + indent(call, ' ' * prefix_length)[prefix_length:]
        return VulkanFunction._indent_lines_after_first(call, indentation)

    def get_implementation(self, max_width: int = 0):
        call = indent(self.get_call('return ::', max_width, ' ' * 8), ' ' * 4)
        signature = self.get_signature(max_width, ' ' * 8)[:-1]
        implementation = f"""\
        {signature}
        {{
        {call};
        }}
        """
        return dedent(implementation)


class CountQueryFunction(VulkanFunction):
    def __init__(self, function: VulkanFunction, device_parameter_name: str):
        super().__init__(function.name, function.return_type, function.parameter_list)
        self.data_type = self.parameter_list.get_parameter_by_index(-1).param_type.replace('*', '')

    def get_signature(self, offset: int = 0, max_width: int = 0):
        custom_parameter_list = copy.deepcopy(self.parameter_list)
        custom_parameter_list.remove_parameter_by_type('uint32_t*')
        self.return_type = f'std::vector<{self.data_type}>'
        custom_parameter_list.remove_parameter_by_index(-1)

        signature = f'{self.return_type} {self.name}('
        signature += custom_parameter_list.to_str(True)
        signature += ');'
        signature = VulkanFunction._format_function(signature, offset, max_width)
        return indent(signature, ' ' * offset)

    def get_implementation(self, max_width: int = 0):
        call_parameter_list = copy.deepcopy(self.parameter_list)
        call_parameter_list.remove_parameter_by_type('uint32_t*')
        call_parameter_list.remove_parameter_by_index(-1)

        implementation = ""#self.get_signature(offset, max_width)[:-1]
        implementation += '\n{\n'
        implementation += f'    std::vector<{self.data_type}> data;\n'
        implementation += f'    uint32_t count = 0;\n'
        implementation += f'    {self.name}({call_parameter_list.to_str(False)}, &count, nullptr);\n'
        implementation += f'    data.resize(count);\n'
        implementation += f'    auto result = {self.name}({call_parameter_list.to_str(False)}, &count, data.data());\n'

    def get_call(self, offset: int = 0, additional_offset: int = 0, max_width: int = 0):
        custom_parameter_list = copy.deepcopy(self.parameter_list)
        custom_parameter_list.remove_parameter_by_type('uint32_t*')
        data_parameter = custom_parameter_list.parameters[-1]
        data_type = data_parameter.param_type.replace('*', '')

        call = f'{self.name}('
        call += custom_parameter_list.to_str(False)
        call += ');'
        call = VulkanFunction._format_function(call, offset, additional_offset, max_width)
        return indent(call, ' ' * offset)
