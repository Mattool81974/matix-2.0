#include <iostream>
#include "physic.h"

// Physic_Object constructor
Physic_Object::Physic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform): attached_transform(a_attached_transform), base_struct(a_base_struct)
{

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