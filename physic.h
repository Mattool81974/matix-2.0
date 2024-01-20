#pragma once

#include "base_struct.h"

class Physic_Object
{
	// Class representing a physic object
public:
	Physic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform); // Physic_Object constructor
	void update(); // Update the physic object
	~Physic_Object(); // Physic_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
private:
	float mass = 1; // Mass of the object

	Transform_Object& attached_transform; // Reference to the attached transform object
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
};