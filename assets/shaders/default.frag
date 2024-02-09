#version 330 core

in vec3 tex_multiplier;
in vec2 tex_pos;
in vec4 tex_rect;

out vec4 FragColor;

uniform vec3 scale;
uniform sampler2D texture_0;

void main()
{
	vec2 final_tex_pos = (tex_pos - tex_rect.xy);
	
	if(tex_multiplier.x == 0)
	{
		final_tex_pos.x = final_tex_pos.x * scale.x;
	}
	else if(tex_multiplier.x == 1)
	{
		final_tex_pos.y = final_tex_pos.y * scale.x;
	}
	
	if(tex_multiplier.y == 0)
	{
		final_tex_pos.x = final_tex_pos.x * scale.y;
	}
	else if(tex_multiplier.y == 1)
	{
		final_tex_pos.y = final_tex_pos.y * scale.y;
	}
	
	if(tex_multiplier.z == 0)
	{
		final_tex_pos.x = final_tex_pos.x * scale.z;
	}
	else if(tex_multiplier.z == 1)
	{
		final_tex_pos.y = final_tex_pos.y * scale.z;
	}
	
	while(final_tex_pos.x > tex_rect[2])
	{
		final_tex_pos.x -= tex_rect[2];
	}
	while(final_tex_pos.y > tex_rect[3])
	{
		final_tex_pos.y -= tex_rect[3];
	}
	
	FragColor = texture(texture_0, tex_rect.xy + final_tex_pos);
}