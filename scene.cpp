#include <iostream>
#include <map>
#include "scene.h"
#include <string>
#include <vector>

// Graphic_Scene constructor
Graphic_Scene::Graphic_Scene(Advanced_Struct* a_advanced_struct, std::string a_name): advanced_struct(a_advanced_struct), name(a_name)
{

}

// Add an existing object into the scene
void Graphic_Scene::add_object(std::string name, Graphic_Object* object)
{
	if (contains_object(name)) { std::cout << "Graphic scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to add already exist." << std::endl; return; }
	(*get_objects())[name] = object;
}

// Returns if the scene contains an object
bool Graphic_Scene::contains_object(std::string name)
{
	std::map<std::string, Graphic_Object*>* objects = get_objects();
	for (std::map<std::string, Graphic_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->first == name) { return true; } // Verify each scene name (first element of map)
	}
	return false;
}

// Create a new object into the scene and return it
Graphic_Object* Graphic_Scene::new_object(std::string name, Transform_Object &transform, std::string type)
{
	if (contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to create already exist." << std::endl; return 0; }
	
	// Create and add the object
	Graphic_Object* object = new Graphic_Object(get_base_struct(), transform, (*get_advanced_struct()->get_all_vaos())[(*get_advanced_struct()->get_type())[type]]);
	add_object(name, object);

	return object;
}

// Render the objects in the scene
void Graphic_Scene::render()
{
	std::map<std::string, Graphic_Object*>* objects = get_objects();
	for (std::map<std::string, Graphic_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		it->second->render();
	}
}

// Update the objects in the scene
void Graphic_Scene::update()
{
	std::map<std::string, Graphic_Object*>* objects = get_objects();
	for (std::map<std::string, Graphic_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		it->second->update();
	}
}

// Graphic_Scene destructor
Graphic_Scene::~Graphic_Scene()
{
	std::map<std::string, Graphic_Object*>* objects = get_objects();
	for (std::map<std::string, Graphic_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		delete it->second;
		it->second = 0;
	}
}

// Physic_Scene constructor
Physic_Scene::Physic_Scene(Advanced_Struct* a_advanced_struct, std::string a_name): advanced_struct(a_advanced_struct), name(a_name)
{

}

// Physic_Scene destructor
Physic_Scene::~Physic_Scene()
{

}

// Scene constructor
Scene::Scene(Advanced_Struct* a_advanced_struct, std::string a_name, bool a_graphic, bool a_physic): Transform_Object(), advanced_struct(a_advanced_struct), name(a_name), graphic(a_graphic), physic(a_physic)
{
	if (use_graphic())
	{
		graphic_scene = new Graphic_Scene(get_advanced_struct(), get_name());
	}

	if (use_physic())
	{
		physic_scene = new Physic_Scene(get_advanced_struct(), get_name());
	}
}

// Add an existing object into the scene
void Scene::add_object(std::string name, Transform_Object *object)
{
	if (contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to add already exist." << std::endl; return; }
	(*get_objects())[name] = object;
}

// Returns if the scene contains an object
bool Scene::contains_object(std::string name)
{
	std::map<std::string, Transform_Object*> *objects = get_objects();
	for (std::map<std::string, Transform_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		if (it->first == name) { return true; } // Verify each scene name (first element of map)
	}
	return false;
}

// Create a new object into the scene and return it
Transform_Object* Scene::new_object(std::string name, std::string type, Transform_Object *parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	if (contains_object(name)) { std::cout << "Scene \"" << get_name() << "\" : error ! The object \"" << name << "\" you want to create already exist." << std::endl; return 0; }

	// Configure object
	if (parent == 0) { parent = this; }
	
	// Create and add the object
	Transform_Object* object = new Transform_Object(parent, position, rotation, scale);
	add_object(name, object);

	// Create the object in graphic scene
	if (use_graphic())
	{
		get_graphic_scene()->new_object(name, *object, type);
	}

	return object;
}

// Update the scene
void Scene::update()
{
	std::map<std::string, Transform_Object *> *objects_to_update = get_objects();
	for (std::map<std::string, Transform_Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->update(); // Update every objects
	}

	if (use_graphic()) // Update graphic scene
	{
		get_graphic_scene()->update();
		get_graphic_scene()->render();
	}

	for (std::map<std::string, Transform_Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		it->second->soft_reset(); // Reset every objects
	}
}

// Scene destructor
Scene::~Scene()
{
	// Delete the scene
	if (get_graphic_scene() != 0) { delete graphic_scene; graphic_scene = 0; }
	if (get_physic_scene() != 0) { delete physic_scene; physic_scene = 0; }

	// Delete all objects into the scene
	std::map<std::string, Transform_Object*>* objects_to_update = get_objects();
	for (std::map<std::string, Transform_Object*>::iterator it = objects_to_update->begin(); it != objects_to_update->end(); it++)
	{
		delete it->second;
		it->second = 0;
	}
}