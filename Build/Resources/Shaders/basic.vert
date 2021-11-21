#version 430 core 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textcoord;

out vec3 fs_color;
out vec2 fs_textcoord;

uniform float scale;

void main()
{
	fs_color = color;
	fs_textcoord = textcoord;
    gl_Position = vec4(position * scale, 1.0);
}