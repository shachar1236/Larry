import sys
import re
import argparse
from colorama import Fore
from cpp_types import *
from cpp_code_generator import *

def iswhitespace(char):
    return char == " " or char == "\t" or char == "\n" or char.isspace()

OPERATORS = ["=", ";", ",", ".", "{", "}", "(", ")"]

class Lexer():

    def __init__(self, code):
        self.code = code

    def _peek(self):
        found_start = False
        start_index = 0
        end_index = 0
        for i, char in enumerate(self.code):
            if iswhitespace(char):
                if found_start:
                    end_index = i
                    break
            elif char in OPERATORS:
                if found_start:
                    end_index = i
                    break
                else:
                    found_start = True
                    start_index = i
                    end_index = i+1
                    break
            elif not found_start:
                start_index = i
                found_start = True
        
        if not found_start:
            return "", 0, 0

        if end_index == 0:
            end_index = len(self.code)

        token = self.code[start_index:end_index]
        return token, start_index, end_index

    def peek(self):
        token, _, _ = self._peek()
        return token

    def next(self):
        token, start_index, end_index = self._peek()
        if token != "":
            self.code = self.code[end_index:]
        return token

def parse_type(lexer : Lexer):
    t = lexer.next()
    if t == "const":
        t2 = lexer.next()
        # TODO: check if type is in allowed types
        return f"{t} {t2}"
    return t
        
def parse_function_after_type(lexer : Lexer, line : str) -> tuple[str, list[CppVariable]]:
    function_name = lexer.next()
    pare = lexer.next()
    if pare == "(" and ")" in line:
        args : list[CppVariable] = []
        while lexer.peek() != ")":
            if lexer.peek() == ",":
                lexer.next()
            args.append(CppVariable(parse_type(lexer), lexer.next()))

        return function_name, args
                
    return "", []

def parse_if_class_function(line, state):
    lexer = Lexer(line)
    return_type = parse_type(lexer)
    function_name, args_types = parse_function_after_type(lexer, line)
    if function_name != "":
        return CppClassFunction(function_name, state, return_type, args_types)
    
    return None

def parse_if_constructor(line, curr_class_name):
    function_name, args_types = parse_function_after_type(Lexer(line), line)
    if function_name != "" and function_name == curr_class_name:
        return CppClassConstructor(args_types)
    return None

def parse_if_destractor(line, curr_class_name):
    lexer = Lexer(line)
    class_name = lexer.next()
    if class_name != "" and class_name[0] == "~" and class_name[1:] == curr_class_name:
        return CppClassDestractor()
    return None

def parse_if_variable(line, state):
    lexer = Lexer(line)
    var_type = parse_type(lexer)
    var_name = lexer.next()
    last_token = lexer.next()
    if var_type != "" and var_name != "":
        if last_token == ";":
            return CppClassVariable(var_type, var_name, state)

        if last_token == "=":
            default_value = lexer.next()
            semi_colon = lexer.next();
            if default_value != "" and semi_colon == ";":
                return CppClassVariable(var_type, var_name, state, default_value)
    return None

def CreateFunctionsTable(cpp_class : CppClass) -> str:
    code = f"sol::table create_{cpp_class.name}_functions_table(sol::state_view& lua)" + "\n{\n"
    code += "\tsol::table table = lua.create_table_with(\n"
    for i, func in enumerate(cpp_class.functions):
        code += f'\t\t"{func.name}", &{cpp_class.name}::{func.name}'
        if i != len(cpp_class.functions) - 1:
          code += ","
        code += "\n"
    code += "\t);\n"
    code += "\treturn table;\n}"
    return code

def validate_cpp(parsed_objects):
    # TODO: check if types exists
    return True

            

def create_lua_code(parsed_objects, out_file):
    pass

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument("files", nargs="+")
    parser.add_argument("-o", action="store", nargs=2, help="The output files, the first is the cpp file and the second is the lua file.")
    parser.add_argument("--ignore-classes", "-ic", action="store", nargs="+", help="Dont parse classes with the provided names.")
    parser.add_argument("--exists", "-e", action="store", nargs="+", help="Act as if the provided classes already exists and dont throw an error when when refrencing them.")
    parser.add_argument("-I", action="store", nargs="+", help="Directories where the header file are.")

    args = parser.parse_args()

    files = args.files
    out_cpp_file = args.o[0]
    out_lua_file = args.o[1]
    ignore_classes = []
    if args.ignore_classes:
        ignore_classes = args.ignore_classes

    input_code = ""
    for file in files:
        with open(file, "r") as f:
            print(f"{Fore.MAGENTA}Reading file ", file)
            input_code += f.read() + "\n"

    parsed_objects = []

    in_class = False
    curr_class : CppClass = None
    class_name = ""
    class_brace_count = 0
    class_fields_state = "private"
    namespaces = []

    code_lines = input_code.splitlines()

    for line in code_lines:
        lexer = Lexer(line)

        if in_class:
            token = lexer.next()

            match token:
                case "{":
                    class_brace_count += 1
                case "}":
                    class_brace_count -= 1
                    if class_brace_count == 0:
                        in_class = False
                        parsed_objects.append(curr_class)
                        curr_class = None
                case "public:" | "private:":
                    class_fields_state = token[:-1]
                    print(f"{Fore.CYAN}Class fields stated changed to {token[:-1]}")
                case _:
                    not_in_function = class_brace_count == 1
                    if not_in_function:
                        v = parse_if_variable(line, class_fields_state)
                        if v != None:
                            print(f"{Fore.BLUE}Found a {class_fields_state} {v}")
                            curr_class.variables.append(v)
                        elif (d := parse_if_destractor(line, class_name)) != None:
                            print(f"{Fore.BLUE}Found a {d}")
                            curr_class.destructor = d
                        elif class_fields_state == "public":
                            if (c := parse_if_constructor(line, class_name)) != None:
                                print(f"{Fore.BLUE}Found a {c}")
                                curr_class.constructors.append(c)
                            elif (f := parse_if_class_function(line, class_fields_state)) != None:
                                print(f"{Fore.BLUE}Found a {class_fields_state} {f}")
                                curr_class.functions.append(f)


        while (token := lexer.next()) != "":
            match token:
                case "namespace":
                    namespaces.append(lexer.next())
                case "struct" | "class":
                    class_name = lexer.next()
                    if class_name in ignore_classes:
                        continue
                    end = lexer.next()
                    match end:
                        case "{":
                            class_brace_count += 1
                        case "}":
                            class_brace_count -= 1

                    if end != ";":
                        in_class = True
                        curr_class = CppClass(class_name)
                        print(f"{Fore.GREEN}Found {token} {class_name}")
                        if token == "struct":
                            class_fields_state = "public"
                        else:
                            class_fields_state = "private"

    print(Fore.RESET,parsed_objects)

    if validate_cpp(parsed_objects):
        create_cpp_code(files, args.I, namespaces, parsed_objects, out_cpp_file)

# need to set lua object, setmetatable(o, {__index = {functions}})

if __name__ == "__main__":
    main()
    
    




