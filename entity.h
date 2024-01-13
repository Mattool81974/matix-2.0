#pragma once

#include "base_struct.h"

class Entity: public Transform_Object
{
public:
	Entity();
	~Entity();
private:

};

class Player : public Entity
{
public:
	Player();
	~Player();
private:

};