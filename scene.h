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
	Graphic_Scene(Advanced_Struct* a_advanced_struct, std::string a_name); // Graphic_Scene constructor
	void add_object(std::string name, Graphic_Object* object); // Add an existing object into the scene
	bool contains_object(std::string name); // Returns if the scene contains an object
	Graphic_Object* new_object(std::string name, Transform_Object& transform, std::string type, std::string texture_path = "", bool texture_resize = true); // Create a new object into the scene and return it
	void render(); // Render the objects in the scene
	void update(); // Update the objects in the scene
	~Graphic_Scene(); // Graphic_Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Graphic_Object*> *get_objects() { return &objects; };
private:
	std::string name; // Name of the scene

	Advanced_Struct* advanced_struct = 0; // Pointer to the Advanced_Struct in the game
	std::map<std::string, Graphic_Object*> objects = std::map<std::string, Graphic_Object*>(); // Each objects, with their name at key, in the game
};

class Physic_Scene
{
	// Class representing a collection of physic object
public:
	Physic_Scene(Advanced_Struct* a_advanced_struct, std::string a_name); // Physic_Scene constructor
	~Physic_Scene(); // Physic_Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Physic_Object*> *get_objects() { return &objects; };
private:
	std::string name; // Name of the scene

	Advanced_Struct* advanced_struct = 0; // Pointer to the Advanced_Struct in the game
	std::map<std::string, Physic_Object*> objects = std::map<std::string, Physic_Object*>(); // Each objects, with their name at key, in the game
};

class Scene: public Transform_Object
{
	// Class representing a collection of object
public:
	Scene(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_map_path = "", bool a_graphic = true, bool a_physic = true); // Scene constructor
	void add_object(std::string name, Transform_Object* object); // Add an existing object into the scene
	bool contains_object(std::string name); // Returns if the scene contains an object
	void load_from_map(std::string); // Load the scene from a map
	void load_from_file(std::string map_path); // Load the scene from a map file
	Transform_Object *new_object(std::string name, std::string type, Transform_Object* parent = 0, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), std::string texture_path = "", bool texture_resize = true, bool use_graphic_object = true, bool use_physic_object = true); // Create a new object into the scene and return it
	void update(); // Update the scene
	~Scene(); // Scene destructor

	// Getters and setters
	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline Graphic_Scene* get_graphic_scene() { return graphic_scene; };
	inline std::string get_name() { return name; };
	inline Transform_Object* get_object(std::string name) { if (!contains_object(name)) { std::cout << "Scene \"" << get_name() << "\": error : The object \"" << name << "\" you want to access does not exist." << std::endl; return 0; } return (*get_objects())[name]; };
	inline std::map<std::string, Transform_Object*> *get_objects() { return &objects; };
	inline Physic_Scene* get_physic_scene() { return physic_scene; };
	inline bool use_graphic() { return graphic; };
	inline bool use_physic() { return physic; };
private:
	bool graphic; // If the scene use graphic
	std::string name; // Name of the scene
	bool physic; // If the scene use physic

	Advanced_Struct* advanced_struct = 0; // Pointer to the Advanced_Struct in the game
	Graphic_Scene* graphic_scene = 0; // Pointer to the graphic scene
	std::map<std::string, Transform_Object *> objects = std::map<std::string, Transform_Object*>(); // Each objects, with their name at key, in the scene
	Physic_Scene* physic_scene = 0; // Pointer to the physic scene
};