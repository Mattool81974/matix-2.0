#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> cut_string(std::string string, std::string cut)
{
	std::string last_string = "";
	std::string last_string_cut = "";
	std::vector<std::string> result = std::vector<std::string>();
	for (int i = 0; i < string.size(); i++)
	{
		last_string_cut += string[i];
		if (last_string_cut.size() > cut.size())
		{
			last_string_cut = last_string_cut.substr(1, cut.size());
		}

		if (last_string_cut == cut)
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

	if (last_string.size() > 0) { result.push_back(last_string); }
	return result;
}

// Transform_Object contructor
Transform_Object::Transform_Object(Transform_Object *a_parent, glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale) : parent(a_parent), position(a_position), rotation(a_rotation), scale(a_scale)
{
	calculate_direction();
}

// Transform_Object copy constructor
Transform_Object::Transform_Object(const Transform_Object& copy): Transform_Object(copy.parent, copy.position, copy.rotation, copy.scale)
{

}

// Calculate the direction vector of the transform object
void Transform_Object::calculate_direction()
{
	float x = cos(glm::radians(get_rotation()[1])) * (cos(glm::radians(get_rotation()[0])));
	float y = sin(glm::radians(get_rotation()[0]));
	float z = sin(glm::radians(get_rotation()[1])) * (cos(glm::radians(get_rotation()[0])));

	forward = glm::normalize(glm::vec3(x, y, z));
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
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
	return glm::lookAt(get_position(), get_position() + get_forward(), get_up());
}

// Camera destructor
Camera::~Camera()
{

}

// Base_Struct constructor
Base_Struct::Base_Struct(double& a_mouse_x, double& a_mouse_y): mouse_x(a_mouse_x), mouse_y(a_mouse_y), last_mouse_x(a_mouse_x), last_mouse_y(a_mouse_y)
{

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