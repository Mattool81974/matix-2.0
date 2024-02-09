#pragma once

#include "../headers/base_struct.h"

class Collision
{
	// Class representing a physic collision
public:
	Collision(unsigned short a_type = 1, float a_width = 1, float a_height = 1, float a_length = 1); // Collision constructor
	Collision(Collision& copy); // Collision copy constructor
	~Collision(); // Collision destructor

	// Getters and setters
	inline float get_height() { return height; };
	inline float get_length() { return length; };
	inline unsigned short get_type() { return type; };
	inline float get_width() { return width; };
	inline void set_height(float a_height) { height = a_height; };
	inline void set_length(float a_length) { length = a_length; };
	inline void set_width(float a_width) { width = a_width; };
private:
	float height; // Height of the collision
	float length; // Length of the collision
	unsigned short type; // 1 = square
	float width; // Width of the collision
};

class Physic_Object
{
	// Class representing a physic object
public:
	Physic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, bool a_static_object = true, unsigned short type = 1); // Physic_Object constructor
	void apply_force(glm::vec3 force, bool directly_apply = false); // Apply a force to the object
	void apply_gravity(glm::vec3 gravity_force); // Apply the gravity to the object
	void update(); // Update the physic object
	void update_velocity(); // Update the velocity of the object
	~Physic_Object(); // Physic_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline Collision* get_collision() { return &collision; };
	inline float get_elasticity() { return elasticity; };
	inline glm::vec3 get_gravity_value() { return gravity_value; };
	inline float get_mass() { return mass; };
	inline glm::vec3 get_velocity() { return velocity; };
	inline bool is_falling() { return fall; };
	inline bool is_standing() { return stand; };
	inline bool is_static() { return static_object; };
	inline void set_elasticity(float new_elasticity) { elasticity = new_elasticity; };
	inline void set_gravity_value(glm::vec3 a_gravity_value) { gravity_value = a_gravity_value; };
	inline void set_is_falling(bool is_falling) { fall = is_falling; };
	inline void set_is_standing(bool is_standing) { stand = is_standing; };
	inline void set_mass(float a_mass) { mass = a_mass; };
	inline void set_use_collision(bool a_use_collision) { is_using_collision = a_use_collision; };
	inline void set_velocity(glm::vec3 a_velocity) { velocity = a_velocity; };
	inline bool use_collision() { return is_using_collision; };
private:
	float elasticity = 1.0f; // Elasticity of the object
	float mass = 1; // Mass of the object
	bool static_object = true; // If the object is static or not
	bool fall = false; // If the object fall or not
	bool is_using_collision = true; // If the object use the collisions or not
	bool stand = false; // If the object is standing here

	Transform_Object& attached_transform; // Reference to the attached transform object
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	Collision collision; // Collision of the object
	glm::vec3 gravity_value = glm::vec3(0, 1, 0); // Multiplier for the gravity force
	glm::vec3 velocity = glm::vec3(0, 0, 0); // Velocity of the object
};