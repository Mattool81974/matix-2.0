#pragma once

#include "advanced_struct.h"
#include "base_struct.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include "model.h"
#include "scene.h"
#include <string>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

class Game
{
public:
	Game(int a_window_width, int a_window_height);
	void add_scene(std::string name, Scene* scene);
	bool contains_scene(std::string name);
	Scene* new_scene(std::string name);
	void run();
	void update();
	~Game();

	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline Scene* get_current_scene() { return (*get_scenes())[get_current_scene_name()]; };
	inline std::string get_current_scene_name() { return current_scene; };
	inline std::map<std::string, Scene*> *get_scenes() { return &scenes; };
	inline int get_window_height() { return window_height; };
	inline int get_window_width() { return window_width; };
	inline void set_current_scene(std::string a_name) { current_scene = a_name; };
private:
	Advanced_Struct *advanced_struct = 0;

	std::string current_scene = "";
	float last_frame_time = 0;
	int window_height = 0;
	int window_width = 0;

	std::map<std::string, Scene *> scenes = std::map<std::string, Scene *>();
	GLFWwindow* window = 0;
};

