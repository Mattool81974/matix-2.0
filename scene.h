#pragma once

#include "advanced_struct.h"
#include "graphic.h"
#include <map>
#include "model.h"
#include "physic.h"
#include <string>
#include <vector>

class Graphic_Scene
{
	// Class representing a collection of graphic object
public:
	Graphic_Scene(Advanced_Struct* a_advanced_struct, std::string a_name);
	void add_object(std::string name, Graphic_Object* object);
	bool contains_object(std::string name);
	Graphic_Object* new_object(std::string name, Transform_Object& transform, std::string type, std::string texture_path = "");
	void render();
	void update();
	~Graphic_Scene();

	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Graphic_Object*> *get_objects() { return &objects; };
private:
	std::string name;

	Advanced_Struct* advanced_struct = 0;
	std::map<std::string, Graphic_Object*> objects = std::map<std::string, Graphic_Object*>();
};

class Physic_Scene
{
	// Class representing a collection of physic object
public:
	Physic_Scene(Advanced_Struct* a_advanced_struct, std::string a_name);
	~Physic_Scene();

	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Physic_Object*> *get_objects() { return &objects; };
private:
	std::string name;

	Advanced_Struct* advanced_struct = 0;
	std::map<std::string, Physic_Object*> objects = std::map<std::string, Physic_Object*>();
};

class Scene: public Transform_Object
{
	// Class representing a collection of object
public:
	Scene(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_map_path = "", bool a_graphic = true, bool a_physic = true);
	void add_object(std::string name, Transform_Object* object);
	bool contains_object(std::string name);
	void load_from_map(std::string);
	void load_from_file(std::string map_path);
	Transform_Object *new_object(std::string name, std::string type, Transform_Object* parent = 0, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), std::string texture_path = "");
	void update();
	~Scene();

	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline Graphic_Scene* get_graphic_scene() { return graphic_scene; };
	inline std::string get_name() { return name; };
	inline std::map<std::string, Transform_Object*> *get_objects() { return &objects; };
	inline Physic_Scene* get_physic_scene() { return physic_scene; };
	inline bool use_graphic() { return graphic; };
	inline bool use_physic() { return physic; };
private:
	bool graphic;
	std::string name;
	bool physic;

	Advanced_Struct* advanced_struct = 0;
	Graphic_Scene* graphic_scene = 0;
	std::map<std::string, Transform_Object *> objects = std::map<std::string, Transform_Object*>();
	Physic_Scene* physic_scene = 0;
};