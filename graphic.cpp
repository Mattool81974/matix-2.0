#include "base_struct.h"
#include "graphic.h"
#include "model.h"

// Graphic_Object contructor
Graphic_Object::Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, VAO* a_vao) : base_struct(a_base_struct), attached_transform(a_attached_transform), vao(a_vao)
{

}

// Render the graphic object
void Graphic_Object::render()
{
	texture.bind();
	vao->get_shader_program()->set_uniform4fv_value("model", attached_transform.get_model_matrix());
	vao->get_shader_program()->set_uniform4fv_value("projection", get_base_struct()->get_projection());
	vao->get_shader_program()->set_uniform4fv_value("view", get_base_struct()->get_camera()->get_view());
	vao->render();
}

// Update the graphic object
void Graphic_Object::update() {}

// Graphic_Object destructor
Graphic_Object::~Graphic_Object()
{

}