#version 330 core

in vec2 tex_pos;

out vec4 FragColor;

uniform sampler2D texture_0;
uniform vec4 texture_rect;

void main()
{
	vec2 final_position = texture_rect.xy + (texture_rect.zw * tex_pos);
	vec4 color = texture(texture_0, final_position);
	FragColor = color;
}