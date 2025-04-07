#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexSlot;

out vec4 bColor;
out vec2 TexCoord;
out float TexSlot;

void main()
{
    bColor = aColor;
    TexCoord = aTexCoord;
    TexSlot = aTexSlot;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
