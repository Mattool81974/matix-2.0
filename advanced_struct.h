#pragma once

#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "model.h"

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

class Advanced_Struct
{
	// Class representing the advanced struct in the game
public:
	Advanced_Struct(Base_Struct *a_base_struct); // Advanced_Struct constructor
	void assign_part(unsigned int number, Part part); // Assign to a number a part
	bool contains_part(unsigned int number); // Returns if the struct contains a part
	bool contains_texture(std::string texture_path); // Returns if the struct contains a textures
	bool contains_vao(std::string type); // Returns if the struct contains a VAO
	Part* get_part(unsigned int number); // Returns a part
	Texture* get_texture(std::string texture_path); // Returns a texture in the struct
	Part new_part(unsigned int number, std::string type, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(0, 0, 0), std::string texture_path = ""); // Create a new part into the struct and return it
	VAO* new_vao(std::string path, std::string type, std::string shader_path = "../shaders/default"); // Create a new VAO into the game
	~Advanced_Struct(); // Advanced_Struct destructor

	// Getters and setters
	inline std::map<std::string, VAO*> *get_all_vaos() { return &all_vaos; };
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline std::map<unsigned int, Part>* get_parts() { return &parts; };
	inline std::map<std::string, Texture*>* get_textures() { return &textures; };
	inline std::map<std::string, std::string>* get_type() { return &types; };
	inline std::map<std::string, VAO*>* get_vaos() { return &all_vaos; };
private:
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	std::map<unsigned int, Part> parts = std::map<unsigned int, Part>(); // Each parts, with their number as key, in the game
	std::map<std::string, Texture*> textures = std::map<std::string, Texture*>(); // Each texture, with their texture path as key, in the game
	std::map<std::string, std::string> types = std::map<std::string, std::string>(); // Each types, with their main type as key, in the game
	std::map<std::string, VAO*> all_vaos = std::map<std::string, VAO*>(); // Each vaos, with their type as key, in the game
};