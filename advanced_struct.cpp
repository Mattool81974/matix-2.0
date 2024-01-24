#include "advanced_struct.h"
#include <iostream>

// Part constructor
Part::Part(glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale, std::string a_type, std::string a_texture_path) : position(a_position), rotation(a_rotation), scale(a_scale), type(a_type), texture_path(a_texture_path)
{

}

// Part copy constructor
Part::Part(const Part& copy) : Part(copy.position, copy.rotation, copy.scale, copy.type, copy.texture_path)
{

}

// Part destructor
Part::~Part()
{

}

// Advanced_Struct constructor
Advanced_Struct::Advanced_Struct(double& a_mouse_x, double& a_mouse_y): Base_Struct(a_mouse_x, a_mouse_y)
{
	
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

// Returns if the struct contains a VAO
bool Advanced_Struct::contains_vao(std::string type)
{
	std::map<std::string, VAO*>* vaos = get_vaos();
	for (std::map<std::string, VAO*>::iterator it = vaos->begin(); it != vaos->end(); it++)
	{
		if (it->first == type) { return true; } // Verify each vaos name (first element of map)
	}
	return false;
}

// Returns a part
Part* Advanced_Struct::get_part(unsigned int number)
{
	if (contains_part(number))
	{
		return &(*get_parts())[number];
	}
	else
	{
		std::cout << "Matrix game : error ! The part \"" << number << "\" you want to get does not exist." << std::endl;
		return 0;
	}
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

// Loads the VAOs in the advanced struct
void Advanced_Struct::load_VAOs()
{
	// Create types
	types["chair"] = "chair";
	types["circle"] = "circle";
	types["cube"] = "cube";
	types["cylinder"] = "cylinder";
	types["player"] = "cube";
	types["one_faced_cube"] = "one_faced_cube";
	types["square"] = "triangle";
	types["table"] = "table";

	// Create VAOs
	all_vaos["chair"] = new VAO("../shaders/default", "../vbos/chair.vbo");
	all_vaos["circle"] = new VAO("../shaders/default", "../vbos/polygon50.vbo");
	all_vaos["cylinder"] = new VAO("../shaders/default", "../vbos/polygon_3d50.vbo");
	all_vaos["cube"] = new VAO("../shaders/default", "../vbos/cube.vbo");
	all_vaos["one_faced_cube"] = new VAO("../shaders/default", "../vbos/one_faced_cube.vbo");
	all_vaos["table"] = new VAO("../shaders/default", "../vbos/table.vbo");
	all_vaos["triangle"] = new VAO("../shaders/default", "");
}

// Create a new part into the struct and return it
Part Advanced_Struct::new_part(unsigned int number, std::string type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string texture_path)
{
	if (contains_part(number)) { std::cout << "Matrix game : error ! The part \"" << number << "\" you want to create already exist." << std::endl; }

	// Create the part
	Part part = Part(position, rotation, scale, type, texture_path);
	assign_part(number, part);
	return part;
}

// Create a new VAO into the game
VAO* Advanced_Struct::new_vao(std::string path, std::string type, std::string shader_path)
{
	if (contains_vao(path)) { std::cout << "Matrix game : error ! The \"" << type << "\" VAO already exists." << std::endl; return 0; }

	types[type] = type;
	all_vaos[type] = new VAO(shader_path, path);
	return all_vaos[type];
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

// Clossion_Result constructor
Collision_Result::Collision_Result(void* a_object1, void* a_object2) : object1(a_object1), object2(a_object2)
{

}

// Collision_Result copy constructor
Collision_Result::Collision_Result(const Collision_Result& copy) : Collision_Result(copy.object1, copy.object2)
{
	collide = copy.collide;
}

// Collision_Result destructor
Collision_Result::~Collision_Result()
{

}

// Object constructor
Object::Object(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : game_struct(a_game_struct), name(a_name), attached_transform(a_attached_transform), attached_graphic(a_attached_graphic), attached_physic(a_attached_physic), scene_name(a_scene_name)
{

}

// Returns if the object collides with an other object
Collision_Result Object::collides_with(Object *object)
{
	Collision_Result result(this, object);

	float object_height = object->get_attached_physic_object()->get_collision()->get_height() / 2.0; // Calculate if the objects collides on the Y axis
	float object_y = object->get_attached_transform()->get_absolute_position()[1];
	float height = get_attached_physic_object()->get_collision()->get_height() / 2.0;
	float y = get_attached_transform()->get_absolute_position()[1];
	if (object_y < y)
	{
		if (y - height > object_y + height)
		{
			result.set_colliding(false);
		}
	}
	else
	{
		if (y + height < object_y - height)
		{
			result.set_colliding(false);
		}
	}

	return result;
}

// Object destructor
Object::~Object()
{
	if (use_graphic()) { delete attached_graphic; attached_graphic = 0; }
	if (use_physic()) { delete attached_physic; attached_physic = 0; }
	delete attached_transform;
	attached_transform = 0;
}