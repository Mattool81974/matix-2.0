#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform_Object
{
public:
	Transform_Object(Transform_Object* a_parent = 0, glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f));
	Transform_Object(const Transform_Object &copy);
	void calculate_direction();
	glm::mat4 get_model_matrix();
	void move(glm::vec3 a_mouvement);
	void rescale(glm::vec3 a_scale);
	void rotate(glm::vec3 a_rotation);
	void soft_reset();
	void update() {};
	~Transform_Object();

	inline glm::vec3 get_absolute_position() { if (get_parent() != 0) { return get_parent()->get_absolute_position() + get_position(); } return get_position(); };
	inline glm::vec3 get_forward() { return forward; };
	inline glm::vec3 get_movement() { return movement; };
	inline Transform_Object* get_parent() { return parent; };
	inline glm::vec3 get_position() { return position; };
	inline glm::vec3 get_right() { return right; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline glm::vec3 get_up() { return up; };

	inline void set_parent(Transform_Object* new_parent) { parent = new_parent; };
	inline void set_movement(glm::vec3 new_movement) { movement = new_movement; };
	inline void set_position(glm::vec3 new_position) { position = new_position; };
	inline void set_rotation(glm::vec3 new_rotation) { rotation = new_rotation; calculate_direction(); };
	inline void set_scale(glm::vec3 new_scale) { scale = new_scale; };
private:
	Transform_Object* parent = 0;

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Camera: public Transform_Object
{
public:
	Camera(glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 get_projection(int window_height, int window_width);
	glm::mat4 get_view();
	~Camera();

	inline float get_fov() { return fov; };
private:
	float fov = 45.0f;
};

class Base_Struct
{
public:
	Base_Struct(double& a_mouse_x, double& a_mouse_y);
	glm::mat4 get_projection();
	~Base_Struct();

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
	float delta_time = 0;
	double last_mouse_x = 0;
	double last_mouse_y = 0;
	double mouse_move_x = 0;
	double mouse_move_y = 0;
	double& mouse_x;
	double& mouse_y;
	int window_height = 0;
	int window_width = 0;

	Camera camera;
};