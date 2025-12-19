from cpp_types import *

TAB = " " * 4

def create_lua_code(parsed_objects, out_file):
    code = """local ffi = require("ffi")
ffi.cdef[[
"""
    tab_count = 1
    for obj in parsed_objects:
        if isinstance(obj, CppFunction):
            code += f"{TAB * tab_count}{obj.return_value} {obj.name}({', '.join([x.to_cpp() for x in obj.args])})"
        if isinstance(obj, CppClass):
            # for constructor in obj.constructors:
                # code += generate_constructor(obj.name, constructor, tab_count)
            for function in obj.functions:
                code += generate_function(obj.name, function, tab_count)
            # if obj.destructor:
                # code += generate_destractor(obj.name, tab_count)
    tab_count -= 1


