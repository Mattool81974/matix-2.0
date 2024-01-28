#pragma once

#include "advanced_struct.h"
#include "entity.h"
#include "graphic.h"
#include <iostream>
#include <map>
#include "model.h"
#include "physic.h"
#include <string>
#include <vector>

struct Map_Level {
	// Struct representing a level of a map
	unsigned short id = 0;
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
};
enum Map_Level_Orientation {Vertical, Horizontal}; // Differents orientations for a map lev collection
class Map_Level_Collection {
	// Class representing a collection of cutted part in level of a map
public:
	Map_Level_Collection(); // Map_Level_Collection constructor
	Map_Level_Collection(const Map_Level_Collection& copy); // Map_Level_Collection copy constructor
	~Map_Level_Collection(); // Map_Level_Collection destructor

	// Getters and setters
	inline unsigned short get_part() { return part; };
	inline glm::vec3 get_base_position() { return base_position; };
	inline glm::vec3 get_difference() { return get_final_position() - get_base_position(); };
	inline glm::vec3 get_final_position() { return final_position; };
	inline glm::vec3 get_middle() { return get_base_position() + get_difference() / glm::vec3(2, 2, 2); };
	inline std::string get_name() { return get_orientation_name()[get_orientation()]; };
	inline Map_Level_Orientation get_orientation() { return orientation; };
	static std::map<Map_Level_Orientation, std::string> get_orientation_name() { std::map<Map_Level_Orientation, std::string> orientation_name = std::map<Map_Level_Orientation, std::string>(); orientation_name[Map_Level_Orientation::Horizontal] = "horizontal"; orientation_name[Map_Level_Orientation::Vertical] = "vertical"; return orientation_name; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline void set_part(unsigned short a_part) { part = a_part; };
	inline void set_base_position(glm::vec3 a_base_position) { base_position = a_base_position; };
	inline void set_final_position(glm::vec3 a_final_rotation) { final_position = a_final_rotation; };
	inline void set_orientation(Map_Level_Orientation a_orientation) { orientation = a_orientation; };
	inline void set_rotation(glm::vec3 a_rotation) { rotation = a_rotation; };
	inline void set_scale(glm::vec3 a_scale) { scale = a_scale; };
private:
	unsigned short part = 0;
	glm::vec3 base_position = glm::vec3(0, 0, 0);
	glm::vec3 final_position = glm::vec3(0, 0, 0);
	Map_Level_Orientation orientation = Map_Level_Orientation::Vertical;
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
};
enum Map_Opening_Mode { Simple, Complex }; // Opening mode for the map

class Graphic_Scene
{
	// Class representing a collection of graphic object
public:
	Graphic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects); // Graphic_Scene constructor
	Graphic_Object* new_object(std::string name, Transform_Object& transform, std::string type, std::string texture_path = "", bool texture_resize = true); // Create a new object into the scene and return it
	void render(); // Render the objects in the scene
	void update(); // Update the objects in the scene
	~Graphic_Scene(); // Graphic_Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_game_struct() { return game_struct; };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Object*>* get_objects() { return &objects; };
private:
	std::string name; // Name of the scene

	Advanced_Struct* game_struct = 0; // Pointer to the Advanced_Struct in the game
	std::map<std::string, Object*>& objects; // Each objects, with their name at key, in the game
};

class Physic_Scene
{
	// Class representing a collection of physic object
public:
	Physic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects, std::map<unsigned short, std::vector<std::vector<Object*>>>& a_objects_map); // Physic_Scene constructor
	void check_collisions(); // Check the collisions in the system
	Physic_Object* new_object(std::string name, Transform_Object& transform, bool static_object = true); // Create a new object into the scene and return it
	void update(); // Update the objects in the scene
	~Physic_Scene(); // Physic_Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_game_struct() { return game_struct; };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Object*>* get_objects() { return &objects; };
	inline std::vector<std::vector<Object*>>* get_objects_map(unsigned short level = 0) { return &objects_map[level]; };
private:
	std::string name; // Name of the scene

	Advanced_Struct* game_struct = 0; // Pointer to the Advanced_Struct in the game
	std::map<std::string, Object*>& objects; // Each objects, with their name at key, in the game
	std::map<unsigned short, std::vector<std::vector<Object*>>> &objects_map; // Each objects, arranged as a map, in the scene
};

class Scene: public Transform_Object
{
	// Class representing a collection of object
public:
	std::string map_part_delimitation = "<----------------------------------------------->";

	Scene(Advanced_Struct* a_game_struct, std::string a_name, std::string a_map_path = "", bool a_graphic = true, bool a_physic = true, Map_Opening_Mode mode = Map_Opening_Mode::Simple); // Scene constructor
	void add_object(std::string name, Object* object); // Add an existing object into the scene
	void construct_level(std::vector<std::string> lines, Map_Level *level, unsigned short level_count); // Construct a level from a vector of line
	bool contains_object(std::string name); // Returns if the scene contains an object
	void destroy(std::string name); // Destroy an object in the scene
	void load_from_map(std::string, Map_Opening_Mode mode = Map_Opening_Mode::Simple); // Load the scene from a map
	void load_from_file(std::string map_path, Map_Opening_Mode mode = Map_Opening_Mode::Simple); // Load the scene from a map file
	template <class O = Object> // Template for adding a type of object
	O *new_object(std::string name, std::string type, Transform_Object* parent = 0, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), bool static_object = true, std::string texture_path = "", bool texture_resize = true, bool use_graphic_object = true, bool use_physic_object = true); // Create a new object into the scene and return it
	void update(); // Update the scene
	~Scene(); // Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_game_struct() { return game_struct; };
	inline Graphic_Scene* get_graphic_scene() { return graphic_scene; };
	inline std::string get_name() { return name; };
	inline Object* get_object(std::string name) { if (!contains_object(name)) { std::cout << "Scene \"" << get_name() << "\": error : The object \"" << name << "\" you want to access does not exist." << std::endl; return 0; } return (*get_objects())[name]; };
	inline std::map<std::string, Object*> *get_objects() { return &objects; };
	inline std::vector<std::vector<Object*>>* get_objects_map(unsigned short level = 0) { return &objects_map[level]; };
	inline Physic_Scene* get_physic_scene() { return physic_scene; };
	inline std::vector<std::map<std::string, Object*>::iterator>* get_to_destroy() { return &to_destroy; };
	inline bool use_graphic() { return graphic; };
	inline bool use_physic() { return physic; };
private:
	bool graphic; // If the scene use graphic
	std::string name; // Name of the scene
	bool physic; // If the scene use physic

	Advanced_Struct* game_struct = 0; // Pointer to the Advanced_Struct in the game
	Graphic_Scene* graphic_scene = 0; // Pointer to the graphic scene
	std::map<std::string, Object *> objects = std::map<std::string, Object*>(); // Each objects, with their name at key, in the scene
	std::map<unsigned short, std::vector<std::vector<Object*>>> objects_map = std::map<unsigned short, std::vector<std::vector<Object*>>>(); // Each objects, arranged as a map, in the scene
	Physic_Scene* physic_scene = 0; // Pointer to the physic scene
	std::vector<std::map<std::string, Object*>::iterator> to_destroy = std::vector<std::map<std::string, Object*>::iterator>(); // Name of the objects to destroy at the end of the frame
};

// Create a new object into the scene and return it
template <class O> // Template for adding a type of object
O* Scene::new_object(std::string name, std::string type, Transform_Object* parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool static_object, std::string texture_path, bool texture_resize, bool use_graphic_object, bool use_physic_object)
{
	if (contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to create already exist." << std::endl; return 0; }

	// Configure object
	if (parent == 0) { parent = this; }

	// Create and add the object
	Transform_Object* object = 0;
	if (type == "player")
	{
		object = new Player(get_game_struct(), parent, position, rotation, scale);
	}
	else
	{
		object = new Transform_Object(parent, position, rotation, scale);
	}

	// Create the object in graphic scene
	Graphic_Object* graphic_object = 0;
	if (use_graphic() and use_graphic_object)
	{
		graphic_object = get_graphic_scene()->new_object(name, *object, type, texture_path, texture_resize);
	}

	// Create the object in physic scene
	Physic_Object* physic_object = 0;
	if (use_physic() and use_physic_object)
	{
		physic_object = get_physic_scene()->new_object(name, *object, static_object);
		physic_object->get_collision()->set_height(scale[1]);
	}
	O* final_object = new O(get_game_struct(), name, get_name(), object, graphic_object, physic_object);
	add_object(name, final_object);

	return final_object;
}