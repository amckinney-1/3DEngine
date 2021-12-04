#version 430 core
flat in vec3 fs_color;
in vec2 fs_textcoord;

out vec4 outColor;

uniform sampler2D textureSampler;

void main()
{
    outColor = texture(textureSampler, fs_textcoord) * vec4(fs_color, 1);
}