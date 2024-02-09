#version 330 core

in vec2 tex_pos;

out vec4 FragColor;

uniform vec4 background_color;
uniform vec4 text_color;
uniform sampler2D texture_0;
uniform vec4 texture_rect;

void main()
{
	vec2 final_position = texture_rect.xy + (texture_rect.zw * tex_pos);
	vec4 color = texture(texture_0, final_position);
	if(color[3] == 0) { FragColor = background_color; }
	else { FragColor = text_color; }
}