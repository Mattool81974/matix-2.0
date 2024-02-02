#pragma once

#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <random>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> cut_string(std::string string, std::string cut, bool erase_blank = false); // Cut a string where there are the "cut"
glm::vec3 normalize_rotation(glm::vec3 rotation); // Normalize a rotation and return it
glm::vec3 rotate_vector(glm::vec3 vector, glm::vec3 rotation, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation_multiplier = glm::vec3(1, 1, 1)); // Rotate a vector around a rotating point
float sign(float number); // Return the sign of a number

struct Transform_Animation {
	// Struct representing a transform object animation
	glm::vec3 base_position = glm::vec3(0, 0, 0);
	glm::vec3 base_rotation = glm::vec3(0, 0, 0);
	glm::vec3 base_scale = glm::vec3(1, 1, 1);

	float duration = 0;

	glm::vec3 final_position = glm::vec3(0, 0, 0);
	glm::vec3 final_rotation = glm::vec3(0, 0, 0);
	glm::vec3 final_scale = glm::vec3(1, 1, 1);

	bool modify_position = true;
	bool modify_rotation = true;
	bool modify_scale = true;

	float state = 0;
};

class Transform_Object
{
	// Class representing a object with a transform value
public:
	Transform_Object(Transform_Object* a_parent = 0, glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Transform_Object contructor
	Transform_Object(const Transform_Object &copy); // Transform_Object copy constructor
	void add_animation(float duration, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale); // Add an animation to the object
	void add_animation(float duration, glm::vec3 base_position, glm::vec3 base_rotation, glm::vec3 base_scale, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale); // Add an animation to the object with base transform
	void add_position_animation(float duration, glm::vec3 base_position, glm::vec3 final_position); // Add an animation to the object with the position
	void add_rotation_animation(float duration, glm::vec3 base_rotation, glm::vec3 final_rotation); // Add an animation to the object with the rotation
	void calculate_direction(); // Calculate the direction vector of the transform object
	glm::mat4 get_model_matrix(); // Return the transformation matrix of the object
	void move(glm::vec3 a_mouvement); // Move the object
	void remove_child(Transform_Object* object); // Remove an object from the children
	void rescale(glm::vec3 a_scale); // Rescale the object
	void reset_animation(bool reset_position = true); // Reset the animation of the object
	virtual void rotate(glm::vec3 a_rotation, bool rotate_around = true); // Rotate the object
	void rotate_around_anchor(glm::vec3 a_rotation, glm::vec3 rotation_multiplier = glm::vec3(1, 1, 1)); // Rotate the object around the anchored point
	void set_rotation(glm::vec3 a_rotation, glm::vec3 rotation_multiplier = glm::vec3(1, 1, 1), bool rotate_around = true); // Set the rotation around the anchord point
	void soft_reset(); // Reset softly the object
	virtual void update() { update_animation(); }; // Update the object
	void update_animation(); // Update the animations for the object
	~Transform_Object(); // Transform_Object destructor

	// Getters
	inline glm::vec3 get_absolute_position()
	{
		if (get_parent() != 0)
		{
			return get_parent()->get_absolute_position() + get_position() + get_position_animation() + position_offset;
		}
		return get_position() + get_position_animation() + position_offset;
	};
	inline glm::vec3 get_anchored_position() { return anchored_position; };
	inline std::vector<Transform_Animation>* get_animations() { return &animations; };
	inline std::vector<Transform_Object*> *get_children() { return &children; };
	inline Transform_Animation* get_current_animation() { if (get_animations()->size() <= 0) return 0; return &((*get_animations())[get_animations()->size() - 1]); };
	inline glm::vec3 get_current_animation_position() {
		Transform_Animation* animation = get_current_animation();
		glm::vec3 new_position = animation->final_position;

		// Calculate new transform
		float animation_purcentate = animation->state / animation->duration;
		if (animation_purcentate < 1)
		{
			glm::vec3 position_difference = animation->final_position - animation->base_position;
			new_position = animation->base_position + position_difference * animation_purcentate;
		}
		return new_position;
	};
	inline glm::vec3 get_current_animation_rotation() {
		Transform_Animation* animation = get_current_animation();
		glm::vec3 new_rotation = animation->final_rotation;

		// Calculate new transform
		float animation_purcentate = animation->state / animation->duration;
		if (animation_purcentate < 1)
		{
			glm::vec3 rotation_difference = animation->final_rotation - animation->base_rotation;
			new_rotation = animation->base_rotation + rotation_difference * animation_purcentate;
		}
		return new_rotation;
	};
	inline glm::vec3 get_current_animation_scale() {
		Transform_Animation* animation = get_current_animation();
		glm::vec3 new_scale = animation->final_scale;

		// Calculate new transform
		float animation_purcentate = animation->state / animation->duration;
		if (animation_purcentate < 1)
		{
			glm::vec3 scale_difference = animation->final_scale - animation->base_scale;
			new_scale = animation->base_scale + scale_difference * animation_purcentate;
		}
		return new_scale;
	};
	inline glm::vec3 get_forward() { return forward; };
	inline glm::vec3 get_movement() { return movement; };
	inline Transform_Object* get_parent() { return parent; }
	inline glm::vec3 get_parent_rotation_multiplier() { return parent_rotation_multiplier; };
	inline glm::vec3 get_position() { return position; };
	inline glm::vec3 get_position_animation() { return position_animation; };
	inline glm::vec3 get_position_move_multiplier() { return position_move_multiplier; };
	inline glm::vec3 get_position_offset() { return position_offset; };
	inline glm::vec3 get_right() { return right; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline glm::vec3 get_up() { return up; };
	inline bool is_animation_playing() { return animation_playing; };

	// Setters
	inline void set_anchored_position(glm::vec3 a_anchored_position) { anchored_position = a_anchored_position; position_offset = -a_anchored_position; };
	inline void set_parent(Transform_Object* new_parent) { if (get_parent() != 0) { get_parent()->remove_child(this); } parent = new_parent; if (new_parent != 0) { new_parent->get_children()->push_back(this); } };
	inline void set_parent_rotation_multiplier(glm::vec3 a_parent_rotation_multiplier) { parent_rotation_multiplier = a_parent_rotation_multiplier; };
	inline void set_movement(glm::vec3 new_movement) { movement = new_movement; };
	inline void set_position(glm::vec3 new_position) { position = new_position; };
	inline void set_position_animation(glm::vec3 new_position) { position_animation = new_position; };
	inline void set_position_move_multipler(glm::vec3 a_position_move_multiplier) { position_move_multiplier = a_position_move_multiplier; };
	inline void set_scale(glm::vec3 new_scale) { scale = new_scale; };
	inline void start_animation() { animation_playing = true; };
	inline void stop_animation() { animation_playing = false; };
protected:
	glm::vec3 position_offset = glm::vec3(0.0f, 0.0f, 0.0f); // Offset position of the object
private:
	bool animation_playing = false; // If the animation is playing or not
	std::vector<Transform_Animation> animations = std::vector<Transform_Animation>(); // List of all the animations in the object

	std::vector<Transform_Object*> children = std::vector<Transform_Object*>(); // List of all the children of the object
	Transform_Object* parent = 0; // Pointer to the parent of the object

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f); // Forward vector of the object
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 1.0f); // Right vector for the object
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector for the object

	glm::vec3 anchored_position = glm::vec3(0.0f, 0.0f, 0.0f); // Base position of the object
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f); // Movement of the object
	glm::vec3 parent_rotation_multiplier = glm::vec3(1.0f, 1.0f, 1.0f); // Multiplier to apply to a rotation coming from a parent
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // Position of the object
	glm::vec3 position_animation = glm::vec3(0.0f, 0.0f, 0.0f); // Position of the animation of the object
	glm::vec3 position_move_multiplier = glm::vec3(1.0, 1.0, 1.0); // Multiplier for the poisitions used in "move"
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f); // Rotation of the object
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); // Scale of the object
};

class Camera: public Transform_Object
{
	// Class representing a camera into the game
public:
	Camera(glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Camera constructor
	glm::mat4 get_projection(int window_height, int window_width); // Return the projection matrix
	glm::mat4 get_view(); // Return the view matrix
	void rotate(glm::vec3 a_rotation); // Rotate the object
	~Camera(); // Camera destructor

	// Getter
	inline float get_far() { return far; };
	inline float get_fov() { return fov; };
	inline float get_sensitivity() { return sensitivity; };
	inline void set_fov(float a_fov) { fov = a_fov; };
private:
	float far = 1000.0f; // Far projection for the camera
	float fov = 45.0f; // Fov of the camera
	float sensitivity = 30; // Sensitivity of the camera
};

class Base_Struct
{
public:
	Base_Struct(double& a_mouse_x, double& a_mouse_y); // Base_Struct constructor
	glm::mat4 get_projection(); // Return the projection matrix
	~Base_Struct(); // Base_Struct destructor

	// Getters and setters
	inline Camera* get_camera() { return &camera; };
	inline float get_delta_time() { return delta_time; };
	inline glm::vec3 get_gravity_force() { return gravity_force; };
	inline unsigned short get_key_state(std::string name) { return (*get_keys_state())[name]; };
	inline unsigned short get_key_state_frame(std::string name) { return (*get_keys_state_frame())[name]; };
	inline std::map<std::string, unsigned short>* get_keys_state() { return &keys_state; };
	inline std::map<std::string, unsigned short>* get_keys_state_frame() { return &keys_state_frame; };
	inline double get_last_mouse_x() { return last_mouse_x; };
	inline double get_last_mouse_y() { return last_mouse_y; };
	inline unsigned short get_left_mouse_button_state() { return left_mouse_button_state; };
	inline double get_mouse_move_x() { return mouse_move_x; };
	inline double get_mouse_move_y() { return mouse_move_y; };
	inline double get_mouse_x() { return mouse_x; };
	inline double get_mouse_y() { return mouse_y; };
	inline unsigned short get_right_mouse_button_state() { return right_mouse_button_state; };
	inline int get_window_height() { return window_height; };
	inline int get_window_width() { return window_width; };
	inline void set_delta_time(float new_delta_time) { delta_time = new_delta_time; };
	inline void set_gravity_force(glm::vec3 a_gravity_force) { gravity_force = a_gravity_force; };
	inline void set_last_mouse_x(double a_last_mouse_x) { last_mouse_x = a_last_mouse_x; };
	inline void set_last_mouse_y(double a_last_mouse_y) { last_mouse_y = a_last_mouse_y; };
	inline void set_left_mouse_button_state(unsigned short new_state) { left_mouse_button_state = new_state; };
	inline void set_mouse_move_x(double a_mouse_move_x) { mouse_move_x = a_mouse_move_x; };
	inline void set_mouse_move_y(double a_mouse_move_y) { mouse_move_y = a_mouse_move_y; };
	inline void set_right_mouse_button_state(unsigned short new_state) { right_mouse_button_state = new_state; };
	inline void set_window_height(int height) { window_height = height; };
	inline void set_window_width(int width) { window_width = width; };
private:
	float delta_time = 0; // Time since the last frame of the game
	double last_mouse_x = 0; // Last X position of the mouse
	double last_mouse_y = 0; // Last Y position of the mouse
	unsigned short left_mouse_button_state = 0; // State of the left button mouse
	unsigned short right_mouse_button_state = 0; // State of the left button mouse
	double mouse_move_x = 0; // Last X move of the mouse
	double mouse_move_y = 0; // Last Y move of the mouse
	double& mouse_x; // Reference towards the mouse X pos
	double& mouse_y; // Reference towards the mouse Y pos
	int window_height = 0; // Height of the graphic window
	int window_width = 0; // Width of the graphic window

	Camera camera; // Camera of the game
	glm::vec3 gravity_force = glm::vec3(0, -9.80665, 0); // Value of the gravity
	std::map<std::string, unsigned short> keys_state = std::map<std::string, unsigned short>(); // Current state of the key
	std::map<std::string, unsigned short> keys_state_frame = std::map<std::string, unsigned short>(); // State of the key if changed during this frame
};