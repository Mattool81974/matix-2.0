#include <fstream>
#include <iostream>
#include <map>
#include "scene.h"
#include <string>
#include <sstream>
#include <vector>

// Map_Level_Collection constructor
Map_Level_Collection::Map_Level_Collection()
{

}

// Map_Level_Collection copy constructor
Map_Level_Collection::Map_Level_Collection(const Map_Level_Collection& copy) : Map_Level_Collection()
{
	part = copy.part;
	base_position = copy.base_position;
	final_position = copy.final_position;
	orientation = copy.orientation;
	rotation = copy.rotation;
	scale = copy.scale;
}

// Map_Level_Collection destructor
Map_Level_Collection::~Map_Level_Collection()
{

}

// Graphic_Scene constructor
Graphic_Scene::Graphic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects): game_struct(a_game_struct), name(a_name), objects(a_objects)
{

}

// Create a new object into the scene and return it
Graphic_Object* Graphic_Scene::new_object(std::string name, Transform_Object &transform, std::string type, std::string texture_path, bool texture_resize)
{
	// Configure variables for creation
	if (texture_path == "")
	{
		if ((*get_game_struct()->get_type())[type] == "cube")
		{
			texture_path = "../textures/unknow_cube.png";
		}
		else
		{
			texture_path = "../textures/unknow.png";
		}
	}

	// Create or get the texture
	Texture* texture = get_game_struct()->get_texture(texture_path, texture_resize);

	// Create and add the object
	VAO* vao = (*get_game_struct()->get_all_vaos())[(*get_game_struct()->get_type())[type]];
	Graphic_Object* object = new Graphic_Object(get_game_struct(), transform, texture, vao);

	return object;
}

// Render the objects in the scene
void Graphic_Scene::render()
{
	std::map<std::string, Object*>* objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->second->use_graphic())
		{
			it->second->get_attached_graphic_object()->render(); // Render each object
		}
	}
}

// Update the objects in the scene
void Graphic_Scene::update()
{
	std::map<std::string, Object*>* objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->second->use_graphic())
		{
			it->second->get_attached_graphic_object()->update(); // Update each object one by one
		}
	}
}

// Graphic_Scene destructor
Graphic_Scene::~Graphic_Scene()
{
	
}

// Physic_Scene constructor
Physic_Scene::Physic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects, std::map<unsigned short, std::vector<std::vector<Object*>>>& a_objects_map): game_struct(a_game_struct), name(a_name), objects(a_objects), objects_map(a_objects_map)
{

}

// Check the collisions in the system
void Physic_Scene::check_collisions()
{
	std::vector<Object*> dynamic_objects = std::vector<Object*>();
	for (std::map<std::string, Object*>::iterator it = get_objects()->begin(); it != get_objects()->end(); it++)
	{
		if (it->second->use_physic() && !it->second->get_attached_physic_object()->is_static() && it->second->get_attached_physic_object()->use_collision())
		{
			dynamic_objects.push_back(it->second);
		}
	}

	for (int i = 0; i < dynamic_objects.size(); i++)
	{
		Object* object = dynamic_objects[i];
		glm::vec2 pos = glm::vec2(object->get_attached_transform()->get_position()[0] + 0.5f, object->get_attached_transform()->get_position()[2] + 0.5f); // Calculate max and min pos
		glm::vec2 pos_movement = glm::vec2(object->get_attached_transform()->get_position()[0] + 0.5f + object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_position()[2] + 0.5f + object->get_attached_transform()->get_movement()[2]); // Calculate max and min pos
		glm::vec2 max_pos = glm::vec2(glm::floor(pos[0] + object->get_attached_physic_object()->get_collision()->get_width()), glm::floor(pos[1] + object->get_attached_physic_object()->get_collision()->get_width()));
		glm::vec2 min_pos = glm::vec2(glm::floor(pos[0] - object->get_attached_physic_object()->get_collision()->get_width()), glm::floor(pos[1] - object->get_attached_physic_object()->get_collision()->get_width()));
		glm::vec2 max_pos_movement = glm::vec2(glm::floor(pos[0] + object->get_attached_physic_object()->get_collision()->get_width() + object->get_attached_transform()->get_movement()[0]), glm::floor(pos[1] + object->get_attached_physic_object()->get_collision()->get_width() + object->get_attached_transform()->get_movement()[2]));
		glm::vec2 min_pos_movement = glm::vec2(glm::floor(pos[0] - object->get_attached_physic_object()->get_collision()->get_width() + object->get_attached_transform()->get_movement()[0]), glm::floor(pos[1] - object->get_attached_physic_object()->get_collision()->get_width() + object->get_attached_transform()->get_movement()[2]));
		
		if (max_pos[0] >= 0 and max_pos[0] < get_objects_map()->size()) // Check max position X collision
		{
			if (max_pos_movement[1] >= 0 and max_pos_movement[1] < (*get_objects_map())[max_pos[0]].size())
			{
				if ((*get_objects_map())[max_pos[0]][max_pos_movement[1]] != 0) // Check max position X with max position Y collision
				{
					Object* collide_object = (*get_objects_map())[max_pos[0]][max_pos_movement[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[2] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (pos_movement[1] >= 0 and pos_movement[1] < (*get_objects_map())[max_pos[0]].size()) // Check max position X with position Y collision
			{
				if ((*get_objects_map())[max_pos[0]][pos_movement[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[max_pos[0]][pos_movement[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[2] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (min_pos_movement[1] >= 0 and min_pos_movement[1] < (*get_objects_map())[max_pos[0]].size()) // Check max position X with min position Y collision
			{
				if ((*get_objects_map())[max_pos[0]][min_pos_movement[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[max_pos[0]][min_pos_movement[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[2] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}
		}

		if (max_pos_movement[0] >= 0 and max_pos_movement[0] < get_objects_map()->size()) // Check max position X collision
		{
			if (max_pos[1] >= 0 and max_pos[1] < (*get_objects_map())[max_pos_movement[0]].size())
			{
				if ((*get_objects_map())[max_pos_movement[0]][max_pos[1]] != 0) // Check max position Y with max position X collision
				{
					Object* collide_object = (*get_objects_map())[max_pos_movement[0]][max_pos[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[0] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (pos[1] >= 0 and pos[1] < (*get_objects_map())[max_pos_movement[0]].size()) // Check position Y with max position X collision
			{
				if ((*get_objects_map())[max_pos_movement[0]][pos[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[max_pos_movement[0]][pos[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[0] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (min_pos[1] >= 0 and min_pos[1] < (*get_objects_map())[max_pos_movement[0]].size()) // Check min position Y with max position X collision
			{
				if ((*get_objects_map())[max_pos_movement[0]][min_pos[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[max_pos_movement[0]][min_pos[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[0] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}
		}

		if (min_pos[0] >= 0 and min_pos[0] < get_objects_map()->size()) // Check min position X collision
		{
			if (max_pos_movement[1] >= 0 and max_pos_movement[1] < (*get_objects_map())[min_pos[0]].size())
			{
				if ((*get_objects_map())[min_pos[0]][max_pos_movement[1]] != 0) // Check max position X with max position Y collision
				{
					Object* collide_object = (*get_objects_map())[min_pos[0]][max_pos_movement[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[2] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (pos_movement[1] >= 0 and pos_movement[1] < (*get_objects_map())[min_pos[0]].size()) // Check max position X with position Y collision
			{
				if ((*get_objects_map())[min_pos[0]][pos_movement[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[min_pos[0]][pos_movement[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[2] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (min_pos_movement[1] >= 0 and min_pos_movement[1] < (*get_objects_map())[min_pos[0]].size()) // Check max position X with min position Y collision
			{
				if ((*get_objects_map())[min_pos[0]][min_pos_movement[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[min_pos[0]][min_pos_movement[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[2] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}
		}

		if (min_pos_movement[0] >= 0 and min_pos_movement[0] < get_objects_map()->size()) // Check max position X collision
		{
			if (max_pos[1] >= 0 and max_pos[1] < (*get_objects_map())[min_pos_movement[0]].size())
			{
				if ((*get_objects_map())[min_pos_movement[0]][max_pos[1]] != 0) // Check max position Y with max position X collision
				{
					Object* collide_object = (*get_objects_map())[min_pos_movement[0]][max_pos[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[0] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (pos[1] >= 0 and pos[1] < (*get_objects_map())[min_pos_movement[0]].size()) // Check position Y with max position X collision
			{
				if ((*get_objects_map())[min_pos_movement[0]][pos[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[min_pos_movement[0]][pos[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[0] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}

			if (min_pos[1] >= 0 and min_pos[1] < (*get_objects_map())[min_pos_movement[0]].size()) // Check min position Y with max position X collision
			{
				if ((*get_objects_map())[min_pos_movement[0]][min_pos[1]] != 0)
				{
					Object* collide_object = (*get_objects_map())[min_pos_movement[0]][min_pos[1]];
					Collision_Result result = collide_object->collides_with(object);
					if (result.is_colliding())
					{
						glm::vec3 new_movement = glm::vec3(object->get_attached_transform()->get_movement()[0], object->get_attached_transform()->get_movement()[1], object->get_attached_transform()->get_movement()[2]);
						new_movement[0] = 0;
						object->get_attached_transform()->set_movement(new_movement);
						object->get_collisions()->push_back(Collision_Result(object, collide_object));
						collide_object->get_collisions()->push_back(Collision_Result(object, collide_object));
					}
				}
			}
		}
	}
}

// Create a new object into the scene and return it
Physic_Object* Physic_Scene::new_object(std::string name, Transform_Object& transform, bool static_object)
{
	// Create and add the object
	Physic_Object* object = new Physic_Object(get_game_struct(), transform, static_object);
	return object;
}

// Update the Physic_Scene
void Physic_Scene::update()
{
	std::map<std::string, Object*>* objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->second->use_physic())
		{
			it->second->get_attached_physic_object()->update(); // Update each object one by one
		}
	}
	check_collisions();
}

// Physic_Scene destructor
Physic_Scene::~Physic_Scene()
{
	
}

// Scene constructor
Scene::Scene(Advanced_Struct* a_game_struct, std::string a_name, std::string a_map_path, bool a_graphic, bool a_physic, Map_Opening_Mode mode): Transform_Object(), game_struct(a_game_struct), name(a_name), graphic(a_graphic), physic(a_physic)
{
	if (use_graphic())
	{
		graphic_scene = new Graphic_Scene(get_game_struct(), get_name(), objects); // If use graphic, construct a graphic scene
	}

	if (use_physic())
	{
		physic_scene = new Physic_Scene(get_game_struct(), get_name(), objects, objects_map); // If use physic, construct a physic scene
	}

	if (a_map_path != "")
	{
		load_from_file(a_map_path, mode); // Load from a map if necessary
	}
}

// Add an existing object into the scene
void Scene::add_object(std::string name, Object *object)
{
	if (contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to add already exist." << std::endl; return; }
	(*get_objects())[name] = object;
}

// Construct a level from a vector of line
void Scene::construct_level(std::vector<std::string> lines, Map_Level* level, unsigned short level_count)
{
	std::vector<std::vector<unsigned short>> map = std::vector< std::vector<unsigned short>>();
	std::vector<std::vector<bool>> map_horizontal_confirmation = std::vector< std::vector<bool>>();
	std::vector<std::vector<bool>> map_vertical_confirmation = std::vector< std::vector<bool>>();
	for (int j = 0; j < lines.size() - 1; j++) // Fill a temporary generation map
	{
		std::vector<std::string> line = cut_string(lines[j + 1], ";");
		std::vector<bool> map_horizontal_line = std::vector<bool>();
		std::vector<unsigned short> map_line = std::vector<unsigned short>();
		std::vector<bool> map_vertical_line = std::vector<bool>();
		for (int k = 0; k < line.size(); k++)
		{
			map_horizontal_line.push_back(false);
			map_line.push_back(std::stoi(line[k]));
			map_vertical_line.push_back(false);
		}
		map_horizontal_confirmation.push_back(map_horizontal_line);
		map.push_back(map_line);
		map_vertical_confirmation.push_back(map_vertical_line);
	}

	// Search for repeated part scalable, for perormance
	std::vector<Map_Level_Collection> collections = std::vector<Map_Level_Collection>();
	for (int j = 0; j < map.size(); j++) // Browse the map char by char
	{
		for (int k = 0; k < map[j].size() - 1; k++) // Browse the map char by char
		{
			if (!map_horizontal_confirmation[j][k])
			{
				unsigned int part_number = map[j][k];
				if (part_number != 0)
				{
					glm::vec2 end_pos = glm::vec2(j, k);
					glm::vec2 start_pos = glm::vec2(j, k);
					unsigned short iter = 0;
					k++;
					while (k < map[j].size() - 1) // Browse the map in one sense to detect repetition in horizontal axis
					{
						unsigned int other_part_number = map[j][k];
						if (other_part_number != part_number)
						{
							break;
						}
						else
						{
							end_pos = glm::vec2(j, k);
							map_horizontal_confirmation[j][k] = true;
							k++;
						}
						iter++;
					}

					if (iter > 0)
					{
						map_horizontal_confirmation[start_pos[0]][start_pos[1]] = true;
						Map_Level_Collection collection; // Create the collection
						collection.set_base_position(glm::vec3(start_pos[0], 0, start_pos[1]));
						collection.set_final_position(glm::vec3(end_pos[0], 0, end_pos[1]));
						collection.set_orientation(Map_Level_Orientation::Horizontal);
						collection.set_part(part_number);
						collections.push_back(collection);
					}
					k--;
				}
			}
		}
	}

	// Search for repeated part scalable, for perormance
	for (int j = 0; j < map.size(); j++) // Browse the map char by char
	{
		for (int k = 0; k < map[j].size() - 1; k++) // Browse the map char by char
		{
			if (!map_vertical_confirmation[j][k])
			{
				unsigned int part_number = map[j][k];
				if (part_number != 0)
				{
					glm::vec2 end_pos = glm::vec2(j, k);
					glm::vec2 start_pos = glm::vec2(j, k);
					unsigned short iter = 0;
					j++;
					while (j < map.size()) // Browse the map in one sense to detect repetition in vertical axis
					{
						unsigned int other_part_number = map[j][k];
						if (other_part_number != part_number)
						{
							break;
						}
						else
						{
							end_pos = glm::vec2(j, k);
							map_vertical_confirmation[j][k] = true;
							j++;
						}
						iter++;
					}

					if (iter > 0)
					{
						map_vertical_confirmation[start_pos[0]][start_pos[1]] = true;
						Map_Level_Collection collection; // Create the collection
						collection.set_base_position(glm::vec3(start_pos[0], 0, start_pos[1]));
						collection.set_final_position(glm::vec3(end_pos[0], 0, end_pos[1]));
						collection.set_orientation(Map_Level_Orientation::Vertical);
						collection.set_part(part_number);
						collections.push_back(collection);
					}
					j = start_pos[0];
					k = start_pos[1];
				}
			}
		}
	}

	for (int i = 0; i < collections.size(); i++) // Construct each collections
	{
		Map_Level_Collection collection = collections[i];
		unsigned int part_number = collection.get_part();
		Part* part = get_game_struct()->get_part(part_number); // Get the part at the collection
		if (part != 0)
		{
			glm::vec3 difference = collection.get_difference();
			glm::vec3 middle = collection.get_middle();

			glm::vec3 scale = glm::vec3(difference[0] + 1, part->get_scale()[1], difference[2] + 1);

			float x = middle[0] + level->position[0] + part->get_position()[0];
			float y = middle[1] + level->position[1] + part->get_position()[1];
			float z = middle[2] + level->position[2] + part->get_position()[2];

			std::string name = "level" + std::to_string(level->id) + ";w;" + collection.get_name() + ";" + std::to_string(level_count) + ";" + std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);
			
			Object* object = new_object(name, part->get_type(), 0, glm::vec3(x, y, z) + part->get_position(), part->get_rotation(), scale, true, part->get_texture_path()); // Create the object
			object->set_map_pos(glm::vec2(x, z));
			objects_map[level->id][x][z] = object;
		}
	}
	std::cout << get_objects()->size() << std::endl;
}

// Returns if the scene contains an object
bool Scene::contains_object(std::string name)
{
	std::map<std::string, Object*> *objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->first == name) { return true; } // Verify each object name (first element of map)
	}
	return false;
}

// Destroy an object in the scene
void Scene::destroy(std::string name)
{
	if (!contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to destroy does not exist." << std::endl; return; }

	std::map<std::string, Object*>* objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->first == name) // Verify each object name (first element of map)
		{
			for (int i = 0; i < to_destroy.size(); i++) // Check if to_destroy contains the object
			{
				if (to_destroy[i] == it) { return; }
			}
			to_destroy.push_back(it);
			return;
		}
	}
}

// Load the scene from a map
void Scene::load_from_map(std::string map, Map_Opening_Mode mode)
{
	// Reset the scene map
	objects_map.clear();

	if (mode == Map_Opening_Mode::Simple)
	{
		std::vector<std::string> lines = cut_string(map, "\n");

		std::vector<std::string> first_line = cut_string(lines[0], ";");
		unsigned short width = std::stoi(first_line[0]); // Get the size of the map
		unsigned short height = std::stoi(first_line[1]);

		for (int i = 0; i < width; i++) // Full the map with 0
		{
			std::vector<Object*> line = std::vector<Object*>();
			for (int j = 0; j < height; j++)
			{
				line.push_back(0);
			}
			objects_map[0].push_back(line);
		}

		for (int i = 0; i < height; i++)
		{
			std::vector<std::string> line = cut_string(lines[i + 1], ";");
			for (int j = 0; j < width; j++) // Browse the map char by char
			{
				unsigned int part_number = std::stoi(line[j]);
				if (part_number != 0)
				{
					Part* part = get_game_struct()->get_part(part_number); // Get the part at the pos browsed
					if (part != 0)
					{
						float x = j;
						float y = 0;
						float z = i;

						std::string name = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);

						Object* object = new_object(name, part->get_type(), 0, glm::vec3(x, y, z) + part->get_position(), part->get_rotation(), part->get_scale(), true, part->get_texture_path()); // Create the object
						object->set_map_pos(glm::vec2(x, z));
						objects_map[0][x][z] = object;
					}
				}
			}
		}
	}
	else if (mode == Map_Opening_Mode::Complex)
	{
		std::vector<std::string> parts = cut_string(map, map_part_delimitation); // Cut the map by parts

		if (parts.size() <= 1) { std::cout << "Scene \"" << get_name() << "\" : erreur ! The map you want to load is probably corrompted." << std::endl; return; }
		
		std::vector<std::string> lines = cut_string(parts[0], "\n", true); // Check the first part
		if(lines[0] != "l") { std::cout << "Scene \"" << get_name() << "\" : erreur ! The map you want to load is probably corrompted." << std::endl; return; }

		std::map<unsigned short, Map_Level> levels = std::map<unsigned short, Map_Level>();
		std::map<unsigned short, Object*> level_object = std::map<unsigned short, Object*>();
		for (int i = 1; i < lines.size(); i++) // Check each levels of the map
		{
			std::vector<std::string> level_str = cut_string(lines[i], " ");
			unsigned short level_id = std::stoi(level_str[0]);

			unsigned short length = std::stoi(level_str[9]); // Get the size of the level
			unsigned short height = std::stoi(level_str[8]);
			unsigned short width = std::stoi(level_str[7]);

			Map_Level level; // Configurate the level
			level.id = level_id;
			level.scale = glm::vec3(length, height, width);
			levels[level_id] = level;

			std::string object_name = "level" + level_str[0]; // Configurate the level Object
			Transform_Object* level_transform = new Transform_Object(this, level.position, level.rotation, level.scale);
			Object* level_object = new Object(get_game_struct(), object_name, get_name(), level_transform);
			add_object(object_name, level_object);

			objects_map[level_id] = std::vector<std::vector<Object*>>();
			for (int i = 0; i < width; i++) // Full the map with 0
			{
				std::vector<Object*> line = std::vector<Object*>();
				for (int j = 0; j < length; j++)
				{
					line.push_back(0);
				}
				objects_map[level_id].push_back(line);
			}
		}

		for (int i = 1; i < parts.size(); i++)
		{
			std::vector<std::string> lines = cut_string(parts[i], "\n", true);

			std::vector<std::string> level_str = cut_string(lines[0], ";"); // Get the level id of the part
			std::vector<std::string> level_id_str = cut_string(level_str[0], " ");
			unsigned short level_count = std::stoi(level_str[1]);
			unsigned short level_id = std::stoi(level_id_str[1]);
			Map_Level* level = &levels[level_id];

			if (lines[0][0] == 'w')
			{
				construct_level(lines, level, level_count);
			}
			else if (lines[0][0] == 'a')
			{
				for (int j = 0; j < lines.size() - 1; j++) // Browse the map char by char
				{
					std::vector<std::string> line = cut_string(lines[j + 1], ";");
					for (int k = 0; k < lines.size() - 1; k++) // Browse the map char by char
					{
						unsigned int part_number = std::stoi(line[k]);
						if (part_number != 0)
						{
							Part* part = get_game_struct()->get_part(part_number); // Get the part at the pos browsed
							if (part != 0)
							{
								float x = k + level->position[0];
								float y = level->position[1];
								float z = j + level->position[2];

								std::string name = "level" + std::to_string(level_id) + ";f;" + std::to_string(level_count) + ";" + std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);

								Object* object = new_object(name, part->get_type(), 0, glm::vec3(x, y, z) + part->get_position(), part->get_rotation(), part->get_scale(), true, part->get_texture_path()); // Create the object
								object->set_map_pos(glm::vec2(x, z));
								objects_map[level_id][x][z] = object;
							}
						}
					}
				}
			}
		}
	}
}

// Load the scene from a map file
void Scene::load_from_file(std::string map_path, Map_Opening_Mode mode)
{
	std::string map_content;
	std::ifstream map_file;

	// ensure ifstream objects can throw exceptions:
	map_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		map_file.open(map_path);
		std::stringstream map_stream;

		// read file's buffer contents into streams
		map_stream << map_file.rdbuf();

		// close file handlers
		map_file.close();

		// convert stream into string
		map_content = map_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Scene \"" << get_name() << "\" : map file \"" << map_path << "\" unreadable." << std::endl;
	}

	load_from_map(map_content, mode); // Load the map
}

// Update the scene
void Scene::update()
{
	std::map<std::string, Object *> *objects_to_update = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		// Update every animations
		if (it->second->get_attached_transform()->is_animation_playing() && it->second->get_attached_transform()->get_animations()->size() > 0)
		{
			it->second->get_attached_transform()->get_current_animation()->state += get_game_struct()->get_delta_time();
		}
		it->second->update(); // Update every objects
		it->second->get_attached_transform()->update(); // Update every transform objects

		glm::vec2 last_map_pos = it->second->get_last_map_pos();
		glm::vec2 map_pos = it->second->get_map_pos();
		if (map_pos[0] != -1 && map_pos[1] != -1)
		{
			if ((*get_objects_map(it->second->get_map_level()))[map_pos[0]][map_pos[1]] != it->second)
			{
				(*get_objects_map(it->second->get_map_level()))[map_pos[0]][map_pos[1]] = it->second;
				if (last_map_pos[0] != -1 && last_map_pos[1] != -1)
				{
					if ((*get_objects_map(it->second->get_map_level()))[last_map_pos[0]][last_map_pos[1]] == it->second)
					{
						(*get_objects_map(it->second->get_map_level()))[last_map_pos[0]][last_map_pos[1]] = 0;
					}
				}
			}
		}
	}

	if (use_physic())
	{
		get_physic_scene()->update();
	}

	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->late_update(); // Late update every objects
	}

	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->get_attached_transform()->soft_reset(); // Reset every objects
	}

	if (use_graphic()) // Update graphic scene
	{
		get_graphic_scene()->update();
		get_graphic_scene()->render();
	}

	for (int i = 0; i < get_to_destroy()->size(); i++) // Destroyed all needed objects
	{
		std::map<std::string, Object*>::iterator it = (*get_to_destroy())[i];
		if (it->second != 0)
		{
			if (it->second->get_map_pos()[0] != -1) { (*get_objects_map())[it->second->get_map_pos()[0]][it->second->get_map_pos()[1]] = 0; }
			delete it->second;
			it->second = 0;
		}
		objects_to_update->erase(it);
	}
	get_to_destroy()->clear();
}

// Scene destructor
Scene::~Scene()
{
	// Delete the scene
	if (get_graphic_scene() != 0) { delete graphic_scene; graphic_scene = 0; }
	if (get_physic_scene() != 0) { delete physic_scene; physic_scene = 0; }

	// Delete all objects into the scene
	std::map<std::string, Object*>* objects_to_delete = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects_to_delete->begin(); it != objects_to_delete->end(); it++)
	{
		delete it->second;
		it->second = 0;
	}
}