#version 450 core

in vec4 bColor;
in vec2 TexCoord;
in float TexSlot;

out vec4 FragColor;

uniform sampler2D ourTextures[MAX_TEXTURE_UNITS];

void main()
{
    int txSlot = int(TexSlot);
    FragColor = texture(ourTextures[txSlot], TexCoord) * bColor;
    /* FragColor = vec4(ourTextures[txSlot] ,0.0f, 0.1f, 1.0f); */
} 
