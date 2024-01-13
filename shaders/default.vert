#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 in_tex_pos;

out vec2 tex_pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	tex_pos = in_tex_pos;
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
}