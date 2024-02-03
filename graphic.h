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
	inline Texture* get_texture() { return texture; };
	inline void set_texture(Texture* a_texture) { texture = a_texture; };
private:
	Transform_Object& attached_transform; // Reference to the attached transform object
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	VAO* vao = 0; // Pointer to the VAO used to render the object
	Texture *texture = 0; // Pointer to the Texture used to render the object
};

class HUD_Object
{
	// Class representing a HUD object
public:
	HUD_Object(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao); // HUD_Object constructor
	glm::mat4 get_model_matrix(); // Return the transformation matrix of the object
	virtual void render(); // Render the graphic HUD
	void update(); // Update the graphic HUD
	~HUD_Object(); // HUD_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline glm::vec3 get_position() { return glm::vec3(position[0], position[1], 0); };
	inline glm::vec3 get_rotation() { return glm::vec3(rotation[0], rotation[1], rotation[2]); };
	inline glm::vec3 get_scale() { return glm::vec3(scale[0] * 2, scale[1] * 2, 1); };
	inline Texture* get_texture() { return texture; };
	inline void set_position(glm::vec3 a_position) { position = a_position; };
	inline void set_rotation(glm::vec3 a_rotation) { rotation = a_rotation; };
	inline void set_scale(glm::vec3 a_scale) { scale = a_scale; };
	inline void set_texture(Texture* a_texture) { texture = a_texture; };
private:
	std::string name; // Name of the object

	glm::vec2 position = glm::vec2(0, 0); // Position of the HUD on the screen
	glm::vec3 rotation = glm::vec3(0, 0, 0); // Rotation of the HUD on the screen
	glm::vec2 scale = glm::vec2(1, 1); // Size of the HUD on the screen

	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	VAO* vao = 0; // Pointer to the VAO used to render the object
	Texture* texture = 0; // Pointer to the Texture used to render the object
};

class HUD_Font
{
	// Class representing an HUD font object
public:
	HUD_Font(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, Font_VAO* a_vao); // HUD_Font constructor
	void render(); // Render the font for HUD
	~HUD_Font(); // HUD_Object destructor
private:

};