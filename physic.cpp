#include <iostream>
#include "physic.h"

// Collision constructor
Collision::Collision(unsigned short a_type, float a_width, float a_height, float a_length): type(a_type), width(a_width), height(a_height), length(a_length)
{

}

// Collision copy constructor
Collision::Collision(Collision& copy): Collision(copy.type, copy.width, copy.length)
{

}

// Collision destructor
Collision::~Collision()
{

}

// Physic_Object constructor
Physic_Object::Physic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, bool a_static_object, unsigned short type): attached_transform(a_attached_transform), base_struct(a_base_struct), collision(type, 1), static_object(a_static_object)
{

}

// Apply a force to the object
void Physic_Object::apply_force(glm::vec3 force, bool directly_apply)
{
	glm::vec3 new_velocity =  force / glm::vec3(get_mass(), get_mass(), get_mass());
	set_velocity(get_velocity() + new_velocity);
	
	if (directly_apply)
	{
		update_velocity();
	}
}

// Apply the gravity to the object
void Physic_Object::apply_gravity(glm::vec3 gravity_force)
{
	apply_force(gravity_force, true);
}

// Update the physic object
void Physic_Object::update()
{
	update_velocity();
}

// Update the velocity of the object
void Physic_Object::update_velocity()
{
	glm::vec3 move = glm::vec3(get_velocity()[0] * get_base_struct()->get_delta_time(), get_velocity()[1] * get_base_struct()->get_delta_time(), get_velocity()[2] * get_base_struct()->get_delta_time());
	attached_transform.move(move);
}

// Physic_Object destructor
Physic_Object::~Physic_Object()
{

}