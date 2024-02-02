#version 330 core

in vec2 tex_pos;

out vec4 FragColor;

uniform sampler2D texture_0;

void main()
{
	FragColor = texture(texture_0, tex_pos);
}