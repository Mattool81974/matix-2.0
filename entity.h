#pragma once

#include "base_struct.h"
#include <iostream>

class Entity: public Transform_Object
{
	// Class representing an entity
public:
	Entity(Base_Struct* a_base_struct, Transform_Object* a_parent = 0, glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Entity constructor
	~Entity(); // Entity destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline float get_speed() { return speed; };
	inline bool is_moving() { return moving; };
	void update() {}; // Update the entity
protected:
	bool moving = true; // If the entity can move or not
	float speed = 5.0f; // Speed of the entity
private:
	Base_Struct* base_struct = 0; // Base struct of the game
};

class Player : public Entity
{
	// Class representing a player entity
public:
	Player(Base_Struct* a_base_struct, Transform_Object* a_parent = 0, glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Player constructor
	void update(); // Update the player
	void update_move(); // Update the moving of the player
	~Player(); // Player destructor
private:

};