#pragma once

#include "base_struct.h"
#include "model.h"

class Graphic_Object
{
public:
	Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, Texture *a_texture, VAO* a_vao);
	void render();
	void update();
	~Graphic_Object();

	inline Base_Struct* get_base_struct() { return base_struct; };
private:
	Transform_Object& attached_transform;
	Base_Struct* base_struct = 0;
	VAO* vao = 0;
	Texture *texture = 0;
};