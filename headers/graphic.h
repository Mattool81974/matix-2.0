#pragma once

#include "../headers/base_struct.h"
#include "../headers/model.h"

class Graphic_Object
{
	// Class representing a graphic object
public:
	Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, Texture *a_texture, VAO* a_vao); // Graphic_Object contructor
	void render(); // Render the graphic object
	void update(); // Update the graphic object
	~Graphic_Object(); // Graphic_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline Texture* get_texture() { return texture; };
	inline bool is_transparent() { return transparent; };
	inline void set_is_transparent(bool new_transparent) { transparent = new_transparent; };
	inline void set_texture(Texture* a_texture) { texture = a_texture; };
private:
	bool transparent = false; // If the object use transparency or not

	Transform_Object& attached_transform; // Reference to the attached transform object
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	VAO* vao = 0; // Pointer to the VAO used to render the object
	Texture *texture = 0; // Pointer to the Texture used to render the object
};

class HUD_Object
{
	// Class representing a HUD object
public:
	HUD_Object(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao); // HUD_Object constructor
	glm::mat4 get_model_matrix(); // Return the transformation matrix of the object
	virtual void render(); // Render the graphic HUD
	virtual void update(); // Update the graphic HUD
	~HUD_Object(); // HUD_Object destructor

	// Getters and setters
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline glm::vec3 get_position() { return position; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline glm::vec3 get_scale() { return scale * glm::vec3(2, 2, 2); };
	inline Texture* get_texture() { return texture; };
	inline void set_position(glm::vec3 a_position) { position = a_position; };
	inline void set_rotation(glm::vec3 a_rotation) { rotation = a_rotation; };
	inline void set_scale(glm::vec3 a_scale) { scale = a_scale; };
	inline void set_texture(Texture* a_texture) { texture = a_texture; };
protected:
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	VAO* vao = 0; // Pointer to the VAO used to render the object
	Texture* texture = 0; // Pointer to the Texture used to render the object
private:
	std::string name; // Name of the object

	glm::vec3 position = glm::vec3(0, 0, 0); // Position of the HUD on the screen
	glm::vec3 rotation = glm::vec3(0, 0, 0); // Rotation of the HUD on the screen
	glm::vec3 scale = glm::vec3(1, 1, 1); // Size of the HUD on the screen
};

class HUD_Text: public HUD_Object
{
	// Class representing an HUD text object
public:
	HUD_Text(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao); // HUD_Font constructor
	void render(); // Render the text for HUD
	void update(); // Update the text HUD
	void update_cursor(); // Update the cursor of the text
	void update_text_input(); // Apply the text input to the text
	~HUD_Text(); // HUD_Object destructor

	// Getters and setters
	inline bool can_take_input() { return input; };
	inline glm::vec4 get_background_color() { return background_color; };
	inline std::string get_cursor_character() { return cursor_character; };
	inline float get_cursor_display_time() { return cursor_display_time; };
	inline glm::vec4 get_font_color() { return font_color; };
	inline float get_font_size() { return font_size; };
	inline std::string get_input_text() { return input_text; };
	inline std::string get_text(bool with_cursor = false)
	{
		std::string cursor = "";
		if (with_cursor && is_using_cursor() && display_cursor) cursor = get_cursor_character();
		return text + cursor;
	};
	inline Font_Texture* get_texture() { return (Font_Texture*)texture; };
	inline Font_VAO* get_vao() { return (Font_VAO*)vao; };
	inline bool is_focused() { return focused; };
	inline bool is_using_cursor() { return use_cursor; };
	inline void set_background_color(glm::vec4 a_background_color) { background_color = a_background_color; };
	inline void set_cursor_character(std::string new_cursor_character) { cursor_character = new_cursor_character; };
	inline void set_focused(bool a_focused) { focused = a_focused; };
	inline void set_font_color(glm::vec4 a_font_color) { font_color = a_font_color; };
	inline void set_font_size(float size) { font_size = size; };
	inline void set_input(bool a_input) { input = a_input; };
	inline void set_input_text(std::string a_input_text) { input_text = a_input_text; };
	inline void set_text(std::string a_text) { text = a_text; };
	inline void set_use_cursor(bool a_use_cursor) { use_cursor = a_use_cursor; };
private:
	std::string cursor_character = "_"; // Return the character representing the cursor
	float cursor_display_time = 0.5; // Number of second the cursor is displayed and hide
	bool display_cursor = true; // If the cursor should be add to the text at this frame or not
	bool focused = false; // If the object is focused or not
	bool input = false; // If text can be inputed or not
	std::string input_text = ""; // Text that can be took in input
	float time_since_last_cursor_display = 0; // Time passed since the last cursor displaying
	bool use_cursor = false; // If the text use a cursor

	glm::vec4 background_color = glm::vec4(0, 0, 0, 0); // Background color of the object
	glm::vec4 font_color = glm::vec4(0, 0, 0, 1); // Color of the font of the text
	float font_size = 1.0f; // Size of the font of the text
	std::string text = "Matix"; // Text
};