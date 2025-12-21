import sys
import re
from cxxheaderparser.simple import parse_string
from cxxheaderparser.parser import *
import argparse
from colorama import Fore
from cpp_types import *
from cpp_code_generator import *
from lua_code_generator import *

parser = argparse.ArgumentParser()

parser.add_argument("files", nargs="+")
parser.add_argument("-o", action="store", nargs=2, help="The output files, the first is the cpp file and the second is the lua file.")
parser.add_argument("--ignore-classes", "-ic", action="store", nargs="+", help="Dont parse classes with the provided names.")
parser.add_argument("--exists", "-e", action="store", nargs="+", help="Act as if the provided classes already exists and dont throw an error when when refrencing them.")
parser.add_argument("-I", action="store", nargs="+", help="Directories where the header file are.")
parser.add_argument("--skip-errors", "-se", action="store_true", help="If there is an error parsing the file it will just skip it.")
parser.add_argument("--custom-ffi-cdef", action="store", nargs=1, help="Lua script name that returns a cdef to add to the lua ffi.")

args = parser.parse_args()

args_exists = None
if args.exists != None:
    args_exists = args.exists[:]
    for arg in args.exists:
        args_exists.append("const " + arg)
        args_exists.append("const " + arg + "*")
        args_exists.append("const " + arg + "**")
        args_exists.append(arg + "*")
        args_exists.append(arg + "**")

def iswhitespace(char):
    return char == " " or char == "\t" or char == "\n" or char.isspace()

BASE_TYPES = ["int", "float", "double", "char", "bool", "short", "long", "void"]
OPERATORS = ["=", ";", ",", ".", "{", "}", "(", ")"]

def get_clean_type(t, namespaces : set[str], parent_namespace=""):
    if "::" in t:
        i = t.find("::")
        namespace = t[:i]
        if parent_namespace != "":
            namespace = parent_namespace + "::" + namespace
        namespaces.add(namespace)
        return get_clean_type(t[i+2:], namespaces, parent_namespace=namespace)
    return t


def enumerate_on_variables_types_in_object(obj : CppClass, func):
    constructors = []
    for constructor in obj.constructors:
        try:
            for i, arg in enumerate(constructor.arguments):
                constructor.arguments[i].var_type = func(arg.var_type)
            constructors.append(constructor)
        except Exception as e:
            if not args.skip_errors:
                raise e
    obj.constructors = constructors

    functions = []
    for function in obj.functions:
        try:
            for i, arg in enumerate(function.args):
                function.args[i].var_type = func(arg.var_type)
            function.return_value = func(function.return_value)
            functions.append(function)
        except Exception as e:
            if not args.skip_errors:
                raise e
    obj.functions = functions

    for i, arg in enumerate(obj.variables):
        try:
            obj.variables[i].var_type = func(arg.var_type)
        except Exception as e:
            if not args.skip_errors:
                raise e
            else:
                obj._valid = False

def enumerate_on_variables_types_in_function(obj : CppFunction, func):
    try:
        obj.return_value = func(obj.return_value)
        for i, arg in enumerate(obj.args):
            obj.args[i].var_type = func(arg.var_type)
        obj.return_value = func(obj.return_value)
    except Exception as e:
        if not args.skip_errors:
            raise e
        else:
            obj._valid = False


def preprocess_and_count_variable(var_type : str, classes, known_types):
    if var_type in classes.keys():
        classes[var_type]["count"] += 1
        return var_type
    elif not(var_type in known_types):
        if var_type[-1] == "*":
            return "void" + "*" * var_type.count("*")
        elif args_exists != None and var_type in args_exists:
            return var_type
        else:
            print(Fore.RED, f"ERROR: used unknown type: {var_type}")
            raise SystemError(f"ERROR: used unknown type: {var_type}")
    else:
        return var_type


def preprocess_cpp(parsed_objects, namespaces):
    known_types = set()
    for t in BASE_TYPES:
        known_types.add(t)
        known_types.add("unsigned " + t)
        known_types.add("signed " + t)
        known_types.add("const " + t)
        known_types.add(t + "*")
        known_types.add(t + "**")
    for obj in parsed_objects:
        if isinstance(obj, CppClass):
            known_types.add(obj.name)
            known_types.add(obj.name + "*")
            known_types.add(obj.name + "**")
    # TODO: check if types exists
    classes = { obj.name : { "value" : obj, "count" : 0 } for obj in parsed_objects if isinstance(obj, CppClass)}
    for obj in parsed_objects:
        if isinstance(obj, CppClass):
            # enumerate_on_variables_types_in_object(obj, lambda var_type: get_clean_type(var_type, namespaces))
            enumerate_on_variables_types_in_object(obj, lambda var: preprocess_and_count_variable(var, classes, known_types))
        if isinstance(obj, CppFunction):
            # enumerate_on_variables_types_in_function(obj, lambda var_type: get_clean_type(var_type, namespaces))
            enumerate_on_variables_types_in_function(obj, lambda var: preprocess_and_count_variable(var, classes, known_types))
    new_parsed_objects = [o for o in parsed_objects if isinstance(o, CppClass) and o._valid]
    new_parsed_objects.sort(key=lambda obj: classes[obj.name]["count"], reverse=True)
    new_parsed_objects += [o for o in parsed_objects if isinstance(o, CppFunction) and o._valid]
    return new_parsed_objects

namespaces : set[str] = set()
parsed_classes = {}
parsed_functions = []
parsed_objects = []

def resolve_name(type_):
    segments = []
    is_ptr = False
    if isinstance(type_, Pointer):
        segments = type_.ptr_to.typename.segments
        is_ptr = True
    elif isinstance(type_, Type) or isinstance(type_, ClassDecl):
        segments = type_.typename.segments
    else:
        print(Fore.RED, f"ERROR: not supporting type: {type(type_)}")
        raise SystemError(f"ERROR: not supporting type: {type(type_)}")
    if len(segments) > 1:
        namespace = "::".join(map(lambda x: x.name , segments[:-1]))
        namespaces.add(namespace)

    if is_ptr:
        return segments[-1].name + "*"
    return segments[-1].name

ignore_classes = []
if args.ignore_classes:
    ignore_classes = args.ignore_classes


class MyVisitor(CxxVisitor):
    def on_namespace_end(self, state):
        for name in state.namespace.names:
            namespaces.add(name)

    def on_class_start(self, state):
        try:
            cpp_class = CppClass(resolve_name(state.class_decl))
            if cpp_class in ignore_classes:
                return
            print(f"{Fore.BLUE}Found a class {cpp_class.name}")
            parsed_classes[cpp_class.name] = cpp_class
        except Exception as e:
            if not args.skip_errors:
                raise e

    def on_class_field(self, state: ClassBlockState, f: Field) -> None:
        try:
            class_name = resolve_name(state.class_decl)
            if class_name in ignore_classes:
                return
            field_name = f.name
            field_type = resolve_name(f.type)
            field_state = f.access
            field_value = None
            if f.value:
                field_value = f.value.format()
            print(f"{Fore.BLUE}Found a {field_state} {field_type} {field_name}")
            parsed_classes[class_name].variables.append(CppClassVariable(field_type, field_name, field_state, field_value))
        except Exception as e:
            if not args.skip_errors:
                raise e

    def on_class_method(self, state: ClassBlockState, method: Method) -> None:
        try:
            class_name = resolve_name(state.class_decl)
            if class_name in ignore_classes:
                return
            method_name = method.name.segments[-1].name
            method_return_type = None
            if method.return_type:
                method_return_type = resolve_name(method.return_type)
            
            method_parameters = []
            for par in method.parameters:
                parameter = CppVariable(resolve_name(par.type), par.name)
                method_parameters.append(parameter)

            if method.constructor:
                c = CppClassConstructor(method_parameters)
                print(f"{Fore.BLUE}Found a {c}")
                parsed_classes[class_name].constructors.append(c)
            elif method.destructor:
                d = CppClassDestractor()
                print(f"{Fore.BLUE}Found a destructor for {class_name}")
                parsed_classes[class_name].destructor = d
            else:
                f = CppClassFunction(method_name, method.access, method_return_type, method_parameters)
                print(f"{Fore.BLUE}Found a {method.access} {f}")
                parsed_classes[class_name].functions.append(f)
        except Exception as e:
            if not args.skip_errors:
                raise e
    
    def on_function(self, state: NonClassBlockState, fn: Function) -> None:
        try:
            function_name = resolve_name(fn.name)
            fn_return_type = None
            if fn.return_type:
                fn_return_type = resolve_name(fn.return_type)
            
            fn_parameters = []
            for par in fn.parameters:
                parameter = CppVariable(resolve_name(par.type), par.name)
                fn_parameters.append(parameter)

            f = CppFunction(function_name, fn_return_type, fn_parameters)
            print(f"{Fore.BLUE}Found a {f}")
            parsed_functions.append(f)
        except Exception as e:
            if not args.skip_errors:
                raise e

def main():
    files = args.files
    out_cpp_file = args.o[0]
    out_lua_file = args.o[1]
    input_code = ""
    for file in files:
        with open(file, "r") as f:
            print(f"{Fore.MAGENTA}Reading file ", file)
            input_code += f.read() + "\n"

    visitor = MyVisitor()
    parser = CxxParser("", input_code, visitor)
    parser.parse()

    parsed_objects = [v for v in parsed_classes.values()] + parsed_functions

    # TODO: parse cpp function and bind them to lua

    parsed_objects = preprocess_cpp(parsed_objects, namespaces)

    print()
    print(Fore.RESET, parsed_objects)
    create_cpp_code(files, args.I, namespaces, parsed_objects, out_cpp_file)
    create_lua_code(parsed_objects, out_lua_file, args.custom_ffi_cdef)

if __name__ == "__main__":
    main()
    
    




