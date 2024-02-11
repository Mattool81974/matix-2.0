/*
* 
* This class represents the "Lunar rover" mod.
* 
*/



#pragma once

#include "../../headers/game.h"

namespace Lunar_Rover
{
	class Rover : public Object
	{
		/*
		* Class representing the lunar rover
		*/
	public:
		Rover(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0);; // Rover constructor
		void create(); // Create the rover part by part
		void deploy(bool instant = false); // Deploy the rover
		void forward(float multiplier = 1); // Move the rover forward
		void set_back_view(); // Set the back view for the camera
		void set_head_view(); // Set the ehad view for the camera
		void turn_head(float multiplier = 1); // Turn the head of the rover
		void undeploy(bool instant = false); // Undeploy the rover
		void update(); // Update the rover

		// Getters and setters
		inline glm::vec3 get_camera_back_view_offset() { return camera_back_view_offset; };
		inline glm::vec3 get_camera_head_view_offset() { return camera_head_view_offset; };
		inline unsigned short get_wheels_number() { return wheels_number; };
		inline bool is_deployed() { return deployed; };
		inline void set_camera_back_view_offset(glm::vec3 new_camera_back_view_offset) { camera_back_view_offset = new_camera_back_view_offset; };
	private:
		bool deployed = true; // If the rover is deployed or not
		float turn_head_speed = 180; // Speed of turning of the head
		unsigned short wheels_number = 4; // Number of width in the rover

		glm::vec3 camera_back_view_offset = glm::vec3(-7.5, 2, 0); // Offset of the camera for the back view
		glm::vec3 camera_head_view_offset = glm::vec3(0.5, 0, 0); // Offset of the camera for the back view
		Object* head = 0; // Head of the rover
		Object* neck = 0; // Neck of the rover
		Object* support = 0; // Support of the rover
		std::vector<Object*> wheels = std::vector<Object*>(); // Wheels of the rover
		std::vector<Object*> wheels_base = std::vector<Object*>(); // Base of the wheels of the rover
		std::vector<Object*> wheels_support = std::vector<Object*>(); // Support of the wheels of the rover
	};

	// Load the lunar environment
	void load_lunar_environment();

	// Setup the lunar environment
	void set_lunar_environment();

	// Run the lunar rover game
	void run_lunar_rover(std::string path);
}