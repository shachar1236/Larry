from cpp_types import *

TAB = " " * 4

def generate_constructor(class_name, constructor : CppClassConstructor, tab_count):
    code = f"{TAB * tab_count}{class_name} _create_{class_name}({', '.join(map(lambda a: a.to_cpp(), constructor.arguments))})\n";
    code += TAB * tab_count + "{\n";
    tab_count += 1
    code += f"{TAB * tab_count}return {class_name}({', '.join(map(lambda a: a.var_name, constructor.arguments))});\n";
    tab_count -= 1
    return code + f"{TAB * tab_count}" + "}\n\n"

def generate_destractor(class_name, tab_count):
    code = f"{TAB * tab_count}void _destruct_{class_name}({class_name}* __obj)\n";
    code += TAB * tab_count + "{\n";
    tab_count += 1
    code += f"{TAB * tab_count}__obj->~{class_name}();\n";
    tab_count -= 1
    return code + f"{TAB * tab_count}" + "}\n\n"

def generate_function(class_name, function : CppClassFunction, tab_count):
    code = f"{TAB * tab_count}{function.return_value} _{class_name}_{function.name}({class_name}* __obj";
    if len(function.args) > 0:
        code += ", "

    code += f"{', '.join(map(lambda a: a.to_cpp(), function.args))})\n";
    code += TAB * tab_count + "{\n";
    tab_count += 1
    code += f"{TAB * tab_count}return __obj->{function.name}({','.join(map(lambda a: a.var_name, function.args))});\n";
    tab_count -= 1
    return code + f"{TAB * tab_count}" + "}\n\n"

def create_cpp_code(input_files, include_directories, namespaces, parsed_objects, out_file):
    code = """
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
"""
    for f in input_files:
        for I in include_directories:
            if I in f:
                index = f.find(I)
                f_name = f[index+len(I):]
                code += f"#include <{f_name}>\n"
                break

    code += "\n"
    for n in namespaces:
        code += f"using namespace {n};\n"
    code += """

extern "C" {
"""
    tab_count = 1
    for obj in parsed_objects:
        if isinstance(obj, CppClass):
            for constructor in obj.constructors:
                code += generate_constructor(obj.name, constructor, tab_count)
            for function in obj.functions:
                code += generate_function(obj.name, function, tab_count)
            if obj.destructor:
                code += generate_destractor(obj.name, tab_count)
    tab_count -= 1
    code += "}"
    
    with open(out_file, "w") as f:
        f.write(code)
