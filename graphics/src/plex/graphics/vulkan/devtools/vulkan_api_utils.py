from typing import List


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


class VulkanFunction:
    def __init__(self, name: str, return_type: str, parameters: List[FunctionParameter]):
        self.name = name
        self.return_type = return_type
        self.parameters = parameters

    def get_signature(self, offset: int = 0, max_width: int = 0):
        signature = f'{self.return_type} {self.name}('
        signature += self._get_parameters_str(True)
        signature += ');'
        signature = VulkanFunction._format_function(signature, offset, max_width)
        return signature

    def get_call(self, offset: int = 0, max_width: int = 0):
        call = f'{self.name}('
        call += self._get_parameters_str(False)
        call += ');'
        call = VulkanFunction._format_function(call, offset, max_width)
        return call

    @staticmethod
    def _format_function(function_str: str, offset: int = 0, max_width: int = 0):
        if 0 < max_width < len(function_str) + offset:
            length_to_first_parenthesis = function_str.find('(') + offset
            padding = ' ' * length_to_first_parenthesis
            function_str = function_str.replace(',', f',\n{padding}')
        return function_str

    def _get_parameters_str(self, with_type: bool):
        return ', '.join([parameter.to_str(with_type) for parameter in self.parameters])

    def remove_parameter_by_type(self, parameter_type: str):
        for parameter in self.parameters:
            if parameter.param_type == parameter_type:
                self.parameters.remove(parameter)
                return True
        return False

    def get_parameter_by_type(self, parameter_type: str) -> FunctionParameter | None:
        for parameter in self.parameters:
            if parameter.param_type == parameter_type:
                return parameter
        return None


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
        return VulkanFunction(function_name, return_type, function_parameters)

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
