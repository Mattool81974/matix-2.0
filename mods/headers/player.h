/*
*
* This class represents the "Player" mod.
* It allows better camera handle.
*
*/

#pragma once

#include "../../headers/game.h"

namespace Player
{
	class Camera_Handler : public Object
	{
	public:
		Camera_Handler(Advanced_Struct* advanced_struct = 0, std::string a_name = "", std::string scene_name = "", Transform_Object* attached_transform = 0, Graphic_Object* attached_graphic = 0, Physic_Object* attached_physic = 0); // Camera_Handler constructor
		void* clone(Advanced_Struct* game_struct, std::string name, std::string scene_name, Transform_Object* attached_transform, Graphic_Object* attached_graphic = 0, Physic_Object* attached_physic = 0); // Clone the Camera_Handler
		void late_update(); // Late update the Camera_Handler
		void update_move(); // Update the move in the Camera_Handler
		~Camera_Handler(); // Camera_Handler destructor

		// Getters and setters
		inline bool can_move() { return a_can_move; };
		inline bool can_rotate() { return a_can_rotate; };
		inline Camera* get_camera() { return a_camera; };
		inline float get_speed() { return a_speed; };
		inline void set_can_move(bool new_can_move) { a_can_move = new_can_move; };
		inline void set_can_rotate(bool new_can_rotate) { a_can_rotate = new_can_rotate; };
	private:
		// If the player can move or not
		bool a_can_move = true;

		// If the player can rotate or not
		bool a_can_rotate = true;

		// Speed of the camera handler
		float a_speed = 2.5f;

		// Camera in the game
		Camera* a_camera = 0;

		// Game
		Game* a_game = 0;
	};
}