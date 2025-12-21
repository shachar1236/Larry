from cpp_types import *
from colorama import Fore

TAB = " " * 4

def generate_func_args(func : CppClassFunction):
    res = ""
    if len(func.args) > 0:
        res = ", "
        res += ', '.join([f"{arg.var_type} {arg.var_name}" for arg in func.args])
    return res


def create_class_cdef(obj : CppClass):
    res = "typedef struct {\n"
    for var in obj.variables:
        res += f"    {var.var_type} {var.var_name};\n"
    res += "} " + obj.name + ";\n\n"
    return res

def generate_method_cdef(obj : CppClass, function : CppClassFunction):
    res = f"{function.return_value} _{obj.name}_{function.name}({obj.name}* __obj"
    res += generate_func_args(function)
    res += ");\n\n"
    return res

def create_ctype_metamethods(obj : CppClass):
    res = f"local _obj_{obj.name}\n"
    res += f"local _obj_{obj.name}_mt = " + "{\n    __index = {\n"
    for function in obj.functions:
        args = ""
        if len(function.args) > 0:
            args = ", "
            args += ', '.join([f"{arg.var_name}" for arg in function.args])
        res += TAB * 2 + f"{function.name} = function(obj{args}) return ffi.C._{obj.name}_{function.name}(obj{args}) end,\n"
    res += TAB + "}\n}\n\n"
    res += f'_obj_{obj.name} = ffi.metatype("{obj.name}*", _obj_{obj.name}_mt)\n\n'
    return res

def create_unvalid_class_methods(obj : CppClass):
    res = ""
    for function in obj.functions:
        args = ""
        if len(function.args) > 0:
            args = ", "
            args += ', '.join([f"{arg.var_name}" for arg in function.args])
        res += f"function {obj.name}{function.name}({obj.name}{args})\n"
        res += TAB + f"return ffi.C._{obj.name}_{function.name}({obj.name}{args})\nend\n\n"
    return res

def create_function_in_lua(obj : CppFunction):
    args_names = ", ".join([f"{arg.var_name}" for arg in obj.args])
    res = f"function {obj.name}({args_names})\n"
    res += TAB + f"return ffi.C.{obj.name}({args_names})\nend\n\n"
    return res

def create_lua_code(parsed_objects, unvalid_classes : list[CppClass], out_file, custom_ffi_cdef):
    ffi = "ffi"
    code = 'local ffi = require("ffi")\n'
    if custom_ffi_cdef:
        code += f'local custom_cdef = require("{custom_ffi_cdef[0]}")\n'
        code += "local cdef = custom_cdef .. [[\n"
    else:
        code += "local cdef = [[\n"

    for cpp_class in unvalid_classes:
        code += f"typedef void {cpp_class.name};\n"

    code += "\n"

    for obj in parsed_objects:
        if isinstance(obj, CppFunction):
            code += f"{obj.return_value} {obj.name}({', '.join([x.to_cpp() for x in obj.args])})"
        if isinstance(obj, CppClass):
            code += create_class_cdef(obj)
            
            # for constructor in obj.constructors:
                # code += generate_constructor(obj.name, constructor, tab_count)
            for function in obj.functions:
                code += generate_method_cdef(obj, function)
            # if obj.destructor:
                # code += generate_destractor(obj.name, tab_count)
    code += "]]\n\n"
    code += "ffi.cdef(cdef)\n\n"

    for cpp_class in unvalid_classes:
        code += create_unvalid_class_methods(cpp_class)
    
    for obj in parsed_objects:
        if isinstance(obj, CppFunction):
            code += create_function_in_lua(obj)
        if isinstance(obj, CppClass):
            if len(obj.functions) > 0:
                code += create_ctype_metamethods(obj)

    with open(out_file, "w") as f:
        print(Fore.GREEN, "Writing code to ", out_file)
        f.write(code)


