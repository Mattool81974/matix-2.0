#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_position;

out vec2 tex_pos;

uniform mat4 model;

void main()
{
	tex_pos = texture_position;
    gl_Position = model * vec4(position.xyz, 1.0);
}