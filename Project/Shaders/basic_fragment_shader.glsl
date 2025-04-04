#version 330 core

in vec3 pos;

out vec4 FragColor;

void main()
{
    vec2 col = (pos.xy + vec2(1, 1)) / 2.0f;
    /* FragColor = vec4(col.x, col.y, 1.0f, 1.0f); */
    FragColor = vec4(0.5f, 0.3f, 1.0f, 1.0f);
} 
