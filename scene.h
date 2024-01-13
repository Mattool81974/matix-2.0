#pragma once

#include <map>
#include "model.h"
#include <string>
#include <vector>

class Graphic_Scene
{
public:
	Graphic_Scene();
	~Graphic_Scene();
private:
};

class Physic_Scene
{
public:
	Physic_Scene();
	~Physic_Scene();
private:
};

class Scene
{
public:
	Scene(std::string a_name, bool a_graphic = true, bool a_physic = true);
	void add_object(std::string name, Transform_Object* object);
	bool contains_object(std::string name);
	Transform_Object *new_object(std::string name);
	void update();
	~Scene();

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

	Graphic_Scene* graphic_scene = 0;
	std::map<std::string, Transform_Object *> objects = std::map<std::string, Transform_Object*>();
	Physic_Scene* physic_scene = 0;
};