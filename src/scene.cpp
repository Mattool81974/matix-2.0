#include "../headers/scene.h"

// Map_Level_Collection constructor
Map_Level_Collection::Map_Level_Collection()
{

}

// Map_Level_Collection copy constructor
Map_Level_Collection::Map_Level_Collection(const Map_Level_Collection& copy) : Map_Level_Collection()
{
	level = copy.level;
	level_count = copy.level_count;
	part = copy.part;
	base_position = copy.base_position;
	final_position = copy.final_position;
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
Physic_Scene::Physic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects, std::vector<std::vector<std::vector<Object*>>>& a_objects_map): game_struct(a_game_struct), name(a_name), objects(a_objects), objects_map(a_objects_map)
{

}

// Apply the gravity to the objects
void Physic_Scene::apply_gravity()
{
	std::map<std::string, Object*>* objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->second->use_physic() && !it->second->get_attached_physic_object()->is_static())
		{
			// Calculate the gravity force
			glm::vec3 gravity = get_game_struct()->get_gravity_force() * glm::vec3(get_game_struct()->get_delta_time(), get_game_struct()->get_delta_time(), get_game_struct()->get_delta_time());
			it->second->get_attached_physic_object()->apply_gravity(gravity); // Update each object one by one

			// Check if the object is standing
			std::vector<glm::vec3> map_positions = it->second->get_all_map_pos(glm::vec3(0, 1, 0), true); // Check each part of the map the object is
			Collision_Result result = check_collision(it->second, glm::vec3(0, 2, 0), false);
			if (result.size() > 0 && glm::abs(it->second->get_attached_physic_object()->get_velocity()[1]) < 0.1) it->second->get_attached_physic_object()->set_is_standing(true);
			else it->second->get_attached_physic_object()->set_is_standing(false);
		}
	}
}

// Check the collisions in the system
void Physic_Scene::check_collisions()
{
	std::vector<Object*> dynamic_objects = std::vector<Object*>(); // Get the dynamics objects
	for (std::map<std::string, Object*>::iterator it = get_objects()->begin(); it != get_objects()->end(); it++)
	{
		if (it->second->use_physic() && !it->second->get_attached_physic_object()->is_static() && it->second->get_attached_physic_object()->use_collision())
		{
			dynamic_objects.push_back(it->second);
		}
	}

	for (int i = 0; i < dynamic_objects.size(); i++) // Check collision of each objects
	{
		Object* object = dynamic_objects[i];
		(*object->get_collisions_result()) = check_collision(object, glm::vec3(1, 1, 1), true);
	}
}

// Check the collision of an object
Collision_Result Physic_Scene::check_collision(Object* object, glm::vec3 direction, bool apply_to_target)
{
	Collision_Result collision_result = Collision_Result(object);
	std::vector<glm::vec3> map_positions = object->get_all_map_pos(direction, true); // Check each part of the map the object is
	for (int j = 0; j < map_positions.size() / 2; j++)
	{
		glm::vec3 position = map_positions[j * 2 + 1];
		if (position[0] >= 0 && position[0] < get_objects_map()->size())
		{
			float x = position[0];
			if (position[1] >= 0 && position[1] < (*get_objects_map())[x].size())
			{
				float y = position[1];
				if (position[2] >= 0 && position[2] < (*get_objects_map())[x][y].size())
				{
					float z = position[2];
					Object* target = (*get_objects_map())[x][y][z];
					if (target != 0 && object != target)
					{
						One_Collision result = target->collides_with(object);
						if (result.collide) // If the object collides with the target
						{
							int side = glm::floor((3.0f / map_positions.size()) * j);
							result.axis_multiplier = map_positions[j * 2];

							collision_result.add_collision(result);
							if(apply_to_target) target->get_collisions_result()->add_collision(result);
						}
					}
				}
			}
		}
	}
	return collision_result;
}

// Update before everything
void Physic_Scene::early_update()
{
	
}

// Create a new object into the scene and return it
Physic_Object* Physic_Scene::new_object(std::string name, Transform_Object& transform, bool static_object)
{
	// Create and add the object
	Physic_Object* object = new Physic_Object(get_game_struct(), transform, static_object);
	object->get_collision()->set_width(transform.get_scale()[0]);
	object->get_collision()->set_height(transform.get_scale()[1]);
	object->get_collision()->set_length(transform.get_scale()[2]);
	return object;
}

// Update the Physic_Scene
void Physic_Scene::update()
{
	std::map<std::string, Object*>* objects = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->second->use_physic() && !it->second->get_attached_physic_object()->is_static())
		{
			it->second->get_attached_physic_object()->update(); // Update each object one by one
		}
	}
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

// Return the collections in the scene to string to debug
std::string Scene::collections_to_string()
{
	std::vector<Map_Level_Collection>* collections = get_collections();
	std::string result = "";

	for (int i = 0; i < collections->size(); i++)
	{
		result += (*collections)[i].to_string() + "\n";
	}

	return result.substr(0, result.size() - 1);
}

// Construct the collections of a level from a vector of line
std::vector<Map_Level_Collection> Scene::construct_collections(std::vector<std::string> lines, Map_Level* level, unsigned short level_count)
{
	std::vector<std::vector<unsigned short>> map = std::vector< std::vector<unsigned short>>();
	std::vector<std::vector<bool>> map_horizontal_confirmation = std::vector< std::vector<bool>>();
	std::vector<std::vector<bool>> map_confirmation = std::vector< std::vector<bool>>();
	std::vector<std::vector<bool>> map_vertical_confirmation = std::vector< std::vector<bool>>();
	for (int j = 0; j < lines.size() - 1; j++) // Fill a temporary generation map
	{
		std::vector<std::string> line = cut_string(lines[j + 1], ";");
		std::vector<bool> map_horizontal_line = std::vector<bool>();
		std::vector<unsigned short> map_line = std::vector<unsigned short>();
		std::vector<bool> map_confirmation_line = std::vector<bool>();
		std::vector<bool> map_vertical_line = std::vector<bool>();
		for (int k = 0; k < line.size(); k++)
		{
			map_horizontal_line.push_back(false);
			map_line.push_back(std::stoi(line[k]));
			map_confirmation_line.push_back(false);
			map_vertical_line.push_back(false);
		}
		map_horizontal_confirmation.push_back(map_horizontal_line);
		map.push_back(map_line);
		map_confirmation.push_back(map_confirmation_line);
		map_vertical_confirmation.push_back(map_vertical_line);
	}

	// Search for repeated part scalable, for performance
	std::vector<Map_Level_Collection> collections = std::vector<Map_Level_Collection>();
	for (int j = 0; j < map.size(); j++) // Browse the map char by char
	{
		for (int k = 0; k < map[j].size(); k++) // Browse the map char by char
		{
			if (!map_confirmation[j][k])
			{
				unsigned int part_number = map[j][k];
				if (part_number != 0)
				{
					if (get_game_struct()->get_part(part_number)->use_collection())
					{
						glm::vec2 end_pos = glm::vec2(j, k);
						glm::vec2 start_pos = glm::vec2(j, k);
						unsigned short iter = 0;
						while (j < map.size() && k < map[j].size()) // Browse the map in two sense to detect repetition in horizontal/vertical axis
						{
							bool line_good_1 = true;

							if (k < map[j].size() - 1)
							{
								for (int l = j; l >= start_pos[0]; l--) // Check if every part is good or not
								{
									unsigned int other_part_number = map[l][k + 1];
									if (other_part_number != part_number)
									{
										end_pos[1] = k;
										line_good_1 = false;
										break;
									}
									else
									{
										end_pos[1] = k + 1;
									}
								}

								if (line_good_1)
								{
									iter++;
									k++;
									for (int l = j; l >= start_pos[0]; l--) // Check if every part is good or not
									{
										map_confirmation[l][k] = true;
									}
								}
							}
							else
							{
								line_good_1 = false;
							}

							bool line_good_2 = true;

							if (j < map.size() - 1)
							{
								for (int l = k; l >= start_pos[1]; l--) // Check if every part is good or not
								{
									unsigned int other_part_number = map[j + 1][l];
									if (other_part_number != part_number)
									{
										end_pos[0] = j;
										line_good_2 = false;
										break;
									}
									else
									{
										end_pos[0] = j + 1;
									}
								}

								if (line_good_2)
								{
									iter++;
									j++;
									for (int l = k; l >= start_pos[1]; l--) // Check if every part is good or not
									{
										map_confirmation[j][l] = true;
									}
								}
							}
							else
							{
								line_good_2 = false;
							}

							if (!line_good_1 && !line_good_2)
							{
								break;
							}
						}

						if (iter > 0)
						{
							map_confirmation[start_pos[0]][start_pos[1]] = true;
							Map_Level_Collection collection = Map_Level_Collection(); // Create the collection
							collection.set_base_position(glm::vec3(start_pos[0], 0, start_pos[1]));
							collection.set_final_position(glm::vec3(end_pos[0], 0, end_pos[1]));
							collection.set_level(level);
							collection.set_level_count(level_count);
							collection.set_part(part_number);
							collections.push_back(collection);
						}
						j = start_pos[0];
						k = start_pos[1];
					}
					else
					{
						map_confirmation[j][k] = true;
						Map_Level_Collection collection = Map_Level_Collection(); // Create the collection
						collection.set_base_position(glm::vec3(j, 0, k));
						collection.set_final_position(glm::vec3(j, 0, k));
						collection.set_level(level);
						collection.set_level_count(level_count);
						collection.set_part(part_number);
						collections.push_back(collection);
					}
				}
			}
		}
	}

	return collections;
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

// Load the scene
void Scene::load()
{
	for (std::map<std::string, Object*>::iterator it = get_objects()->begin(); it != get_objects()->end(); it++) // After release each objects
	{
		it->second->after_loading();
	}
}

// Load the scene from a vector of collections
void Scene::load_from_collection(std::vector<Map_Level_Collection> collections)
{
	for (int i = 0; i < collections.size(); i++) // Construct each collections
	{
		Map_Level_Collection collection = collections[i];
		unsigned int part_number = collection.get_part();
		Part* part = get_game_struct()->get_part(part_number); // Get the part at the collection
		if (part != 0)
		{
			glm::vec3 difference = collection.get_difference();
			glm::vec3 middle = collection.get_middle();

			float x = middle[0] + collection.get_level()->sub_level_position[collection.get_level_count()][0] + part->get_position()[0];
			float y = middle[1] + collection.get_level()->sub_level_position[collection.get_level_count()][1] + part->get_position()[1];
			float z = middle[2] + collection.get_level()->sub_level_position[collection.get_level_count()][2] + part->get_position()[2];

			glm::vec3 scale = glm::vec3(part->get_scale()[0] * (difference[0] + 1), part->get_scale()[1] * (difference[1] + 1), part->get_scale()[2] * (difference[2] + 1));
			if (part->get_scale_level_multiplier()[0] != 0)
			{
				scale[0] *= collection.get_level()->sub_level_scale[collection.get_level_count()][0] * part->get_scale_level_multiplier()[0];
				x += (collection.get_level()->sub_level_scale[collection.get_level_count()][0] * part->get_scale_level_multiplier()[0]) / 2;
			}
			if (part->get_scale_level_multiplier()[1] != 0)
			{
				scale[1] *= collection.get_level()->sub_level_scale[collection.get_level_count()][1] * part->get_scale_level_multiplier()[1];
				y += (collection.get_level()->sub_level_scale[collection.get_level_count()][1] * part->get_scale_level_multiplier()[1]) / 2;
			}
			if (part->get_scale_level_multiplier()[2] != 0)
			{
				scale[2] *= collection.get_level()->sub_level_scale[collection.get_level_count()][2] * part->get_scale_level_multiplier()[2];
				z += (collection.get_level()->sub_level_scale[collection.get_level_count()][2] * part->get_scale_level_multiplier()[2]) / 2;
			}

			std::string name = "level" + std::to_string(collection.get_level()->id) + ";w;" + collection.get_name() + ";" + std::to_string(collection.get_level_count()) + ";" + std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z);

			Object* object = new_object(name, part->get_type(), collection.get_level()->map_level_object->get_attached_transform(), glm::vec3(x, y, z), part->get_rotation(), scale, true, part->get_texture_path(), part->get_resize_texture(), true, true, part->get_base_object()); // Create the object
			assign_map_pos(object->set_map_pos(glm::vec3(x, y, z)), object);
			object->set_description(part->get_description());
		}
	}
}

// Load the scene from a map
void Scene::load_from_map(std::string map, Map_Opening_Mode mode)
{
	// Reset the scene map

	if (mode == Map_Opening_Mode::Simple) // SImple opening mode
	{
		std::vector<std::string> lines = cut_string(map, "\n");

		std::vector<std::string> first_line = cut_string(lines[0], ";");
		unsigned short width = std::stoi(first_line[0]); // Get the size of the map
		unsigned short height = std::stoi(first_line[1]);

		clear_objects_map(width, 1, height);

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
						object->set_map_pos(glm::vec3(x, y, z));
						objects_map[0][x][z] = object;
					}
				}
			}
		}
	}
	else if (mode == Map_Opening_Mode::Complex) // Complex opening mode
	{
		std::vector<std::string> parts = cut_string(map, map_part_delimitation); // Cut the map by parts

		if (parts.size() <= 1) { std::cout << "Scene \"" << get_name() << "\" : erreur ! The map you want to load is probably corrompted." << std::endl; return; }
		
		std::vector<std::string> lines = cut_string(parts[0], "\n", true); // Check the first part
		if(lines[0] != "l") { std::cout << "Scene \"" << get_name() << "\" : erreur ! The map you want to load is probably corrompted." << std::endl; return; }

		levels = std::map<unsigned short, Map_Level>();
		std::map<unsigned short, Object*> level_object = std::map<unsigned short, Object*>();
		for (int i = 1; i < lines.size(); i++) // Check each levels of the map
		{
			Map_Level level; // Create the level
			std::vector<std::string> level_count = cut_string(lines[i], ",");
			std::vector<std::string> level_specifications = cut_string(level_count[0], " ");
			unsigned short level_id = std::stoi(level_specifications[0]); // Get and set ID
			level.id = level_id;

			for (int j = 1; j < level_count.size(); j++) // Browse each level part
			{
				std::vector<std::string> level_str = cut_string(level_count[j], " ");
				unsigned short level_id = std::stoi(level_str[0]);

				float x = string_to_float(level_str[1]); // Get the position of the level
				float y = string_to_float(level_str[2]);
				float z = string_to_float(level_str[3]);

				float yaw = string_to_float(level_str[5]); // Get the position of the level
				float pitch = string_to_float(level_str[4]);
				float roll = string_to_float(level_str[6]);

				unsigned short length = std::stoi(level_str[9]); // Get the size of the level
				unsigned short height = std::stoi(level_str[8]);
				unsigned short width = std::stoi(level_str[7]);

				level.sub_level_position.push_back(glm::vec3(x, y, z));
				level.sub_level_rotation.push_back(glm::vec3(yaw, pitch, roll));
				level.sub_level_scale.push_back(glm::vec3(length, height, width));
			}

			// Get the level specifications
			float x = string_to_float(level_specifications[1]);
			float y = string_to_float(level_specifications[2]);
			float z = string_to_float(level_specifications[3]);

			float yaw = string_to_float(level_specifications[5]);
			float pitch = string_to_float(level_specifications[4]);
			float roll = string_to_float(level_specifications[6]);

			unsigned short length = std::stoi(level_specifications[9]);
			unsigned short height = std::stoi(level_specifications[8]);
			unsigned short width = std::stoi(level_specifications[7]);

			// Set the level specifications
			level.position = glm::vec3(x, y, z);
			level.rotation = glm::vec3(pitch, yaw, roll);
			level.scale = glm::vec3(width, height, length);

			std::string object_name = "level" + level_specifications[0]; // Configurate the level Object
			Transform_Object* level_transform = new Transform_Object(this, level.position, level.rotation, level.scale);
			Object* level_object = new Object(get_game_struct(), object_name, get_name(), level_transform);
			add_object(object_name, level_object);

			level.map_level_object = level_object;

			levels[level_id] = level; // Add the level
		}

		total_size = glm::vec3(60, 10, 60);

		clear_objects_map();
		collections.clear();

		for (int i = 1; i < parts.size(); i++)
		{
			std::vector<std::string> lines = cut_string(parts[i], "\n", true);

			char part_type = lines[0][0];
			if (part_type == 'p') { continue; }

			std::vector<std::string> level_str = cut_string(lines[0], ";"); // Get the level id of the part
			std::vector<std::string> level_id_str = cut_string(level_str[0], " ");
			unsigned short level_id = std::stoi(level_id_str[1]);
			Map_Level* level = &levels[level_id];

			if (part_type == 'w') // Load each level count
			{
				unsigned short level_count = std::stoi(level_str[1]);

				std::vector<Map_Level_Collection> all = construct_collections(lines, level, level_count);
				for (int i = 0; i < all.size(); i++) // Construct each collections
				{
					collections.push_back(all[i]);
				}
				load_from_collection(all);
			}
			else if (part_type == 'c') // Load the collections in a level
			{
				std::vector<Map_Level_Collection> all = std::vector<Map_Level_Collection>();

				for (int j = 1; j < lines.size(); j++) // Construct each collections
				{
					std::vector<std::string> cutted = cut_string(lines[j], ";");

					Map_Level_Collection collection = Map_Level_Collection();
					collection.set_base_position(glm::vec3(string_to_float(cutted[2]), string_to_float(cutted[3]), string_to_float(cutted[4])));
					collection.set_final_position(glm::vec3(string_to_float(cutted[5]), string_to_float(cutted[6]), string_to_float(cutted[7])));
					collection.set_level(level);
					collection.set_level_count(std::stoi(cutted[1]));
					collection.set_part(std::stoi(cutted[0]));
					all.push_back(collection);
				}

				for (int i = 0; i < all.size(); i++)
				{
					collections.push_back(all[i]);
				}
				load_from_collection(all);
			}
		}
	}
}

// Load the scene from a map file
void Scene::load_from_file(std::string map_path, Map_Opening_Mode mode)
{
	load_from_map(read_file(map_path), mode); // Load the map
}

// Return the objects map to string to debug
std::string Scene::objects_map_to_string(int y)
{
	std::string result = "";
	for (int i = 0; i < objects_map.size(); i++)
	{
		std::string line = "";
		for (int j = 0; j < objects_map[i][y].size(); j++)
		{
			if (objects_map[i][y][j] == 0)
			{
				line += "0";
			}
			else
			{
				line += objects_map[i][y][j]->get_description();
			}
			line += ";";
		}
		result += line.substr(0, line.size() - 1) + "\n";
	}
	return result;
}

// Update the scene
void Scene::update()
{
	clear_objects_map();

	if (use_physic() && get_physic_scene() != 0)
	{
		get_physic_scene()->early_update();
	}

	std::map<std::string, Object *> *objects_to_update = get_objects();
	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		if (it->second->use_physic() && it->second->get_attached_physic_object()->is_static())
		{
			assign_map_pos(it->second->get_all_map_pos(), it->second);
		}

		// Update every animations
		if (it->second->get_attached_transform()->is_animation_playing() && it->second->get_attached_transform()->get_animations()->size() > 0)
		{
			it->second->get_attached_transform()->get_current_animation()->state += get_game_struct()->get_delta_time();
		}
		it->second->update(); // Update every objects
		it->second->get_attached_transform()->update(); // Update every transform objects
	}

	if (use_physic() && get_physic_scene() != 0)
	{
		get_physic_scene()->apply_gravity();
	}

	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->late_update(); // Late update every objects
	}

	if (use_physic() && get_physic_scene() != 0)
	{
		get_physic_scene()->update();
		get_physic_scene()->check_collisions();
	}

	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->last_update(); // Last update every objects
	}

	for (std::map<std::string, Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->get_attached_transform()->soft_reset(); // Reset every objects
		if(it->second->can_reset_map_pos())it->second->set_map_pos(it->second->get_attached_transform()->get_absolute_position());
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