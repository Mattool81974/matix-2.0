#pragma once

#include "../headers/base_struct.h"
#include "../headers/graphic.h"
#include "../headers/model.h"
#include "../headers/physic.h"

class Part
{
	// Class representing a part of a map to load into a scene
public:
	Part(glm::vec3 a_position = glm::vec3(0, 0, 0), glm::vec3 a_rotation = glm::vec3(0.0, 0.0, 0.0), glm::vec3 a_scale = glm::vec3(1.0, 1.0, 1.0), std::string a_type = "", std::string a_texture_path = "", void* a_base_object = 0); // Part constructor
	Part(const Part& copy); // Part copy constructor
	~Part(); // Part destructor

	// Getters and setters
	inline void* get_base_object() { return base_object; };
	inline std::string get_description() { return description; };
	inline glm::vec3 get_position() { return position; };
	inline bool get_resize_texture() { return resize_texture; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale; };
	inline glm::vec3 get_scale_level_multiplier() { return scale_level_multiplier; };
	inline std::string get_texture_path() { return texture_path; };
	inline std::string get_type() { return type; };
	inline bool is_transparent() { return transparent; };
	inline void set_description(std::string a_description) { description = a_description; };
	inline void set_is_transparent(bool new_transparent) { transparent = new_transparent; };
	inline void set_resize_texture(bool a_resize_texture) { resize_texture = a_resize_texture; };
	inline void set_scale_level_multiplier(glm::vec3 a_scale_level_multiplier) { scale_level_multiplier = a_scale_level_multiplier; };
	inline void set_use_collection(bool a_use_collection) { collection = a_use_collection; };
	inline bool use_collection() { return collection; };
private:
	std::string description = "1"; // Little description of the object, usefull for debug
	bool resize_texture = true; // If the texture should be resized or not
	bool collection = true; // If the part can use collection
	bool transparent = false; // If the part use transparency

	void* base_object = 0; // Pointer to a buffer for the base object
	glm::vec3 position; // Position of the part
	glm::vec3 rotation; // Rotation of the part
	glm::vec3 scale; // Scale of the part
	glm::vec3 scale_level_multiplier = glm::vec3(0, 0, 0); // Position level of the part

	std::string texture_path; // Texture path of the part
	std::string type; // Type of the part
};

class Advanced_Struct : public Base_Struct
{
	// Class representing the advanced struct in the game
public:
	Advanced_Struct(double& a_mouse_x, double& a_mouse_y, std::string a_exec_path); // Advanced_Struct constructor
	void assign_part(unsigned int number, Part* part); // Assign to a number a part
	bool contains_font(std::string font_name); // Returns if the struct contains a textures
	bool contains_part(unsigned int number); // Returns if the struct contains a part
	bool contains_texture(std::string texture_path); // Returns if the struct contains a textures
	bool contains_vao(std::string type); // Returns if the struct contains a VAO
	Font_Texture* get_font_texture(std::string font_name); // Return a fotn in the struct
	Part* get_part(unsigned int number); // Returns a part
	Texture* get_texture(std::string texture_path, bool texture_resize = true); // Returns a texture in the struct
	void load_hud_VAOs(); // Loads the HUD VAOs in the advanced struct
	void load_VAOs(); // Loads the VAOs in the advanced struct
	template <class O = Object> // Template for adding a type of object
	Part* new_part(unsigned int number, std::string type, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(0, 0, 0), std::string texture_path = ""); // Create a new part into the struct and return it
	VAO* new_vao(std::string path, std::string type, std::string shader_path = "-1"); // Create a new VAO into the game
	void unload_fonts(); // Unload all the textures
	void unload_textures(); // Unload all the textures
	~Advanced_Struct(); // Advanced_Struct destructor

	// Getters and setters
	inline std::map<std::string, VAO*> *get_all_vaos() { return &all_vaos; };
	inline std::map<std::string, Font_Texture*>* get_fonts_textures() { return &fonts_textures; };
	inline std::map<std::string, Font_VAO*>* get_fonts_vaos() { return &all_fonts_vaos; };
	inline std::map<unsigned int, Part*>* get_parts() { return &parts; };
	inline std::map<std::string, Texture*>* get_textures() { return &textures; };
	inline std::map<std::string, std::string>* get_type() { return &types; };
	inline std::map<std::string, VAO*>* get_vaos() { return &all_vaos; };
private:
	std::map<std::string, Font_Texture*> fonts_textures = std::map<std::string, Font_Texture*>(); // Each texture, with their texture path as key, in the game
	std::map<unsigned int, Part*> parts = std::map<unsigned int, Part*>(); // Each parts, with their number as key, in the game
	std::map<std::string, Texture*> textures = std::map<std::string, Texture*>(); // Each texture, with their texture path as key, in the game
	std::map<std::string, std::string> types = std::map<std::string, std::string>(); // Each types, with their main type as key, in the game
	std::map<std::string, Font_VAO*> all_fonts_vaos = std::map<std::string, Font_VAO*>(); // Each vaos, with their type as key, in the game
	std::map<std::string, VAO*> all_vaos = std::map<std::string, VAO*>(); // Each vaos, with their type as key, in the game
};

struct One_Collision
{
	// Struct representing datas about a collision
	glm::vec3 axis_multiplier = glm::vec3(0, 0, 0); // Multiplier for each axis
	bool collide = true; // If there is a collision or not
	void* object1 = 0; // Pointer to the first object
	void* object2 = 0; // Pointer to the second object
	glm::vec3 position = glm::vec3(0, 0, 0); // Position of the collision
};

struct One_Raycast
{
	// Struct representing datas about a raycast
	char axis = '0'; // Axis of the raycast
	glm::vec3 contact_pos = glm::vec3(0, 0, 0); // Exact position of the contact
	void* touched_object = 0; // Object touched
};

class Collision_Result
{
	// Class representing the result of a detected collision
public:
	Collision_Result(void* a_parent = 0); // Collision_Result contructor
	Collision_Result(const Collision_Result& copy); // Collision_Result copy constructor
	inline void clear() { positions.clear(); }; // Clear the collision result
	~Collision_Result(); // Collision_Result destructor

	// Getters
	inline void add_collision(One_Collision collision) { positions.push_back(collision); };
	inline One_Collision* at(unsigned short pos) { return &positions[pos]; };
	inline bool is_colliding() { return collide; };
	inline void set_colliding(bool colliding) { collide = colliding; };
	inline unsigned short size() { return positions.size(); };
private:
	bool collide = true; // If the 2 objects are colliding or not
	void* parent = 0; // Pointer to the parent of the collision result

	std::vector<One_Collision> positions = std::vector<One_Collision>(); // Positions of the collisions
};

class Object
{
	// Class representing an object into a scene
public:
	Object(Advanced_Struct* a_game_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Object constructor
	virtual void after_loading() {}; // Function called after loading, usefull for heritage
	virtual void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the object
	virtual One_Collision collides_with(Object* object); // Returns if the object collides with an other object
	virtual void last_update(); // Update the object after physic collision
	virtual void late_update() {}; // Update the object after physic modification beforce physic collision
	std::vector<glm::vec3> set_map_pos(glm::vec3 a_map_pos); // Change the middle pos of the object in the map and return the list of pos in the map
	virtual void update() { }; // Update the object
	~Object(); // Object destructor

	// Getters and setters
	inline bool can_reset_map_pos() { return reset_map_pos; };
	inline bool contains_tag(std::string tag) { for (int i = 0; i < tags.size(); i++) { if (tags[i] == tag) { return true; } } return false; };
	inline Advanced_Struct* get_game_struct() { return game_struct; };
	std::vector<glm::vec3> get_all_map_pos(glm::vec3 movement_use = glm::vec3(0, 0, 0), bool add_movement = false);
	inline Graphic_Object* get_attached_graphic_object() { return attached_graphic; };
	inline Physic_Object* get_attached_physic_object() { return attached_physic; };
	inline Transform_Object* get_attached_transform() { return attached_transform; };
	inline Collision_Result* get_collisions_result() { return &collision_result; };
	inline std::string get_description() { return description; };
	inline glm::vec3 get_last_map_pos() { return last_map_pos; };
	inline glm::vec3 get_map_pos() { return map_pos; };
	inline std::string get_name() { return name; };
	inline std::string get_scene_name() { return scene_name; };
	inline std::vector<std::string>* get_tags() { return &tags; };
	inline std::string get_type() { return type; };
	inline void set_description(std::string a_description) { description = a_description; };
	inline bool use_graphic() { return get_attached_graphic_object() != 0; };
	inline bool use_physic() { return get_attached_physic_object() != 0; };
protected:
	bool reset_map_pos = true; // If the map pos should be reset at each frame
	std::string type = ""; // Type of the object
private:
	std::string description = "1"; // Little description of the object, usefull for debug
	std::string name; // Name of the object
	std::string scene_name; // Name of the scene of the object
	Graphic_Object* attached_graphic = 0; // Graphic object attached
	Physic_Object* attached_physic = 0; // Physic object attached
	Transform_Object* attached_transform = 0; // Transform object attached
	Collision_Result collision_result;
	Advanced_Struct* game_struct = 0; // Base struct in the game
	glm::vec3 last_map_pos = glm::vec3(-1, -1, -1); // The pos of the object in the physic map, or -1 if not in it
	glm::vec3 map_pos = glm::vec3(-1, -1, -1); // The pos of the object in the physic map, or -1 if not in it
	std::vector<std::string> tags = std::vector<std::string>(); // Tags about the object
};

// Create a new part into the struct and return it
template <class O> // Template for adding a type of object
Part* Advanced_Struct::new_part(unsigned int number, std::string type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string texture_path)
{
	if (contains_part(number)) { std::cout << "Matrix game : error ! The part \"" << number << "\" you want to create already exist." << std::endl; }

	// Create the part
	Part* part = new Part(position, rotation, scale, type, texture_path, new O());
	assign_part(number, part);
	return part;
}