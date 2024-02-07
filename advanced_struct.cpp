#include "advanced_struct.h"

// Part constructor
Part::Part(glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale, std::string a_type, std::string a_texture_path, void* a_base_object) : position(a_position), rotation(a_rotation), scale(a_scale), type(a_type), texture_path(a_texture_path), base_object(a_base_object)
{
	
}

// Part copy constructor
Part::Part(const Part& copy) : Part(copy.position, copy.rotation, copy.scale, copy.type, copy.texture_path, copy.base_object)
{
	
}

// Part destructor
Part::~Part()
{
	if (base_object != 0)
	{
		delete base_object;
		base_object = 0;
	}
}

// Advanced_Struct constructor
Advanced_Struct::Advanced_Struct(double& a_mouse_x, double& a_mouse_y, std::string a_exec_path): Base_Struct(a_mouse_x, a_mouse_y, a_exec_path)
{
	
}

// Assign to a number a part
void Advanced_Struct::assign_part(unsigned int number, Part* part)
{
	if (contains_part(number)) { std::cout << "Matrix game : error ! The part \"" << number << "\" you want to add already exist." << std::endl; return; }
	(*get_parts())[number] = part;
}

// Returns if the struct contains a font
bool Advanced_Struct::contains_font(std::string font_name)
{
	std::map<std::string, Font_Texture*>* textures = get_fonts_textures();
	for (std::map<std::string, Font_Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
	{
		if (it->first == font_name) { return true; } // Verify each font name (first element of map)
	}
	return false;
}

// Returns if the struct contains a part
bool Advanced_Struct::contains_part(unsigned int number)
{
	std::map<unsigned int, Part*>* parts = get_parts();
	for (std::map<unsigned int, Part*>::iterator it = parts->begin(); it != parts->end(); it++)
	{
		if (it->first == number) { return true; } // Verify each part name (first element of map)
	}
	return false;
}

// Returns if the struct contains a texture
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

// Returns a texture in the struct
Font_Texture* Advanced_Struct::get_font_texture(std::string font_name)
{
	if (contains_font(font_name))
	{
		return (*get_fonts_textures())[font_name];
	}
	else
	{
		Font_Texture* texture = new Font_Texture(font_name);
		(*get_textures())[font_name] = texture;
		return texture;
	}
}

// Returns a part
Part* Advanced_Struct::get_part(unsigned int number)
{
	if (contains_part(number))
	{
		return (*get_parts())[number];
	}
	else
	{
		std::cout << "Matrix game : error ! The part \"" << number << "\" you want to get does not exist." << std::endl;
		return 0;
	}
}

// Returns a texture in the struct
Texture* Advanced_Struct::get_texture(std::string texture_path, bool texture_resize)
{
	if (contains_texture(texture_path))
	{
		return (*get_textures())[texture_path];
	}
	else
	{
		Texture *texture = new Texture(texture_path, texture_resize);
		(*get_textures())[texture_path] = texture;
		return texture;
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
	types["hud"] = "hud";
	types["one_faced_cube"] = "one_faced_cube";
	types["player"] = "cube";
	types["square"] = "triangle";
	types["table"] = "table";

	// Define attributes for VAOs
	// Create base Shader_Program_Variable for the shader program
	std::vector<Shader_Program_Variable> base_3d_attributes = std::vector<Shader_Program_Variable>();
	std::vector<Shader_Program_Variable> hud_attributes = std::vector<Shader_Program_Variable>();
	Shader_Program_Variable v1 = Shader_Program_Variable();
	Shader_Program_Variable v2 = Shader_Program_Variable();
	Shader_Program_Variable v3 = Shader_Program_Variable();
	Shader_Program_Variable v4 = Shader_Program_Variable();
	v1.vector_size = 3;
	v2.vector_size = 2;
	v3.vector_size = 4;
	v4.vector_size = 3;
	base_3d_attributes.push_back(v1);
	base_3d_attributes.push_back(v2);
	base_3d_attributes.push_back(v3);
	base_3d_attributes.push_back(v4);
	hud_attributes.push_back(v1);
	hud_attributes.push_back(v2);

	// Create VAOs
	all_vaos["chair"] = new VAO("../shaders/default", base_3d_attributes, "../vbos/chair.vbo");
	all_vaos["circle"] = new VAO("../shaders/default", base_3d_attributes, "../vbos/polygon50.vbo");
	all_vaos["cylinder"] = new VAO("../shaders/default", base_3d_attributes, "../vbos/polygon_3d50.vbo");
	all_vaos["cube"] = new VAO("../shaders/default", base_3d_attributes, "../vbos/cube.vbo");
	all_vaos["default_font"] = new Font_VAO();
	all_vaos["hud"] = new VAO("../shaders/hud", hud_attributes, "0");
	all_vaos["one_faced_cube"] = new VAO("../shaders/default", base_3d_attributes, "../vbos/one_faced_cube.vbo");
	all_vaos["table"] = new VAO("../shaders/default", base_3d_attributes, "../vbos/table.vbo");
	all_vaos["triangle"] = new VAO("../shaders/default", base_3d_attributes, "0");

	// Create base texture
	textures["../fonts/default.png"] = new Font_Texture("../fonts/consolas.png");
}

// Create a new VAO into the game
VAO* Advanced_Struct::new_vao(std::string path, std::string type, std::string shader_path)
{
	if (contains_vao(path)) { std::cout << "Matrix game : error ! The \"" << type << "\" VAO already exists." << std::endl; return 0; }

	// Create base Shader_Program_Variable for the shader program
	std::vector<Shader_Program_Variable> base_3d_attributes = std::vector<Shader_Program_Variable>();
	Shader_Program_Variable v1 = Shader_Program_Variable();
	Shader_Program_Variable v2 = Shader_Program_Variable();
	Shader_Program_Variable v3 = Shader_Program_Variable();
	Shader_Program_Variable v4 = Shader_Program_Variable();
	v1.vector_size = 3;
	v2.vector_size = 2;
	v3.vector_size = 4;
	v4.vector_size = 3;
	base_3d_attributes.push_back(v1);
	base_3d_attributes.push_back(v2);
	base_3d_attributes.push_back(v3);
	base_3d_attributes.push_back(v4);

	types[type] = type;
	all_vaos[type] = new VAO(shader_path, base_3d_attributes, path);
	return all_vaos[type];
}

// Unload all the fonts
void Advanced_Struct::unload_fonts()
{
	std::map<std::string, Font_Texture*>* fonts = get_fonts_textures();
	for (std::map<std::string, Font_Texture*>::iterator it = fonts->begin(); it != fonts->end(); it++)
	{
		delete it->second; // Delete fonts
		it->second = 0;
	}
}

// Unload all the textures
void Advanced_Struct::unload_textures()
{
	std::map<std::string, Texture*>* textures = get_textures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
	{
		delete it->second; // Delete textures
		it->second = 0;
	}
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

	unload_fonts();
	unload_textures();

	std::map<unsigned int, Part*>* parts = get_parts();
	for (std::map<unsigned int, Part*>::iterator it = parts->begin(); it != parts->end(); it++)
	{
		delete it->second; // Delete parts
		it->second = 0;
	}
}

// Clossion_Result constructor
Collision_Result::Collision_Result(void* a_parent) : parent(a_parent)
{

}

// Collision_Result copy constructor
Collision_Result::Collision_Result(const Collision_Result& copy) : Collision_Result(copy.parent)
{
	collide = copy.collide;
	positions = copy.positions;
}

// Collision_Result destructor
Collision_Result::~Collision_Result()
{

}

// Object constructor
Object::Object(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : game_struct(a_game_struct), name(a_name), attached_transform(a_attached_transform), attached_graphic(a_attached_graphic), attached_physic(a_attached_physic), scene_name(a_scene_name), collision_result(this)
{
	
}

// Clone the object
void* Object::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
	return new Object(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Returns if the object collides with an other object
One_Collision Object::collides_with(Object *object)
{
	One_Collision result;
	result.object1 = this;
	result.object2 = object;
	return result;
}

// Return the list of pos in the map
std::vector<glm::vec3> Object::get_all_map_pos(glm::vec3 movement_use, bool add_movement)
{
	std::vector<glm::vec3> positions = std::vector<glm::vec3>();

	glm::vec3 map_pos = get_map_pos() - glm::vec3(0.5, 0.5, 0.5);

	if (!use_physic() || !get_attached_physic_object()->use_collision()) { return positions; }
	float scale0 = get_attached_physic_object()->get_collision()->get_width();
	float scale1 = get_attached_physic_object()->get_collision()->get_height();
	float scale2 = get_attached_physic_object()->get_collision()->get_length();

	int min_x = (int)glm::floor(map_pos[0] - scale0 / 2.0); // Calculate minimum and maximum pos
	int max_x = (int)glm::ceil(map_pos[0] + scale0 / 2.0);
	int min_y = (int)glm::floor(map_pos[1] - scale1 / 2.0);
	int max_y = (int)glm::ceil(map_pos[1] + scale1 / 2.0);
	int min_z = (int)glm::floor(map_pos[2] - scale2 / 2.0);
	int max_z = (int)glm::ceil(map_pos[2] + scale2 / 2.0);

	if (movement_use[0] != 0) // If the X movement is used
	{
		if (get_attached_transform()->get_movement()[0] < 0)
		{
			min_x = (int)glm::floor((map_pos[0] - scale0 / 2.0) + get_attached_transform()->get_movement()[0] * movement_use[0]);
		}
		else
		{
			max_x = (int)glm::ceil((map_pos[0] + scale0 / 2.0) + get_attached_transform()->get_movement()[0] * movement_use[0]);
		}

		for (int i = min_x; i < max_x; i++) // Create each positions with x movement
		{
			for (int j = min_y; j < max_y; j++)
			{
				for (int k = min_z; k < max_z; k++)
				{
					glm::vec3 vector = glm::vec3(i + 1, j + 1, k + 1);
					if(add_movement) positions.push_back(glm::vec3(1, 0, 0));
					positions.push_back(vector);
				}
			}
		}

		min_x = (int)glm::floor(map_pos[0] - scale0 / 2.0);
		max_x = (int)glm::ceil(map_pos[0] + scale0 / 2.0);
	}

	if (movement_use[1] != 0) // If the Y movement is used
	{
		if (get_attached_transform()->get_movement()[1] < 0)
		{
			min_y = (int)glm::floor((map_pos[1] - scale1 / 2.0) + get_attached_transform()->get_movement()[1] * movement_use[1]);
		}
		else
		{
			max_y = (int)glm::ceil((map_pos[1] + scale1 / 2.0) + get_attached_transform()->get_movement()[1] * movement_use[1]);
		}

		for (int i = min_x; i < max_x; i++) // Create each positions with y movement
		{
			for (int j = min_y; j < max_y; j++)
			{
				for (int k = min_z; k < max_z; k++)
				{
					glm::vec3 vector = glm::vec3(i + 1, j + 1, k + 1);
					if (add_movement) positions.push_back(glm::vec3(0, 1, 0));
					positions.push_back(vector);
				}
			}
		}

		min_y = (int)glm::floor(map_pos[1] - scale1 / 2.0);
		max_y = (int)glm::ceil(map_pos[1] + scale1 / 2.0);
	}

	if (movement_use[2] != 0) // If the Z movement is used
	{
		if (get_attached_transform()->get_movement()[2] < 0)
		{
			min_z = (int)glm::floor((map_pos[2] - scale2 / 2.0) + get_attached_transform()->get_movement()[2] * movement_use[2]);
		}
		else
		{
			max_z = (int)glm::ceil((map_pos[2] + scale2 / 2.0) + get_attached_transform()->get_movement()[2] * movement_use[2]);
		}
	}
	
	if (movement_use == glm::vec3(0, 0, 0) || movement_use[2] != 0)
	{
		for (int i = min_x; i < max_x; i++) // Create each positions
		{
			for (int j = min_y; j < max_y; j++)
			{
				for (int k = min_z; k < max_z; k++)
				{
					glm::vec3 vector = glm::vec3(i + 1, j + 1, k + 1);
					if (add_movement) positions.push_back(glm::vec3(0, 0, 1));
					positions.push_back(vector);
				}
			}
		}
	}

	return positions;
}

// Update the object after physic modification
void Object::last_update()
{
	for (int i = 0; i < get_collisions_result()->size(); i++) // Check each collisions
	{
		One_Collision* collision = get_collisions_result()->at(i);
		Object* other = (Object*)collision->object1;
		if(other == this) {other = (Object*)collision->object2;}

		// Calculate the differences
		float difference_collision_height = glm::abs(get_attached_physic_object()->get_collision()->get_height() / 2.0 + other->get_attached_physic_object()->get_collision()->get_height() / 2.0);
		float difference_collision_length = glm::abs(get_attached_physic_object()->get_collision()->get_length() / 2.0 + other->get_attached_physic_object()->get_collision()->get_length() / 2.0);
		float difference_collision_width = glm::abs(get_attached_physic_object()->get_collision()->get_width() / 2.0 + other->get_attached_physic_object()->get_collision()->get_width() / 2.0);
		float difference_x = (get_map_pos()[0]) - other->get_map_pos()[0];
		float difference_y = (get_map_pos()[1]) - other->get_map_pos()[1];
		float difference_z = (get_map_pos()[2]) - other->get_map_pos()[2];
		float difference_x_movement = (get_map_pos()[0] + get_attached_transform()->get_movement()[0]) - other->get_map_pos()[0];
		float difference_y_movement = (get_map_pos()[1] + get_attached_transform()->get_movement()[1]) - other->get_map_pos()[1];
		float difference_z_movement = (get_map_pos()[2] + get_attached_transform()->get_movement()[2]) - other->get_map_pos()[2];

		glm::vec3 new_movement = get_attached_transform()->get_movement();
		glm::vec3 new_velocity = get_attached_physic_object()->get_velocity();
		if (collision->axis_multiplier[0] != 0 && glm::abs(difference_x_movement) < glm::abs(difference_collision_width) && sign(difference_x_movement) != sign(new_movement[0]))
		{
			if (sign(new_movement[0]) == sign(new_velocity[0])) new_velocity[0] *= -1 * get_attached_physic_object()->get_elasticity();
			new_movement[0] = 0;		
		}

		if (collision->axis_multiplier[1] != 0 && new_movement[1] != 0 && glm::abs(difference_y_movement) < glm::abs(difference_collision_height) && sign(difference_y_movement) != sign(new_movement[1]))
		{
			if (sign(new_movement[1]) == sign(new_velocity[1])) new_velocity[1] *= -1 * get_attached_physic_object()->get_elasticity();
			new_movement[1] = 0;
		}

		if (collision->axis_multiplier[2] != 0 && glm::abs(difference_z_movement) < glm::abs(difference_collision_length) && sign(difference_z_movement) != sign(new_movement[2]))
		{
			if (sign(new_movement[2]) == sign(new_velocity[2])) new_velocity[2] *= -1 * get_attached_physic_object()->get_elasticity();
			new_movement[2] = 0;
		}

		get_attached_transform()->set_movement(new_movement);
		get_attached_physic_object()->set_velocity(new_velocity);
	}

	get_collisions_result()->clear(); // Clear the object
	last_map_pos = map_pos;
};

// Change the middle pos of the object in the map and return the list of pos in the map
std::vector<glm::vec3> Object::set_map_pos(glm::vec3 a_map_pos)
{
	map_pos = a_map_pos;

	return get_all_map_pos();
}

// Object destructor
Object::~Object()
{
	if (use_graphic()) { delete attached_graphic; attached_graphic = 0; }
	if (use_physic()) { delete attached_physic; attached_physic = 0; }
	delete attached_transform;
	attached_transform = 0;
}