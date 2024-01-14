#pragma once

#include "base_struct.h"
#include <map>
#include "model.h"

class Part
{
	// Class representing a part of a map to load into a scene
public:
	Part(std::string a_texture_path = "");
	Part(const Part& copy);
	~Part();

	inline std::string get_texture_path() { return texture_path; };
private:
	std::string texture_path;
};

class Advanced_Struct
{
public:
	Advanced_Struct(Base_Struct *a_base_struct);
	void assign_part(unsigned int number, Part part);
	bool contains_part(unsigned int number);
	bool contains_texture(std::string texture_path);
	Texture* get_texture(std::string texture_path);
	Part new_part(unsigned int number, std::string texture_path = "");
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