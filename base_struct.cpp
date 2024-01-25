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

// Normalize a rotation and return it
glm::vec3 normalize_rotation(glm::vec3 rotation)
{
	while (rotation[0] >= 360)
	{
		rotation[0] -= 360;
	}
	while (rotation[0] < 0)
	{
		rotation[0] += 360;
	}

	while (rotation[1] >= 360)
	{
		rotation[1] -= 360;
	}
	while (rotation[1] < 0)
	{
		rotation[1] += 360;
	}

	while (rotation[2] >= 360)
	{
		rotation[2] -= 360;
	}
	while (rotation[2] < 0)
	{
		rotation[2] += 360;
	}

	return rotation;
}

// Transform_Object contructor
Transform_Object::Transform_Object(Transform_Object *a_parent, glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale) : parent(0), position(a_position), rotation(a_rotation), scale(a_scale)
{
	position_offset = a_position;
	set_parent(a_parent);
	calculate_direction();
}

// Transform_Object copy constructor
Transform_Object::Transform_Object(const Transform_Object& copy): Transform_Object(copy.parent, copy.position, copy.rotation, copy.scale)
{

}

// Add an animation to the object
void Transform_Object::add_animation(float duration, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale)
{
	add_animation(duration, get_position(), get_rotation(), get_scale(), final_position, final_rotation, final_scale);
}

// Add an animation to the object with the base position
void Transform_Object::add_animation(float duration, glm::vec3 base_position, glm::vec3 base_rotation, glm::vec3 base_scale, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale)
{
	Transform_Animation animation;
	animation.base_position = base_position;
	animation.base_rotation = base_rotation;
	animation.base_scale = base_scale;
	animation.duration = duration;
	animation.final_position = final_position;
	animation.final_rotation = final_rotation;
	animation.final_scale = final_scale;
	(*get_animations()).push_back(animation);
}

// Add an animation to the object with the position
void Transform_Object::add_position_animation(float duration, glm::vec3 base_position, glm::vec3 final_position)
{
	Transform_Animation animation;
	animation.base_position = base_position;
	animation.duration = duration;
	animation.final_position = final_position;
	animation.modify_rotation = false;
	animation.modify_rotation = false;
	animation.modify_scale = false;
	(*get_animations()).push_back(animation);
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
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[1]), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[0]), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[2]), glm::vec3(0, 0, 1));

	// Scale matrix
	matrix = glm::scale(matrix, get_scale());

	return matrix;
}

// Move the object
void Transform_Object::move(glm::vec3 a_movement)
{
	movement = get_movement() + a_movement * get_position_move_multiplier();
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
		a_rotation *= child->get_parent_rotation_multiplier();
		child->rotate(a_rotation);
		child->rotate_around(child->get_anchored_position(), get_rotation(), glm::vec3(1, 1, 0));
	}
}

// Rotate the object around a point with euler angle
void Transform_Object::rotate_around(glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 rotation_multiplier)
{
	position_offset = -get_anchored_position() + get_position();

	// Calculate the angle in a local XZ circle with Y angle
	glm::vec2 difference_position = glm::vec2(a_position[0], a_position[2]);
	if (!(difference_position[0] == 0 and difference_position[1] == 0) and rotation_multiplier[1] == 1)
	{
		// Calculate the angle of the position
		float opposite = difference_position[0] - get_position()[0];
		float adjacent = difference_position[1] - get_position()[2];
		float hypothenus = glm::distance(difference_position, glm::vec2(0, 0));
		glm::vec2 normalized = glm::normalize(glm::vec2(opposite, adjacent));
	
		float angle = glm::atan(opposite / adjacent);

		// Calculate the position in the local circle
		float final_angle = angle + glm::radians(a_rotation[1]);
		glm::vec2 final_position = -glm::vec2(glm::cos(final_angle) * hypothenus, glm::sin(final_angle) * hypothenus);

		// Calculate the final position
		position_offset[0] = final_position[0];
		position_offset[2] = final_position[1];
	}

	// Calculate the angle in a local YZ circle with X angle
	if (!(a_position[0] == 0 and a_position[2] == 0) and rotation_multiplier[0] == 1)
	{
		// Calculate the angle of the position
		float adjacent = glm::distance(difference_position, glm::vec2(0, 0));
		float hypothenus = glm::distance(glm::vec3(difference_position[0], a_position[1] - get_position()[1], difference_position[1]), glm::vec3(0, 0, 0));
		glm::vec2 opposite_normalized = glm::normalize(difference_position);

		float angle = glm::acos(adjacent / hypothenus);
		if (a_position[1] - get_position()[1] < 0)
		{
			angle = 3.1415 * 2 - glm::acos(adjacent / hypothenus);
		}

		// Calculate the position in the local circle
		float final_angle = angle + glm::radians(a_rotation[0]);

		// Calculate the final position
		position_offset[1] = glm::sin(final_angle) * hypothenus;
		position_offset[0] *= -glm::cos(final_angle);
		position_offset[2] *= -glm::cos(final_angle);
	}
}

// Reset softly the object
void Transform_Object::soft_reset()
{
	set_position(get_position() + get_movement());
	set_movement(glm::vec3(0.0, 0.0, 0.0));
}

// Update the animation for this transform object
void Transform_Object::update_animation()
{
	if (is_animation_playing() && get_animations()->size() > 0)
	{
		Transform_Animation* animation = get_current_animation();

		// Calculate new transform
		float animation_purcentate = animation->state / animation->duration;
		if (animation_purcentate < 1)
		{
			if (animation->modify_position) // Position modification
			{
				glm::vec3 position_difference = animation->final_position - animation->base_position;
				glm::vec3 new_position = animation->base_position + position_difference * animation_purcentate;
				set_position(new_position);
			}

			if (animation->modify_rotation) // Rotation modification
			{
				glm::vec3 rotation_difference = animation->final_rotation - animation->base_rotation;
				glm::vec3 new_rotation = animation->base_rotation + rotation_difference * animation_purcentate;
				set_rotation(new_rotation);
			}

			if (animation->modify_scale) // Scale modification
			{
				glm::vec3 scale_difference = animation->final_scale - animation->base_scale;
				glm::vec3 new_scale = animation->base_scale + scale_difference * animation_purcentate;
				set_scale(new_scale);
			}
		}
		else
		{
			// Apply transform
			if (animation->modify_position) set_position(animation->final_position);
			if (animation->modify_rotation) set_rotation(animation->final_rotation);
			if (animation->modify_scale) set_scale(animation->final_scale);

			// Delete the finished element
			std::cout << "Bis " << get_animations()->size() << std::endl;
			get_animations()->pop_back();
			std::cout << "Ter " << get_animations()->size() << std::endl;
		}
	}
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
	projection = glm::perspective(glm::radians(get_fov()), float(window_width) / float(window_height), 0.1f, 100.0f);

	return projection;
}

// Return the view matrix
glm::mat4 Camera::get_view()
{
	return glm::lookAt(get_absolute_position(), get_absolute_position() + get_forward(), get_up());
}

// Rotate the object
void Camera::rotate(glm::vec3 a_rotation)
{
	glm::vec3 rotation = a_rotation;
	glm::vec3 final_rotation = normalize_rotation(get_rotation() + a_rotation);
	if (final_rotation[0] > 89 and final_rotation[0] < 271) // Resize the position if necessary
	{
		if(rotation[0] > 0)
			rotation[0] = 89 - normalize_rotation(get_rotation())[0];
		else
			rotation[0] = 271 - normalize_rotation(get_rotation())[0];
	}
	Transform_Object::rotate(rotation);
}

// Camera destructor
Camera::~Camera()
{

}

// Base_Struct constructor
Base_Struct::Base_Struct(double& a_mouse_x, double& a_mouse_y): mouse_x(a_mouse_x), mouse_y(a_mouse_y), last_mouse_x(a_mouse_x), last_mouse_y(a_mouse_y)
{
	(*get_keys_state())["a"] = 0;
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