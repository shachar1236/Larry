import re

# things I want my lua code to have acces to:
# ECS
# All my components
# singeltons
# Input system
# Events

def extract_namespace_content(cxx_code: str, namespace_name: str) -> str:
    """
    Extracts all content within a specified C++ namespace block
    by performing a character-by-character scan and tracking brace depth.
    This is generally more robust for nested structures than line-based regex.

    Args:
        cxx_code: A string containing the C++ code.
        namespace_name: The name of the namespace to extract (e.g., "Larry").

    Returns:
        A string containing all the content found within the specified
        namespace block, or an empty string if the namespace is not found.
    """
    # 1. Find the exact start position of the namespace's opening brace
    #    We need to make sure we're finding the '{' that directly follows the namespace declaration.
    namespace_start_pattern = re.compile(
        rf'namespace\s+{re.escape(namespace_name)}\s*{{',
        re.DOTALL
    )
    match = namespace_start_pattern.search(cxx_code)

    if not match:
        return "" # Namespace opening not found

    # Get the index immediately after the opening brace '{' of the namespace
    start_index = match.end()

    brace_depth = 1  # We just passed one opening brace
    content_chars = []
    
    # Iterate through the code character by character starting from after the opening brace
    i = start_index
    while i < len(cxx_code):
        char = cxx_code[i]

        if char == '{':
            brace_depth += 1
        elif char == '}':
            brace_depth -= 1

        # If brace_depth returns to 0, we found the matching closing brace for the namespace
        if brace_depth == 0:
            break # Stop scanning
        
        content_chars.append(char)
        i += 1

    return "".join(content_chars).strip()

def extract_extern_c_declarations_as_string(c_code: str) -> str:
    """
    Extracts all declarations within an 'extern "C"' block from C code
    and returns them as a single string, with each declaration on a new line.

    Args:
        c_code: A string containing the C code.

    Returns:
        A single string containing all declarations found within the
        extern "C" block, or an empty string if not found.
    """
    declarations_list = []
    match = re.search(r'extern "C"\s*{(.*)}', c_code, re.DOTALL)

    if match:
        extern_c_content = match.group(1)
        return extern_c_content
    return ""

def extract_struct_members_strictly(cxx_code: str, struct_name: str) -> str:
    """
    Extracts the name and strictly the member variable declarations of a specific struct
    from C++ code, by tracking curly brace depth to exclude methods and their internal logic.

    Args:
        cxx_code: A string containing the C++ code.
        struct_name: The name of the struct to extract (e.g., "Background").

    Returns:
        A string containing the struct name and its member variable declarations,
        formatted as requested, or an empty string if the struct is not found
        or contains no identifiable members.
    """
    # Regex to find the specific struct definition block.
    # It captures the content inside the struct's curly braces.
    struct_block_pattern = re.compile(
        rf'struct\s+{re.escape(struct_name)}\s*{{\s*(.*?)\s*}};',
        re.DOTALL
    )
    match = struct_block_pattern.search(cxx_code)

    if not match:
        return "" # Struct not found

    struct_content = match.group(1) # Content inside the outer struct braces
    struct_content_lines = struct_content.splitlines()

    data = ""
    brace_state = 0
    for line in struct_content_lines:
        if "{" in line:
             brace_state += 1
        elif "}" in line:
            brace_state -= 1
        elif brace_state == 0 and line.strip() != "":
            line = line.strip()
            if "=" in line:
                line = line[:line.index("=")].strip() + ";"
            data += line + "\n"


    return f"typedef struct {struct_name} {'{\n    ' + '\n    '.join(data.splitlines()) + '\n}'} {struct_name};"

def struct_to_typedef(code : str) -> str:
    found = False
    braces_count = 0
    struct_name = ""
    res = ""
    for line in code.splitlines(True):
        if not found and "struct" in line:
            found = True
            index = line.index("struct")
            line = line[:index] + " typedef " + line[index:]
            words = line.split(" ")
            struct_name = words[words.index("struct") + 1]

        if found and "{" in line:
            braces_count += 1
        if found and "}" in line:
            braces_count -= 1

            if braces_count == 0:
                index = line.index("}")
                line = line[:index+1] + " " + struct_name + " " + line[index+1:]

        res += line

    return res


lua_code = """
return [[
typedef struct Vec1 {
    float x;
} Vec1;
typedef struct Vec2 {
    float x, y;
} Vec2;
typedef struct Vec3 {
    float x, y, z;
} Vec3;
typedef struct Vec4 {
    float x, y, z, w;
} Vec4;


"""
lua_end_code = "]]"



with open("Larry/src/ECS/ECS_C.h", "r") as file:
    code = file.read()
    lua_code += extract_extern_c_declarations_as_string(code) + "\n\n"

with open("Larry/src/TextureLoader/TextureConfig.h", "r") as file:
    code = file.read()
    texture_loader = extract_namespace_content(code, "Larry")
    code = ""

    can_change = False
    for line in texture_loader.splitlines():
        if "TextureConfig" in line:
            can_change = True
        if can_change and "=" in line:
            line = line[:line.index("=")] + ";"
        code += line + "\n"

    lua_code += struct_to_typedef(code) + "\n\n"

with open("Larry/src/Scripts/lua/LarryApiFunctions.h", "r") as file:
    code = file.read()
    code = code.replace("Larry::", "")
    lua_code += extract_extern_c_declarations_as_string(code) + "\n\n"

components_files = [
  "Background",
  "Button",
  # "Camera",
  # "Projection",
  "Quad",
  "Relationship",
  "Transform",
]

for struct_name in components_files:
    with open(f"Larry/src/Application/Components/{struct_name}.h", "r") as file:
        s = extract_struct_members_strictly(file.read(), struct_name)
        s = s.replace("Math::", "")
        s = s.replace("TextureObject*", "void*")
        lua_code +=  s + "\n"

lua_code += "\n\n"

print(lua_code)

with open("Larry/src/Scripts/lua/CDef.lua", "w") as f:
    f.write(lua_code + lua_end_code)


