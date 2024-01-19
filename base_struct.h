#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

std::vector<std::string> cut_string(std::string string, std::string cut); // Cut a string where there are the "cut"

class Transform_Object
{
	// Class representing a object with a transform value
public:
	Transform_Object(Transform_Object* a_parent = 0, glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Transform_Object contructor
	Transform_Object(const Transform_Object &copy); // Transform_Object copy constructor
	void calculate_direction(); // Calculate the direction vector of the transform object
	glm::mat4 get_model_matrix(); // Return the transformation matrix of the object
	void move(glm::vec3 a_mouvement); // Move the object
	void rescale(glm::vec3 a_scale); // Rotate the object
	void rotate(glm::vec3 a_rotation); // Scale the object
	void soft_reset(); // Reset softly the object
	void update() {}; // Update the object
	~Transform_Object(); // Transform_Object destructor

	// Getters
	inline glm::vec3 get_absolute_position() { if (get_parent() != 0) { return get_parent()->get_absolute_position() + get_position(); } return get_position(); };
	inline glm::vec3 get_forward() { return forward; };
	inline glm::vec3 get_movement() { return movement; };
	inline Transform_Object* get_parent() { return parent; };
	inline glm::vec3 get_position() { return position; };
	inline glm::vec3 get_right() { return right; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline glm::vec3 get_up() { return up; };

	// Setters
	inline void set_parent(Transform_Object* new_parent) { parent = new_parent; };
	inline void set_movement(glm::vec3 new_movement) { movement = new_movement; };
	inline void set_position(glm::vec3 new_position) { position = new_position; };
	inline void set_rotation(glm::vec3 new_rotation) { rotation = new_rotation; calculate_direction(); };
	inline void set_scale(glm::vec3 new_scale) { scale = new_scale; };
private:
	Transform_Object* parent = 0; // Pointer to the parent of the object

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f); // Forward vector of the object
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 1.0f); // Right vector for the object
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector for the object

	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f); // Movement of the object
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // Position of the object
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
	~Camera(); // Camera destructor

	// Getter
	inline float get_fov() { return fov; };
private:
	float fov = 45.0f; // Fov of the camera
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
	inline double get_last_mouse_x() { return last_mouse_x; };
	inline double get_last_mouse_y() { return last_mouse_y; };
	inline double get_mouse_move_x() { return mouse_move_x; };
	inline double get_mouse_move_y() { return mouse_move_y; };
	inline double get_mouse_x() { return mouse_x; };
	inline double get_mouse_y() { return mouse_y; };
	inline int get_window_height() { return window_height; };
	inline int get_window_width() { return window_width; };
	inline void set_delta_time(float new_delta_time) { delta_time = new_delta_time; };
	inline void set_last_mouse_x(double a_last_mouse_x) { last_mouse_x = a_last_mouse_x; };
	inline void set_last_mouse_y(double a_last_mouse_y) { last_mouse_y = a_last_mouse_y; };
	inline void set_mouse_move_x(double a_mouse_move_x) { mouse_move_x = a_mouse_move_x; };
	inline void set_mouse_move_y(double a_mouse_move_y) { mouse_move_y = a_mouse_move_y; };
	inline void set_window_height(int height) { window_height = height; };
	inline void set_window_width(int width) { window_width = width; };
private:
	float delta_time = 0; // Time since the last frame of the game
	double last_mouse_x = 0; // Last X position of the mouse
	double last_mouse_y = 0; // Last Y position of the mouse
	double mouse_move_x = 0; // Last X move of the mouse
	double mouse_move_y = 0; // Last Y move of the mouse
	double& mouse_x; // Reference towards the mouse X pos
	double& mouse_y; // Reference towards the mouse Y pos
	int window_height = 0; // Height of the graphic window
	int window_width = 0; // Width of the graphic window

	Camera camera; // Camera of the game
};