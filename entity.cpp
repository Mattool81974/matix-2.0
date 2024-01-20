#include "base_struct.h"
#include "entity.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Entity constructor
Entity::Entity(Base_Struct *a_base_struct, Transform_Object* a_parent, glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale): Transform_Object(a_parent, a_position, a_rotation, a_scale), base_struct(a_base_struct)
{

}

// Entity destructor
Entity::~Entity()
{

}

// Player constructor
Player::Player(Base_Struct* a_base_struct, Transform_Object* a_parent, glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale) : Entity(a_base_struct, a_parent, a_position, a_rotation, a_scale)
{

}

// Update the moving of the player
void Player::update_move()
{
	float delta_time = get_base_struct()->get_delta_time();
	float speed = get_speed();
	speed *= delta_time;

	// Rotate and move camera
	float rotate_speed = 45;
	float sensitivity = get_base_struct()->get_camera()->get_sensitivity();
	rotate(glm::vec3(0.0, sensitivity * delta_time * get_base_struct()->get_mouse_move_x(), 0.0));
	get_base_struct()->get_camera()->rotate(glm::vec3(-sensitivity * get_base_struct()->get_delta_time() * get_base_struct()->get_mouse_move_y(), 0.0, 0.0));

	// Move the player
	if (get_base_struct()->get_key_state("z") == 1)
		move(glm::vec3(speed) * get_forward());
	if (get_base_struct()->get_key_state("s") == 1)
		move(glm::vec3(speed) * -get_forward());
	if (get_base_struct()->get_key_state("q") == 1)
		move(glm::vec3(speed) * get_right());
	if (get_base_struct()->get_key_state("d") == 1)
		move(glm::vec3(speed) * -get_right());
	if (get_base_struct()->get_key_state("space") == 1)
		move(glm::vec3(speed) * get_up());
	if (get_base_struct()->get_key_state("left shift") == 1)
		move(glm::vec3(speed) * -get_up());
}

// Update the player
void Player::update()
{
	update_move();
}

// Player destructor
Player::~Player()
{

}