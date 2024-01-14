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
	Part(glm::vec3 a_position = glm::vec3(0, 0, 0), glm::vec3 a_rotation = glm::vec3(0.0, 0.0, 0.0), glm::vec3 a_scale = glm::vec3(1.0, 1.0, 1.0), std::string a_type = "", std::string a_texture_path = "");
	Part(const Part& copy);
	~Part();

	inline glm::vec3 get_position() { return position; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline std::string get_texture_path() { return texture_path; };
	inline std::string get_type() { return type; };
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::string texture_path;
	std::string type;
};

class Advanced_Struct
{
public:
	Advanced_Struct(Base_Struct *a_base_struct);
	void assign_part(unsigned int number, Part part);
	bool contains_part(unsigned int number);
	bool contains_texture(std::string texture_path);
	Part* get_part(unsigned int number);
	Texture* get_texture(std::string texture_path);
	Part new_part(unsigned int number, std::string type, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(0, 0, 0), std::string texture_path = "");
	~Advanced_Struct();

	inline std::map<std::string, VAO*> *get_all_vaos() { return &all_vaos; };
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline std::map<unsigned int, Part>* get_parts() { return &parts; };
	inline std::map<std::string, Texture*>* get_textures() { return &textures; };
	inline std::map<std::string, std::string>* get_type() { return &types; };
private:
	Base_Struct* base_struct = 0;
	std::map<unsigned int, Part> parts = std::map<unsigned int, Part>();
	std::map<std::string, Texture*> textures = std::map<std::string, Texture*>();
	std::map<std::string, std::string> types = std::map<std::string, std::string>();
	std::map<std::string, VAO*> all_vaos = std::map<std::string, VAO*>();
};