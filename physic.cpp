#include <iostream>
#include "physic.h"

// Physic_Object constructor
Physic_Object::Physic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform): attached_transform(a_attached_transform), base_struct(a_base_struct)
{

}

// Update the physic object
void Physic_Object::update()
{
	
}

// Physic_Object destructor
Physic_Object::~Physic_Object()
{

}