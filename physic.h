#pragma once

#include "base_struct.h"

class Collision
{
	// Class representing a physic collision
public:
	Collision(unsigned short a_type = 1, float a_width = 1, float a_height = 1); // Collision constructor
	Collision(Collision& copy); // Collision copy constructor
	~Collision(); // Collision destructor

	// Getters and setters
	inline float get_height() { return height; };
	inline unsigned short get_type() { return type; };
	inline float get_width() { return width; };
	inline void set_height(float a_height) { height = a_height; };
	inline void set_width(float a_width) { width = a_width; };
private:
	float height; // Height of the collision
	unsigned short type; // 1 = square
	float width; // Width of the collision
};

class Physic_Object
{
	// Class representing a physic object
public:
	Physic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, bool a_static_object = true, unsigned short type = 1); // Physic_Object constructor
	void update(); // Update the physic object
	void update_velocity(); // Update the velocity of the object
	~Physic_Object(); // Physic_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline Collision* get_collision() { return &collision; };
	inline glm::vec3 get_velocity() { return velocity; };
	inline bool is_static() { return static_object; };
	inline void set_velocity(glm::vec3 a_velocity) { velocity = a_velocity; };
	inline bool use_collision() { return is_using_collision; };
private:
	float mass = 1; // Mass of the object
	bool static_object = true; // If the object is static or not
	bool is_using_collision = true; // If the object use the collisions or not

	Transform_Object& attached_transform; // Reference to the attached transform object
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	Collision collision; // Collision of the object
	glm::vec3 velocity = glm::vec3(0, 0, 0); // Velocity of the object
};