#include "advanced_struct.h"
#include <iostream>

// Part constructor
Part::Part(std::string a_texture_path) : texture_path(a_texture_path)
{

}

// Part copy constructor
Part::Part(const Part& copy) : Part(copy.texture_path)
{

}

// Part destructor
Part::~Part()
{

}

// Advanced_Struct constructor
Advanced_Struct::Advanced_Struct(Base_Struct *a_base_struct): base_struct(a_base_struct)
{
	// Create types
	types["cube"] = "cube";
	types["square"] = "triangle";

	// Create VAOs
	all_vaos["cube"] = new VAO("../shaders/default", "cube");
	all_vaos["triangle"] = new VAO("../shaders/default", "triangle");
}

// Assign to a number a part
void Advanced_Struct::assign_part(unsigned int number, Part part)
{
	if (contains_part(number)) { std::cout << "Matrix game : error ! The part \"" << number << "\" you want to add already exist." << std::endl; return; }
	(*get_parts())[number] = part;
}

// Returns if the struct contains a part
bool Advanced_Struct::contains_part(unsigned int number)
{
	std::map<unsigned int, Part>* parts = get_parts();
	for (std::map<unsigned int, Part>::iterator it = parts->begin(); it != parts->end(); it++)
	{
		if (it->first == number) { return true; } // Verify each part name (first element of map)
	}
	return false;
}

// Returns if the struct contains a textures
bool Advanced_Struct::contains_texture(std::string texture_path)
{
	std::map<std::string, Texture*>* textures = get_textures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
	{
		if (it->first == texture_path) { return true; } // Verify each texture path (first element of map)
	}
	return false;
}

// Returns a texture in the struct
Texture* Advanced_Struct::get_texture(std::string texture_path)
{
	if (contains_texture(texture_path))
	{
		return (*get_textures())[texture_path];
	}
	else
	{
		std::cout << "Matrix game : error ! The texture of path\"" << texture_path << "\" is not loaded." << std::endl;
		return 0;
	}
}

// Create a new part into the struct and return it
Part Advanced_Struct::new_part(unsigned int number, std::string texture_path)
{
	if (contains_part(number)) { std::cout << "Matrix game : error ! The part \"" << number << "\" you want to create already exist." << std::endl; }

	// Create the part
	Part part = Part(texture_path);
	assign_part(number, part);
	return part;
}

// Advanced_Struct destructor
Advanced_Struct::~Advanced_Struct()
{
	std::map<std::string, VAO*>* vaos = get_all_vaos();
	for (std::map<std::string, VAO*>::iterator it = vaos->begin(); it != vaos->end(); it++)
	{
		delete it->second; // Delete VAOs
		it->second = 0;
	}

	std::map<std::string, Texture*>* textures = get_textures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
	{
		delete it->second; // Delete textures
		it->second = 0;
	}
}