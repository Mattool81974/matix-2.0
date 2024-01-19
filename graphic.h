#pragma once

#include "base_struct.h"
#include "model.h"

class Graphic_Object
{
	// Class representing a graphic object
public:
	Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, Texture *a_texture, VAO* a_vao); // Graphic_Object contructor
	void render(); // Render the graphic object
	void update(); // Update the graphic object
	~Graphic_Object(); // Graphic_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
private:
	Transform_Object& attached_transform; // Reference to the attached transform object
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	VAO* vao = 0; // Pointer to the VAO used to render the object
	Texture *texture = 0; // Pointer to the Texture used to render the object
};