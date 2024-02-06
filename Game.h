#pragma once

#include "advanced_struct.h"
#include "base_struct.h"
#include "model.h"
#include "scene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Callback function for window resizing
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Callback function for mouse moving

class HUD
{
	// Class representing an HUD
public:
	HUD(Advanced_Struct* a_advanced_struct, std::string a_name); // HUD constructor
	void add_hud_object(std::string name, HUD_Object* object); // Add an existing HUD to the game
	bool contains_hud_object(std::string name); // Return if the game contains an HUD Object
	template <class O = HUD_Object> // Template for adding a type of HUD object
	O* new_hud_object(std::string name, std::string texture_path = "", std::string vao_name = "hud"); // Create a new HUD Object into the game
	void render(); // Render the HUD
	void sort_objects(); // Sort the HUD for a good render
	void unload(); // Unload the objects in the HUD
	virtual void update(); // Update the HUD
	void update_object(); // Update all the objects in the HUD
	~HUD(); // HUD destructor

	// Getters and setters
	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline HUD_Object* get_hud_object(std::string a_name) { return hud_objects[a_name]; };
	inline std::map < std::string, HUD_Object*>* get_hud_objects() { return &hud_objects; };
	inline std::string get_name() { return name; };
	inline std::vector<HUD_Object*>* get_sorted_hud_objects() { return &sorted_hud_objects; };
private:
	std::string name; // Name of the HUD

	Advanced_Struct* advanced_struct = 0;
	std::map < std::string, HUD_Object*> hud_objects = std::map < std::string, HUD_Object*>(); // Each HUD_Object, with their name as key, in the HUD
	std::vector<HUD_Object*> sorted_hud_objects = std::vector<HUD_Object*>(); // Each HUD_Object, shorted for rendering
};

class Game: public Advanced_Struct
{
	// Class representing the game
public:
	Game(int a_window_width, int a_window_height); // Game constructor
	void add_hud(std::string name, HUD* object); // Add an existing HUD to the game
	void add_scene(std::string name, Scene* scene); // Add an existing scene to the game
	bool contains_hud(std::string name); // Return if the game contains an HUD Object
	bool contains_scene(std::string name); // Returns if the game contains a scene
	void load_keys(); // Load the keys in the game
	template <class O = HUD> // Template for adding a type of HUD
	O* new_hud(std::string name); // Create a new HUD into the game
	Scene* new_scene(std::string name, std::string map_path = "", Map_Opening_Mode mode = Map_Opening_Mode::Simple, bool use_graphic = true, bool use_physic = true); // Create a scene into the game and return it
	bool run(); // Run the game by doing multiples call to update
	void update(); // Update one frame of the game
	void update_event(); // Update the event of the game during this frame
	~Game(); // Game destructor

	// Getters and setters
	inline bool continue_running() { return is_running; };
	inline glm::vec4 get_background_color() { return background_color; };
	inline HUD* get_current_hud() { if (get_current_hud_name() == "") return 0; return (*get_huds())[get_current_hud_name()]; };
	inline Scene* get_current_scene() { if (get_current_scene_name() == "") return 0; return (*get_scenes())[get_current_scene_name()]; };
	inline std::string get_current_hud_name() { return current_hud; };
	inline std::string get_current_scene_name() { return current_scene; };
	inline HUD* get_hud(std::string a_name) { return huds[a_name]; };
	inline std::map < std::string, HUD*>* get_huds() { return &huds; };
	inline Scene* get_scene(std::string name) { if (contains_scene(name) && name != "") { return scenes[name]; } std::cout << "Matrix game : error ! The scene \"" << name << "\" does not exist." << std::endl; return 0; }
	inline std::map<std::string, Scene*> *get_scenes() { return &scenes; };
	inline int get_window_height() { return window_height; };
	inline int get_window_width() { return window_width; };
	inline void set_background_color(glm::vec4 a_background_color) { background_color = a_background_color; };
	void set_current_hud(std::string a_name);
	void set_current_scene(std::string a_name);
	inline void set_is_running(bool a_run) { is_running = a_run; };
private:
	std::string current_hud = ""; // Name of the current hud loaded
	std::string current_scene = ""; // Name of the current scene loaded
	bool is_running = true; // If the game should continue running or not
	float last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
	int window_height = 0; // Height of the graphic window
	int window_width = 0; // Widt of the graphic window

	glm::vec4 background_color = glm::vec4(0.0f, (1.0f/255.0f) * 204.0f, (1.0f / 255.0f) * 204.0f, 1.0f); // Background color of the game
	std::map < std::string, HUD*> huds = std::map < std::string, HUD*>(); // Each HUD, with their name as key, in the game
	std::map<std::string, Scene *> scenes = std::map<std::string, Scene *>(); // Each scenes, with their name as key, in the game
	GLFWwindow* window = 0; // Pointer to the GLFW window
};

// Create a new HUD Object into the HUD
template <class O> // Template for adding a type of HUD object
O* HUD::new_hud_object(std::string name, std::string texture_path, std::string vao_name)
{
	if (contains_hud_object(name)) { std::cout << "HUD \"" << get_name() << "\" error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

	bool texture_resize = false; // Load the texture
	Texture* texture = get_advanced_struct()->get_texture(texture_path, texture_resize);

	// Load the VAO
	VAO* vao = (*get_advanced_struct()->get_all_vaos())[vao_name];

	O* new_object = new O(get_advanced_struct(), name, texture, vao);
	add_hud_object(name, new_object);
	return new_object;
}

// Create a new HUD Object into the game
template <class O> // Template for adding a type of HUD
O* Game::new_hud(std::string name)
{
	if (contains_scene(name)) { std::cout << "Matix game : error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

	O* new_object = new O(this, name);
	add_hud(name, new_object);
	return new_object;
}