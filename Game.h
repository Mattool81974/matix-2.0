#pragma once

#include "advanced_struct.h"
#include "base_struct.h"
#include "model.h"
#include "scene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Callback function for window resizing
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Callback function for mouse moving

class Game: public Advanced_Struct
{
	// Class representing the game
public:
	Game(int a_window_width, int a_window_height); // Game constructor
	void add_hud_object(std::string name,  HUD_Object* object); // Add an existing HUD to the game
	void add_scene(std::string name, Scene* scene); // Add an existing scene to the game
	bool contains_hud_object(std::string name); // Return if the game contains an HUD Object
	bool contains_scene(std::string name); // Returns if the game contains a scene
	void load_keys(); // Load the keys in the game
	HUD_Object* new_hud_object(std::string name, std::string texture_path = ""); // Create a new HUD Object into the game
	Scene* new_scene(std::string name, std::string map_path = "", Map_Opening_Mode mode = Map_Opening_Mode::Simple, bool use_graphic = true, bool use_physic = true); // Create a scene into the game and return it
	bool run(); // Run the game by doing multiples call to update
	void update(); // Update one frame of the game
	void update_event(); // Update the event of the game during this frame
	~Game(); // Game destructor

	// Getters and setters
	inline Scene* get_current_scene() { return (*get_scenes())[get_current_scene_name()]; };
	inline std::string get_current_scene_name() { return current_scene; };
	inline std::map < std::string, HUD_Object*>* get_hud_objects() { return &hud_objects; };
	inline Scene* get_scene(std::string name) { if (contains_scene(name)) { return scenes[name]; } std::cout << "Matrix game : error ! The scene \"" << name << "\" does not exist." << std::endl; return 0; }
	inline std::map<std::string, Scene*> *get_scenes() { return &scenes; };
	inline int get_window_height() { return window_height; };
	inline int get_window_width() { return window_width; };
	void set_current_scene(std::string a_name);
private:
	std::string current_scene = ""; // Name of the current scene loaded
	float last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
	int window_height = 0; // Height of the graphic window
	int window_width = 0; // Widt of the graphic window

	std::map < std::string, HUD_Object*> hud_objects = std::map < std::string, HUD_Object*>(); // Each HUD_Object, with their name as key, in the game
	std::map<std::string, Scene *> scenes = std::map<std::string, Scene *>(); // Each scenes, with their name as key, in the game
	GLFWwindow* window = 0; // Pointer to the GLFW window
};

