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
	private:

	};

	// Load the lunar environment
	void load_lunar_environment();

	// Setup the lunar environment
	void set_lunar_environment();

	// Run the lunar rover game
	void run_lunar_rover(std::string path);
}