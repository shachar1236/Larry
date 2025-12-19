
class CppVariable():

    def __init__(self, var_type, var_name, ):
        self.var_type = var_type
        self.var_name = var_name

    def to_cpp(self):
        return f"{self.var_type} {self.var_name}"

    def __repr__(self) -> str:
        return f"Var({self.var_type} {self.var_name})"

class CppClassVariable():

    def __init__(self, var_type, var_name, state, default_value=None):
        self.var_type = var_type
        self.var_name = var_name
        self.state = state

    def __repr__(self) -> str:
        return f"Var({self.state}:{self.var_type} {self.var_name})"

class CppClassConstructor():

    def __init__(self, arguments : list[CppVariable]) -> None:
        self.arguments : list[CppVariable] = arguments
    
    def __repr__(self) -> str:
        return f"Constructor({self.arguments})"

class CppClassDestractor():

    def __init__(self) -> None:
        pass

    def __repr__(self) -> str:
        return f"Destructor()"

class CppClassFunction():

    def __init__(self, name, state, return_value, args : list[CppVariable]):
        self.name = name
        self.state = state
        self.return_value = return_value
        self.args = args

    def __repr__(self) -> str:
        return f"ClassFunction({self.state}:{self.return_value} {self.name}({self.args}))"

class CppClass():

    def __init__(self, name):
        self.name = name
        self.constructors = []
        self.destructor = None
        self.variables : list[CppClassVariable] = []
        self.functions : list[CppClassFunction] = [] 

    def __repr__(self) -> str:
        return f"Class({self.name}, {self.constructors}, {self.destructor}, {self.variables}, {self.functions})"

class CppFunction():

    def __init__(self, name, return_value, args : list[CppVariable]):
        self.name = name
        self.return_value = return_value
        self.args = args

    def __repr__(self) -> str:
        return f"Function({self.return_value} {self.name}({self.args}))"
