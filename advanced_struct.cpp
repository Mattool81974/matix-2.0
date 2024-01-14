#include "advanced_struct.h"
#include <iostream>

// Advanced_Struct constructor
Advanced_Struct::Advanced_Struct(Base_Struct *a_base_struct): base_struct(a_base_struct)
{
	// Create types
	types["cube"] = "cube";
	types["square"] = "triangle";

	// Create VAOs
	all_vaos["cube"] = new VAO("../shaders/default", "cube");
	all_vaos["triangle"] = new VAO("../shaders/default", "triangle");
}

// Advanced_Struct destructor
Advanced_Struct::~Advanced_Struct()
{
	std::map<std::string, VAO*>* vaos = get_all_vaos();
	for (std::map<std::string, VAO*>::iterator it = vaos->begin(); it != vaos->end(); it++)
	{
		delete it->second;
		it->second = 0;
	}
}