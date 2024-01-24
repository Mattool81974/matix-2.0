#include <fstream>
#include <iostream>
#include <map>
#include "scene.h"
#include <string>
#include <sstream>
#include <vector>

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
Physic_Scene::Physic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects, std::vector<std::vector<Object*>>& a_objects_map): game_struct(a_game_struct), name(a_name), objects(a_objects), objects_map(a_objects_map)
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
Scene::Scene(Advanced_Struct* a_game_struct, std::string a_name, std::string a_map_path, bool a_graphic, bool a_physic): Transform_Object(), game_struct(a_game_struct), name(a_name), graphic(a_graphic), physic(a_physic)
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
		load_from_file(a_map_path); // Load from a map if necessary
	}
}

// Add an existing object into the scene
void Scene::add_object(std::string name, Object *object)
{
	if (contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to add already exist." << std::endl; return; }
	(*get_objects())[name] = object;
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
void Scene::load_from_map(std::string map)
{
	// Reset the scene map
	objects_map.clear();

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
		objects_map.push_back(line);
	}

	for (int i = 0; i < height; i++)
	{
		std::vector<std::string> line = cut_string(lines[i + 1], ";");
		for (int j = 0; j < width; j++) // Browse the map char by char
		{
			unsigned int part_number = std::stoi(line[j]);
			if (part_number != 0)
			{
				Part *part = get_game_struct()->get_part(part_number); // Get the part at the pos browsed
				if (part != 0)
				{
					float x = j;
					float y = 0;
					float z = i;

					std::string name = std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);

					Object *object = new_object(name, part->get_type(), 0, glm::vec3(x, y, z) + part->get_position(), part->get_rotation(), part->get_scale(), true, part->get_texture_path()); // Create the object
					object->set_map_pos(glm::vec2(x, z));
					objects_map[x][z] = object;
				}
			}
		}
	}
}

// Load the scene from a map file
void Scene::load_from_file(std::string map_path)
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

	load_from_map(map_content); // Load the map
}

// Update the scene
void Scene::update()
{
	std::map<std::string, Object *> *objects_to_update = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->update(); // Update every objects
		it->second->get_attached_transform()->update(); // Update every transform objects

		glm::vec2 last_map_pos = it->second->get_last_map_pos();
		glm::vec2 map_pos = it->second->get_map_pos();
		if (map_pos[0] != -1 && map_pos[1] != -1)
		{
			if ((*get_objects_map())[map_pos[0]][map_pos[1]] != it->second)
			{
				(*get_objects_map())[map_pos[0]][map_pos[1]] = it->second;
				if (last_map_pos[0] != -1 && last_map_pos[1] != -1)
				{
					if ((*get_objects_map())[last_map_pos[0]][last_map_pos[1]] == it->second)
					{
						(*get_objects_map())[last_map_pos[0]][last_map_pos[1]] = 0;
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