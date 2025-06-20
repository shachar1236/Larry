import re

# things I want my lua code to have acces to:
# ECS
# All my components
# singeltons
# Input system
# Events

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


