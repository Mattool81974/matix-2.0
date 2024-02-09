#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 in_tex_pos;
layout (location = 2) in vec4 in_tex_rect;
layout (location = 3) in vec3 in_tex_multiplier;

out vec3 tex_multiplier;
out vec2 tex_pos;
out vec4 tex_rect;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	tex_multiplier = in_tex_multiplier;
	tex_pos = in_tex_pos;
	tex_rect = in_tex_rect;
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
}