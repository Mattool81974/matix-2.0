#pragma once

#include "base_struct.h"

class Entity: public Transform_Object
{
	// Class representing an entity
public:
	Entity(); // Entity constructor
	~Entity(); // Entity destructor
private:

};

class Player : public Entity
{
	// Class representing a player entity
public:
	Player(); // Player constructor
	~Player(); // Player destructor
private:

};