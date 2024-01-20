#pragma once

#include "advanced_struct.h"
#include "base_struct.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "model.h"
#include "scene.h"
#include <string>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Callback function for window resizing
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Callback function for mouse moving

class Game
{
	// Class representing the game
public:
	Game(int a_window_width, int a_window_height); // Game constructor
	void add_scene(std::string name, Scene* scene); // Add an existing scene to the game
	bool contains_scene(std::string name); // Returns if the game contains a scene
	Part new_part(unsigned int number, std::string type, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), std::string texture_path = ""); // Create a new part into the struct and return it
	Scene* new_scene(std::string name, std::string map_path = ""); // Create a scene into the game and return it
	void run(); // Run the game by doing multiples call to update
	void update(); // Update one frame of the game
	void update_event(); // Update the event of the game during this frame
	~Game(); // Game destructor

	// Getters and setters
	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline Base_Struct* get_base_struct() { return get_advanced_struct()->get_base_struct(); };
	inline Scene* get_current_scene() { return (*get_scenes())[get_current_scene_name()]; };
	inline std::string get_current_scene_name() { return current_scene; };
	inline std::map<std::string, Scene*> *get_scenes() { return &scenes; };
	inline int get_window_height() { return window_height; };
	inline int get_window_width() { return window_width; };
	inline void set_current_scene(std::string a_name) { current_scene = a_name; };
private:
	Advanced_Struct *advanced_struct = 0; // Pointer to the Advanced_Struct

	std::string current_scene = ""; // Name of the current scene loaded
	float last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
	int window_height = 0; // Height of the graphic window
	int window_width = 0; // Widt of the graphic window

	std::map<std::string, Scene *> scenes = std::map<std::string, Scene *>(); // Each scenes, with their name at key, in the game
	GLFWwindow* window = 0; // Pointer to the GLFW window
};

