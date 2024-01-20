#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

// Cut a string where there are the "cut"
std::vector<std::string> cut_string(std::string string, std::string cut)
{
	std::string last_string = ""; // String since the last cut
	std::string last_string_cut = ""; // String of the "cut" size which allows to know where to cut
	std::vector<std::string> result = std::vector<std::string>();
	for (int i = 0; i < string.size(); i++) // Browse the string char by char
	{
		last_string_cut += string[i];
		if (last_string_cut.size() > cut.size()) // If the string which allows to know where to cut is too long, cut him
		{
			last_string_cut = last_string_cut.substr(1, cut.size());
		}

		if (last_string_cut == cut) // If the string which allows to know where to cut is equal to the part to cut, do a cut
		{
			result.push_back(last_string);
			last_string = "";
			last_string_cut = "";
		}
		else
		{
			last_string += string[i];
		}
	}

	if (last_string.size() > 0) { result.push_back(last_string); } // Add the last non-cutted element
	return result;
}

// Transform_Object contructor
Transform_Object::Transform_Object(Transform_Object *a_parent, glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale) : parent(0), position(a_position), rotation(a_rotation), scale(a_scale)
{
	set_parent(a_parent);
	calculate_direction();
}

// Transform_Object copy constructor
Transform_Object::Transform_Object(const Transform_Object& copy): Transform_Object(copy.parent, copy.position, copy.rotation, copy.scale)
{

}

// Calculate the direction vector of the transform object
void Transform_Object::calculate_direction()
{
	float x = cos(glm::radians(get_rotation()[1])) * (cos(glm::radians(get_rotation()[0]))); // Use trigonometry to calculate forward direction
	float y = sin(glm::radians(get_rotation()[0]));
	float z = sin(glm::radians(get_rotation()[1])) * (cos(glm::radians(get_rotation()[0])));

	forward = glm::normalize(glm::vec3(x, y, z));
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward)); // Use vector calcul to calculate right and up direction
	up = glm::normalize(glm::cross(forward, right));
}

// Return the transformation matrix of the object
glm::mat4 Transform_Object::get_model_matrix()
{
	glm::mat4 matrix = glm::mat4(1.0f);

	// Move matrix
	matrix = glm::translate(matrix, get_absolute_position());

	// Rotate matrix
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[0]), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[1]), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[2]), glm::vec3(0, 0, 1));

	// Scale matrix
	matrix = glm::scale(matrix, get_scale());

	return matrix;
}

// Move the object
void Transform_Object::move(glm::vec3 a_movement)
{
	movement = get_movement() + a_movement;
}

// Remove an object from the children
void Transform_Object::remove_child(Transform_Object* object)
{
	std::vector<Transform_Object*>* children = get_children();
	for (std::vector<Transform_Object*>::iterator it = children->begin(); it != children->end(); it++)
	{
		if ((*it) == object) { children->erase(it); break; } // Verify each child
	}
}

// Scale the object
void Transform_Object::rescale(glm::vec3 a_scale)
{
	set_scale(glm::vec3(get_scale()[0] * a_scale[0], get_scale()[1] * a_scale[1], get_scale()[2] * a_scale[2]));
}

// Rotate the object
void Transform_Object::rotate(glm::vec3 a_rotation)
{
	rotation += a_rotation;
	calculate_direction();
	std::vector<Transform_Object*>* children = get_children();
	for (int i = 0; i < children->size(); i++)
	{
		Transform_Object* child = (*children)[i];
		a_rotation = glm::vec3(a_rotation[0] * child->get_parent_rotation_multiplier(), a_rotation[1] * child->get_parent_rotation_multiplier(), a_rotation[2] * child->get_parent_rotation_multiplier());
		child->rotate(a_rotation);
		child->rotate_around(child->get_position(), get_rotation());
	}
}

// Rotate the object around a point
void Transform_Object::rotate_around(glm::vec3 a_position, glm::vec3 a_rotation)
{
	// Calculate the angle in a local XZ circle
	glm::vec2 difference_position = glm::vec2(a_position[0], a_position[2]);
	glm::vec2 difference_normalized = difference_position;
	if (!(difference_position[0] == 0 and difference_position[1] == 0))
	{
		// Calculate the angle of the position
		difference_normalized = glm::normalize(difference_position);
		float difference_multiplier_x = difference_position[0] / difference_normalized[0];
		float difference_multiplier_z = difference_position[1] / difference_normalized[1];
	
		float angle = glm::asin(difference_normalized[1] / glm::distance(glm::vec2(0, 0), difference_position));

		// Calculate the position in the local circle
		float final_angle = angle + glm::radians(a_rotation[1]);
		glm::vec2 final_position = glm::vec2(glm::cos(final_angle) * difference_multiplier_x, glm::sin(final_angle) * difference_multiplier_z);

		// Calculate the final position
		position_offset = glm::vec3(final_position[0], 0, final_position[1]) - glm::vec3(difference_position[0], 0, difference_position[1]);
	}
}

// Reset softly the object
void Transform_Object::soft_reset()
{
	set_position(get_position() + get_movement());
	set_movement(glm::vec3(0.0, 0.0, 0.0));
}

// Transform_Object destructor
Transform_Object::~Transform_Object()
{

}

// Camera constructor
Camera::Camera(glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale): Transform_Object(0, a_position, a_rotation, a_scale)
{

}

// Return the projection matrix
glm::mat4 Camera::get_projection(int window_height, int window_width)
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), float(window_width) / float(window_height), 0.1f, 100.0f);

	return projection;
}

// Return the view matrix
glm::mat4 Camera::get_view()
{
	return glm::lookAt(get_absolute_position(), get_absolute_position() + get_forward(), get_up());
}

// Camera destructor
Camera::~Camera()
{

}

// Base_Struct constructor
Base_Struct::Base_Struct(double& a_mouse_x, double& a_mouse_y): mouse_x(a_mouse_x), mouse_y(a_mouse_y), last_mouse_x(a_mouse_x), last_mouse_y(a_mouse_y)
{
	(*get_keys_state())["z"] = 0;
	(*get_keys_state())["s"] = 0;
	(*get_keys_state())["q"] = 0;
	(*get_keys_state())["d"] = 0;
	(*get_keys_state())["space"] = 0;
	(*get_keys_state())["left shift"] = 0;
}

// Return the projection matrix
glm::mat4 Base_Struct::get_projection()
{
	return camera.get_projection(get_window_height(), get_window_width());
}

// Base_Struct destructor
Base_Struct::~Base_Struct()
{

}