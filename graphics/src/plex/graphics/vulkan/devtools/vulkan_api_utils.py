from typing import List
from textwrap import indent, dedent


class FunctionParameter:
    def __init__(self,
                 name: str,
                 param_type: str,
                 array_size: int = 0):
        self.name = name
        self.param_type = param_type
        self.array_size = array_size

    def to_str(self, with_type: bool) -> str:
        if not with_type:
            return self.name
        formatted_name = self.name if self.array_size == 0 else f'{self.name}[{self.array_size}]'
        return f'{self.param_type} {formatted_name}'


class ParameterList:
    def __init__(self, parameters: List[FunctionParameter] = None):
        self.parameters = parameters if parameters is not None else []

    def remove_parameter_by_type(self, parameter_type: str):
        for parameter in self.parameters:
            if parameter.param_type == parameter_type:
                self.parameters.remove(parameter)
                return True
        return False

    def remove_parameter_by_index(self, parameter_index: int):
        self.parameters.pop(parameter_index)

    def get_parameter_by_type(self, parameter_type: str) -> FunctionParameter | None:
        for parameter in self.parameters:
            if parameter.param_type == parameter_type:
                return parameter
        return None

    def get_parameter_index_by_type(self, parameter_type: str) -> int | None:
        for i, parameter in enumerate(self.parameters):
            if parameter.param_type == parameter_type:
                return i
        return None

    def get_parameter_by_index(self, parameter_index: int) -> FunctionParameter:
        return self.parameters[parameter_index]

    def to_str(self, with_type: bool) -> str:
        if len(self.parameters) == 0:
            return ''
        return ', '.join([parameter.to_str(with_type) for parameter in self.parameters])

    def __len__(self):
        return len(self.parameters)


class VulkanFunction:
    def __init__(self, name: str, return_type: str, parameter_list: ParameterList):
        self.name = name
        self.return_type = return_type
        self.parameter_list = parameter_list

    @staticmethod
    def _format_function(function_str: str, max_width: int = 0, additional_length: int = 0):
        if 0 < max_width < len(function_str) + additional_length:
            length_to_first_parenthesis = function_str.find('(')
            padding = ' ' * length_to_first_parenthesis
            function_str = function_str.replace(',', f',\n{padding}')
        return function_str

    @staticmethod
    def _indent_lines_after_first(string: str, indentation: str):
        lines = string.split('\n')
        first_line = lines.pop(0)
        return '\n'.join([first_line] + [indentation + line for line in lines])

    def get_signature(self, max_width: int = 0, indentation: str = ""):
        signature = f'{self.return_type} {self.name}({self.parameter_list.to_str(True)});'
        signature = VulkanFunction._format_function(signature, max_width)
        return VulkanFunction._indent_lines_after_first(signature, indentation)

    def get_call(self, prefix: str = "", max_width: int = 0, indentation: str = ""):
        prefix_length = len(prefix)
        call = f"{self.name}({self.parameter_list.to_str(False)})"
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


class VulkanApiFile:

    def __init__(self, vulkan_api_path):
        self.vulkan_api_file_lines = []
        with open(vulkan_api_path, "r") as f:
            self.vulkan_api_file_lines = f.readlines()

    def get_functions(self) -> List[VulkanFunction]:
        return [self._parse_signature_line(signature_line) for signature_line in self._get_signature_lines()]

    def _get_signature_lines(self) -> List[str]:
        function_lines = []
        inside_signature = False
        current_signature = ""

        for line in self.vulkan_api_file_lines:
            if not inside_signature:
                if 'VKAPI_CALL' in line and 'VKAPI_ATTR' in line:
                    inside_signature = True
                    current_signature = line[:-1]
            else:
                current_signature += line[:-1]
                if ');' in line:
                    inside_signature = False
                    function_lines.append(current_signature)

        return function_lines

    @staticmethod
    def _parse_signature_line(signature_line) -> VulkanFunction:
        signature_line = signature_line.replace('VKAPI_CALL ', '').replace('VKAPI_ATTR ', '')
        signature_line = ' '.join(signature_line.split())
        return_type_space_index = signature_line.find(' ')
        return_type = signature_line[:return_type_space_index]
        function_name_end_index = signature_line.find('(')
        function_name = signature_line[return_type_space_index + 1:function_name_end_index]
        function_parameters = VulkanApiFile._parse_function_parameters(signature_line[function_name_end_index + 1:-2])
        return VulkanFunction(function_name, return_type, ParameterList(function_parameters))

    @staticmethod
    def _parse_function_parameters(parameter_line) -> List[FunctionParameter]:
        parameters = []
        for parameter_string in parameter_line.split(','):
            parameter_tokens = parameter_string.strip().split()
            token_count = len(parameter_tokens)

            if token_count == 0:
                continue

            if '[' in parameter_tokens[-1]:
                parameter_name, array_size = VulkanApiFile._parse_array_parameter_name(parameter_tokens[-1])
                parameters.append(
                    FunctionParameter(parameter_name, ' '.join(parameter_tokens[0:-1]), array_size)
                )
            else:
                parameters.append(FunctionParameter(parameter_tokens[-1], ' '.join(parameter_tokens[0:-1])))

        return parameters

    @staticmethod
    def _parse_array_parameter_name(parameter_name) -> [str, int]:
        bracket_index = parameter_name.find('[')
        array_size = int(parameter_name[bracket_index + 1:-1])
        parameter_name = parameter_name[:bracket_index]
        return parameter_name, array_size
