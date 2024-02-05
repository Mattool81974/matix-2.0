#include "base_struct.h"
#include "graphic.h"
#include "model.h"

// Graphic_Object contructor
Graphic_Object::Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, Texture *a_texture, VAO* a_vao) : base_struct(a_base_struct), attached_transform(a_attached_transform), texture(a_texture), vao(a_vao)
{

}

// Render the graphic object
void Graphic_Object::render()
{
	texture->bind(); // Bind the texture
	vao->get_shader_program()->set_uniform4fv_value("model", attached_transform.get_model_matrix()); // Write some uniform variables into the shader
	vao->get_shader_program()->set_uniform4fv_value("projection", get_base_struct()->get_projection());
	vao->get_shader_program()->set_uniform4fv_value("view", get_base_struct()->get_camera()->get_view());
	if (texture->use_resize())
	{
		vao->render(attached_transform.get_scale()); // Render the object with scaling
	}
	else
	{
		vao->render(glm::vec3(1.0, 1.0, 1.0)); // Render the object without scaling
	}
}

// Update the graphic object
void Graphic_Object::update() {}

// Graphic_Object destructor
Graphic_Object::~Graphic_Object()
{

}

// HUD_Object constructor
HUD_Object::HUD_Object(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao) : base_struct(a_base_struct), name(a_name), texture(a_texture), vao(a_vao)
{

}

// Return the transformation matrix of the HUD
glm::mat4 HUD_Object::get_model_matrix()
{
	glm::mat4 matrix = glm::mat4(1.0f);

	// Move matrix
	matrix = glm::translate(matrix, get_position());

	// Rotate matrix
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[1]), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[0]), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[2]), glm::vec3(0, 0, 1));

	// Scale matrix
	matrix = glm::scale(matrix, get_scale());

	return matrix;
}

// Render the HUD object
void HUD_Object::render()
{
	texture->bind(); // Bind the texture
	vao->get_shader_program()->set_uniform4fv_value("model", get_model_matrix()); // Write some uniform variables into the shader
	if (texture->use_resize())
	{
		vao->render(get_scale()); // Render the object with scaling
	}
	else
	{
		vao->render(glm::vec3(1.0, 1.0, 1.0)); // Render the object without scaling
	}
}

// Update the graphic object
void HUD_Object::update() {}

// HUD_Object destructor
HUD_Object::~HUD_Object()
{

}

// HUD_Text constructor
HUD_Text::HUD_Text(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao) : HUD_Object(a_base_struct, a_name, a_texture, a_vao)
{

}

// Render the text for HUD
void HUD_Text::render()
{
	texture->bind(); // Bind the texture
	glm::mat4 model = get_model_matrix();

	std::string final_text = get_text(true);
	unsigned short text_length = final_text.size();
	glm::vec2 text_size = get_texture()->size("a");
	float ratio = text_size[0] / text_size[1];
	glm::vec3 scale = glm::vec3(ratio / 2.0, 1.0, 1); // Calculate the necessary scale
	scale /= glm::vec3(text_length, text_length, text_length);
	scale = glm::normalize(scale) * glm::vec3(get_font_size(), get_font_size(), get_font_size());;

	glm::mat4 model_scaled_y = glm::scale(model, scale); // Calculate the scale of the character without changing anything other than the Y pos
	glm::mat4 model_scaled = model_scaled_y; // Calculate the scale of the character

	for (int i = 0; i < text_length; i++) // Generate each char one by one
	{
		char chr = final_text[i];

		if (chr == '\n') // Jump a line
		{
			model_scaled_y = glm::translate(model_scaled_y, glm::vec3(0, -1, 0));
			model_scaled = model_scaled_y;
		}
		else
		{
			vao->get_shader_program()->set_uniform4f_value("background_color", get_background_color()); // Write some uniform variables into the shader
			vao->get_shader_program()->set_uniform4f_value("text_color", get_font_color()); // Write some uniform variables into the shader
			vao->get_shader_program()->set_uniform4fv_value("model", model_scaled); // Write some uniform variables into the shader
			vao->get_shader_program()->set_uniform4f_value("texture_rect", ((Font_Texture*)texture)->get_character_rect(chr)); // Write some uniform variables into the shader

			((Font_VAO*)vao)->render(((Font_Texture*)texture)->get_character_rect(chr)); // Render the object
			model_scaled = glm::translate(model_scaled, glm::vec3(1, 0, 0));
		}
	}
}

// Update the text HUD
void HUD_Text::update()
{
	update_cursor();
	update_text_input();
}

// Update the cursor of the text
void HUD_Text::update_cursor()
{
	if (is_using_cursor())
	{
		time_since_last_cursor_display += get_base_struct()->get_delta_time();
	}
	else
	{
		display_cursor = true;
		time_since_last_cursor_display = 0;
	}

	if (time_since_last_cursor_display > get_cursor_display_time())
	{
		display_cursor = !display_cursor;
		time_since_last_cursor_display = 0;
	}
}

// Apply the text input to the text
void HUD_Text::update_text_input()
{
	if (is_focused() && can_take_input())
	{
		std::string authorized_text = get_input_text();
		std::vector<std::string>* pressed_keys = get_base_struct()->get_pressed_keys_frame();
		for (int i = 0; i < pressed_keys->size(); i++) // Check each pressed keys
		{
			std::string key = (*pressed_keys)[i];

			if (key == "left shift" || key == "right shift" || key == "backspace") continue; // Ignored keys

			if (key == "space") { key = " "; }
			else if (key == "enter") { key = "\n"; }

			if (get_base_struct()->get_key_state("left shift") || get_base_struct()->get_key_state("right shift")) // Uppercase the character
			{
				key = to_uppercase(key);
			}

			if (authorized_text == "all" || authorized_text.find(key) != std::string::npos) // If the key is authorized
			{
				set_text(get_text() + key);
				display_cursor = true;
				time_since_last_cursor_display = 0;
			}
		}

		if (get_base_struct()->get_key_state_frame("backspace") == Key_State::Pressed) // Delete a character
		{
			if (get_text().size() > 0)
			{
				set_text(get_text().substr(0, get_text().size() - 1));
				display_cursor = true;
				time_since_last_cursor_display = 0;
			}
		}
	}
}

// HUD_Text destructor
HUD_Text::~HUD_Text()
{

}