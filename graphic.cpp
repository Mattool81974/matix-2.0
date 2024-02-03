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

// HUD_Font constructor
HUD_Font::HUD_Font(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao) : HUD_Object(a_base_struct, a_name, a_texture, a_vao)
{

}

// Render the font for HUD
void HUD_Font::render()
{

}

// HUD_Object destructor
HUD_Font::~HUD_Font()
{

}