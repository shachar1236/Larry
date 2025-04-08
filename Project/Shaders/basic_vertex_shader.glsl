#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexSlot;
layout (location = 4) in vec4 aModelCol1;
layout (location = 5) in vec4 aModelCol2;
layout (location = 6) in vec4 aModelCol3;
layout (location = 7) in vec4 aModelCol4;

out vec4 bColor;
out vec2 TexCoord;
out float TexSlot;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    mat4 model = mat4(aModelCol1, aModelCol2, aModelCol3, aModelCol4);
    
    bColor = aColor;
    TexCoord = aTexCoord;
    TexSlot = aTexSlot;
    gl_Position = u_Projection * u_View * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
