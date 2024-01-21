#pragma once

#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphic.h"
#include <map>
#include "model.h"
#include "physic.h"

class Part
{
	// Class representing a part of a map to load into a scene
public:
	Part(glm::vec3 a_position = glm::vec3(0, 0, 0), glm::vec3 a_rotation = glm::vec3(0.0, 0.0, 0.0), glm::vec3 a_scale = glm::vec3(1.0, 1.0, 1.0), std::string a_type = "", std::string a_texture_path = ""); // Part constructor
	Part(const Part& copy); // Part copy constructor
	~Part(); // Part destructor

	// Getters and setters
	inline glm::vec3 get_position() { return position; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline std::string get_texture_path() { return texture_path; };
	inline std::string get_type() { return type; };
private:
	glm::vec3 position; // Position of the part
	glm::vec3 rotation; // Rotation of the part
	glm::vec3 scale; // Scale of the part

	std::string texture_path; // Texture path of the part
	std::string type; // Type of the part
};

class Advanced_Struct : public Base_Struct
{
	// Class representing the advanced struct in the game
public:
	Advanced_Struct(double& a_mouse_x, double& a_mouse_y); // Advanced_Struct constructor
	void assign_part(unsigned int number, Part part); // Assign to a number a part
	bool contains_part(unsigned int number); // Returns if the struct contains a part
	bool contains_texture(std::string texture_path); // Returns if the struct contains a textures
	bool contains_vao(std::string type); // Returns if the struct contains a VAO
	Part* get_part(unsigned int number); // Returns a part
	Texture* get_texture(std::string texture_path); // Returns a texture in the struct
	void load_VAOs(); // Loads the VAOs in the advanced struct
	Part new_part(unsigned int number, std::string type, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(0, 0, 0), std::string texture_path = ""); // Create a new part into the struct and return it
	VAO* new_vao(std::string path, std::string type, std::string shader_path = "../shaders/default"); // Create a new VAO into the game
	~Advanced_Struct(); // Advanced_Struct destructor

	// Getters and setters
	inline std::map<std::string, VAO*> *get_all_vaos() { return &all_vaos; };
	inline std::map<unsigned int, Part>* get_parts() { return &parts; };
	inline std::map<std::string, Texture*>* get_textures() { return &textures; };
	inline std::map<std::string, std::string>* get_type() { return &types; };
	inline std::map<std::string, VAO*>* get_vaos() { return &all_vaos; };
private:
	std::map<unsigned int, Part> parts = std::map<unsigned int, Part>(); // Each parts, with their number as key, in the game
	std::map<std::string, Texture*> textures = std::map<std::string, Texture*>(); // Each texture, with their texture path as key, in the game
	std::map<std::string, std::string> types = std::map<std::string, std::string>(); // Each types, with their main type as key, in the game
	std::map<std::string, VAO*> all_vaos = std::map<std::string, VAO*>(); // Each vaos, with their type as key, in the game
};

class Object
{
	// Class representing an object into a scene
public:
	Object(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Object constructor
	virtual void update() { }; // Update the object
	~Object(); // Object destructor

	// Getters and setters
	inline Advanced_Struct* get_game_struct() { return game_struct; };
	inline Graphic_Object* get_attached_graphic_object() { return attached_graphic; };
	inline Physic_Object* get_attached_physic_object() { return attached_physic; };
	inline Transform_Object* get_attached_transform() { return attached_transform; };
	inline std::string get_name() { return name; };
	inline std::string get_scene_name() { return scene_name; };
	inline bool use_graphic() { return get_attached_graphic_object() != 0; };
	inline bool use_physic() { return get_attached_physic_object() != 0; };
private:
	std::string name; // Name of the object
	std::string scene_name; // Name of the scene of the object

	Advanced_Struct* game_struct = 0; // Base struct in the game
	Graphic_Object* attached_graphic = 0; // Graphic object attached
	Physic_Object* attached_physic = 0; // Physic object attached
	Transform_Object* attached_transform = 0; // Transform object attached
};