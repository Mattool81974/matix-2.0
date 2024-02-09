#include "../headers/base_struct.h"

// To avoid a bug with std::codecvt_utf8<char>, go in the project setting > C/C++ > General > SDL Check and disable it

std::string float_character = "";
std::string non_float_character = "";

// Cut a string where there are the "cut"
std::vector<std::string> cut_string(std::string string, std::string cut, bool erase_blank)
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
			std::string final_string = last_string.substr(0, last_string.size() - (cut.size() - 1));
			if (erase_blank)
			{
				if (final_string != "")
				{
					result.push_back(final_string);
				}
			}
			else
			{
				result.push_back(final_string);
			}
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

// Cut a wstring where there are the "cut"
std::vector<std::wstring> cut_string(std::wstring string, std::wstring cut, bool erase_blank)
{
	std::wstring last_string; // String since the last cut
	std::wstring last_string_cut; // String of the "cut" size which allows to know where to cut
	std::vector<std::wstring> result = std::vector<std::wstring>();
	for (int i = 0; i < string.size(); i++) // Browse the string char by char
	{
		last_string_cut += string[i];
		if (last_string_cut.size() > cut.size()) // If the string which allows to know where to cut is too long, cut him
		{
			last_string_cut = last_string_cut.substr(1, cut.size());
		}

		if (last_string_cut == cut) // If the string which allows to know where to cut is equal to the part to cut, do a cut
		{
			std::wstring final_string = last_string.substr(0, last_string.size() - (cut.size() - 1));
			if (erase_blank)
			{
				if (!final_string.empty())
				{
					result.push_back(final_string);
				}
			}
			else
			{
				result.push_back(final_string);
			}
			last_string.clear();
			last_string_cut.clear();
		}
		else
		{
			last_string += string[i];
		}
	}

	if (last_string.size() > 0) { result.push_back(last_string); } // Add the last non-cutted element
	return result;
}

// Return the content of a directory
std::vector<std::string> directory_content(std::string path)
{
	std::vector<std::string> result = std::vector<std::string>();

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string sub_file = entry.path().string();

		std::cout << sub_file << std::endl;

		result.push_back(sub_file);
	}

	return result;
}

// Return the datas about a file
struct stat file_datas(std::string path)
{
	struct stat sb;

	bool result = (stat(path.c_str(), &sb) == 0);

	return sb;
}

// Returns if a file exists
bool file_exists(std::string path)
{
	struct stat sb;

	bool result = (stat(path.c_str(), &sb) == 0);

	return result;
}

// Returns if a path is a directory or not
bool path_is_directory(std::string path)
{
	return ((file_datas(path).st_mode & S_IFDIR) == S_IFDIR);
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

// Return the file
std::string read_file(std::string path, File_Type type)
{
	std::locale current_locale = std::locale("");

	std::string file_content;
	if (type == File_Type::Text)
	{
		std::ifstream file;

		// ensure ifstream objects can throw exceptions:
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<char>));

			// open files
			file.open(path);
			std::stringstream stream;

			// read file's buffer contents into streams
			stream << file.rdbuf();

			// close file handlers
			file.close();

			// convert stream into string
			file_content = stream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Matrix game : map file \"" << path << "\" unreadable." << std::endl;
		}
	}

	// setlocale(LC_ALL, current_locale.name().c_str());

	return file_content;
}

// Replace a string in an another string
std::string replace(std::string str, std::string to_replace, std::string new_str)
{
	std::vector<std::string> part = cut_string(str, to_replace);
	std::string final = "";

	for (int i = 0; i < part.size(); i++) { final += part[i] + new_str; }

	return final.substr(0, final.size() - new_str.size());
}

// Rotate a vector around a rotating point
glm::vec3 rotate_vector(glm::vec3 vector, glm::vec3 rotation, glm::vec3 position, glm::vec3 rotation_multiplier)
{
	glm::vec3 to_return = glm::vec3(0, 0, 0);
	vector -= position;

	// Calculate the angle in a local XZ circle with Y angle
	glm::vec2 difference_position = glm::vec2(vector[0], vector[2]);
	if (!(difference_position[0] == 0 and difference_position[1] == 0) and rotation_multiplier[1] != 0)
	{
		// Calculate the angle of the position
		float opposite = difference_position[0];
		float adjacent = difference_position[1];
		float hypothenus = glm::distance(difference_position, glm::vec2(0, 0));
		glm::vec2 normalized = glm::normalize(glm::vec2(opposite, adjacent));

		float angle = glm::atan(opposite / adjacent);

		// Calculate the position in the local circle
		float final_angle = angle + glm::radians(rotation[1] * rotation_multiplier[1]);
		glm::vec2 final_position = -glm::vec2(glm::cos(final_angle) * hypothenus, glm::sin(final_angle) * hypothenus);

		// Calculate the final position
		to_return[0] = final_position[0];
		to_return[2] = final_position[1];
	}

	// Calculate the angle in a local YZ circle with X angle
	if (!(vector[0] == 0 and vector[2] == 0) and rotation_multiplier[0] == 1)
	{
		// Calculate the angle of the position
		float adjacent = glm::distance(difference_position, glm::vec2(0, 0));
		float hypothenus = glm::distance(glm::vec3(difference_position[0], vector[1] - position[1], difference_position[1]), glm::vec3(0, 0, 0));
		glm::vec2 opposite_normalized = glm::normalize(difference_position);

		float angle = glm::acos(adjacent / hypothenus);
		if (vector[1] - position[1] < 0)
		{
			angle = 3.1415 * 2 - glm::acos(adjacent / hypothenus);
		}

		// Calculate the position in the local circle
		float final_angle = angle + glm::radians(rotation[0]);

		// Calculate the final position
		to_return[1] = glm::sin(final_angle) * hypothenus;
		to_return[0] *= -glm::cos(final_angle);
		to_return[2] *= -glm::cos(final_angle);
	}

	return to_return;
}

// Return the size of a number
float sign(float number) { return number < 0 ? -1 : (number == 0 ? 0 : 1); }

// Convert a string to a float
float string_to_float(std::string str)
{
	if (float_character == "")
	{
		float_character = ".";
		non_float_character = ",";
		if (std::stod("0.25") != 0.25)
		{
			float_character = ",";
			non_float_character = ".";
		}
	}

	return std::stod(replace(str, non_float_character, float_character));
}

// Transform a string to an uppercase string
std::string to_uppercase(std::string str)
{
	std::string result = "";
	for (int i = 0; i < str.length(); i++)
	{
		char chr = str[i];
		int chr_ascii = chr;

		if(chr_ascii >= 97 && chr_ascii <= 122)
		{
			result += chr_ascii - 32;
		}
		else if (chr == ';')
		{
			result = ".";
		}
		else if (chr == ':')
		{
			result = "/";
		}
		else
		{
			result += chr;
		}
	}
	return result;
}

// Write something in a file
void write_in_file(std::string path, std::string to_write, std::ios::openmode opening_mode, File_Type type)
{
	std::ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try
	{
		if (type == File_Type::Text)
		{
			file.open(path, std::ios::out | opening_mode);

			file << to_write; // Write a text into the file

			file.close();
		}
	}
	catch (std::ofstream::failure e)
	{
		std::cout << "Matrix game : map file \"" << path << "\" unwritable." << std::endl;
	}
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

// Add an animation to the object with the  rotation
void Transform_Object::add_rotation_animation(float duration, glm::vec3 base_rotation, glm::vec3 final_rotation)
{
	Transform_Animation animation;
	animation.base_rotation = base_rotation;
	animation.duration = duration;
	animation.final_rotation = final_rotation;
	animation.modify_position = false;
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

// Reset animation in the object
void Transform_Object::reset_animation(bool reset_position)
{
	get_animations()->clear();
	if (reset_position)
	{
		set_position_animation(glm::vec3(0, 0, 0));
	}
}

// Rotate the object
void Transform_Object::rotate(glm::vec3 a_rotation, bool rotate_around)
{
	set_rotation(get_rotation() + a_rotation, glm::vec3(1, 1, 1), rotate_around);
	calculate_direction();
	std::vector<Transform_Object*>* children = get_children();
	for (int i = 0; i < children->size(); i++)
	{
		Transform_Object* child = (*children)[i];
		a_rotation *= child->get_parent_rotation_multiplier();
		child->rotate(a_rotation);
	}
}

void Transform_Object::rotate_around_anchor(glm::vec3 a_rotation, glm::vec3 rotation_multiplier)
{
	position_offset = (rotate_vector(-get_anchored_position(), a_rotation * glm::vec3(1, -1, 1), glm::vec3(0, 0, 0), rotation_multiplier));
}

// Set the rotation
void Transform_Object::set_rotation(glm::vec3 new_rotation, glm::vec3 rotation_multiplier, bool rotate_around)
{
	if (rotation_multiplier[0] == 1)
		rotation[0] = new_rotation[0];
	if (rotation_multiplier[1] == 1)
		rotation[1] = new_rotation[1];
	if (rotation_multiplier[2] == 1)
		rotation[2] = new_rotation[2];

	if (rotate_around)
		rotate_around_anchor(new_rotation, rotation_multiplier);

	calculate_direction();
};

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

		if (animation->modify_position) set_position_animation(get_current_animation_position());
		if (animation->modify_rotation) set_rotation(get_current_animation_rotation());
		if (animation->modify_scale) set_scale(get_current_animation_scale());

		// Calculate new transform
		float animation_purcentate = animation->state / animation->duration;
		if (animation_purcentate >= 1)
		{
			get_animations()->pop_back();
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
	projection = glm::perspective(glm::radians(get_fov()), float(window_width) / float(window_height), 1.0f / get_far(), get_far());

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
Base_Struct::Base_Struct(double& a_mouse_x, double& a_mouse_y, std::string a_exec_path): exec_path(a_exec_path), mouse_x(a_mouse_x), mouse_y(a_mouse_y), last_mouse_x(a_mouse_x), last_mouse_y(a_mouse_y)
{
	
}

// Cout an error in the program
void Base_Struct::error(std::string thrower, std::string error_content)
{
	std::cout << thrower << " : error ! " << error_content << std::endl;
}

// Return if a file formatted with the struct context
std::string Base_Struct::file_formatted(std::string path)
{
	std::string access = cut_string(path, "/")[0];
	std::string current_path = (std::filesystem::current_path().string());
	std::vector<std::string> cutted_path = cut_string(current_path, "\\");
	unsigned short point_count = 0;
	unsigned short size_to_delete = 0;

	for (int i = 1; i < path.size(); i++) // Count the . in the path
	{
		if (path[i] == '.')
		{
			point_count++;
			size_to_delete += cutted_path[cutted_path.size() - (i + 1)].size() + 1;
		}
		else
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				point_count += 1;
			}
			break;
		}
	}

	path = current_path.substr(0, current_path.size() - size_to_delete) + "/" + path.substr(point_count, path.size() - point_count);

	return path;
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