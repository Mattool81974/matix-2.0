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
	Physic_Scene(Advanced_Struct* a_game_struct, std::string a_name, std::map<std::string, Object*>& a_objects, std::vector<std::vector<Object*>>& a_objects_map); // Physic_Scene constructor
	void check_collisions(); // Check the collisions in the system
	Physic_Object* new_object(std::string name, Transform_Object& transform, bool static_object = true); // Create a new object into the scene and return it
	void update(); // Update the objects in the scene
	~Physic_Scene(); // Physic_Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_game_struct() { return game_struct; };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Object*>* get_objects() { return &objects; };
	inline std::vector<std::vector<Object*>>* get_objects_map() { return &objects_map; };
private:
	std::string name; // Name of the scene

	Advanced_Struct* game_struct = 0; // Pointer to the Advanced_Struct in the game
	std::map<std::string, Object*>& objects; // Each objects, with their name at key, in the game
	std::vector<std::vector<Object*>> &objects_map; // Each objects, arranged as a map, in the scene
};

class Scene: public Transform_Object
{
	// Class representing a collection of object
public:
	Scene(Advanced_Struct* a_game_struct, std::string a_name, std::string a_map_path = "", bool a_graphic = true, bool a_physic = true); // Scene constructor
	void add_object(std::string name, Object* object); // Add an existing object into the scene
	bool contains_object(std::string name); // Returns if the scene contains an object
	void destroy(std::string name); // Destroy an object in the scene
	void load_from_map(std::string); // Load the scene from a map
	void load_from_file(std::string map_path); // Load the scene from a map file
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
	std::vector<std::vector<Object*>> objects_map = std::vector<std::vector<Object*>>(); // Each objects, arranged as a map, in the scene
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
	}
	O* final_object = new O(get_game_struct(), name, get_name(), object, graphic_object, physic_object);
	add_object(name, final_object);

	return final_object;
}