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
		void deploy_arm(bool instant = false); // Deploy the arm of the rover
		void deploy_head(bool instant = false); // Deploy the head of the rover
		void forward(float multiplier = 1); // Move the rover forward
		void turn_arm(glm::vec3 rotation = glm::vec3(1, 0, 0)); // Move the arm forward
		void set_back_view(); // Set the back view for the camera
		void set_head_view(); // Set the head view for the camera
		void set_right_view(); // Set the right view for the camera
		void turn_head(float multiplier = 1); // Turn the head of the rover
		void undeploy_arm(bool instant = false); // Deploy the arm of the rover
		void undeploy_head(bool instant = false); // Undeploy the rover
		void update(); // Update the head of the rover

		// Getters and setters
		inline std::vector<Object*>* get_arm_parts() { return &arm_parts; };
		inline glm::vec3 get_camera_back_view_offset() { return camera_back_view_offset; };
		inline glm::vec3 get_camera_back_view_rotation_offset() { return camera_back_view_rotation_offset; };
		inline glm::vec3 get_camera_head_view_offset() { return camera_head_view_offset; };
		inline glm::vec3 get_camera_head_view_rotation_offset() { return camera_head_view_rotation_offset; };
		inline glm::vec3 get_camera_right_view_offset() { return camera_right_view_offset; };
		inline glm::vec3 get_camera_right_view_rotation_offset() { return camera_right_view_rotation_offset; };
		inline unsigned short get_wheels_number() { return wheels_number; };
		inline bool is_head_deployed() { return head_deployed; };
		inline void set_camera_back_view_offset(glm::vec3 new_camera_back_view_offset) { camera_back_view_offset = new_camera_back_view_offset; };
	private:
		bool head_deployed = true; // If the rover is deployed or not
		float turn_head_speed = 180; // Speed of turning of the head
		unsigned short wheels_number = 4; // Number of width in the rover

		std::vector<Object*> arm_parts = std::vector<Object*>(); // Parts of the arm of the rover
		glm::vec3 camera_back_view_offset = glm::vec3(-7.5, 2, 0); // Offset of the camera for the back view
		glm::vec3 camera_back_view_rotation_offset = glm::vec3(0.0, 0.0, 0.0); // Rotation offset of the camera for the back view
		glm::vec3 camera_head_view_offset = glm::vec3(0.5, 0, 0); // Offset of the camera for the back view
		glm::vec3 camera_head_view_rotation_offset = glm::vec3(0.0, 0.0, 0.0); // Rotation offset of the camera for the back view
		glm::vec3 camera_right_view_offset = glm::vec3(0.0, 2, 8.0); // Offset of the camera for the right view
		glm::vec3 camera_right_view_rotation_offset = glm::vec3(0.0, 270.0, 0.0); // Rotation offset of the camera for the back view
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