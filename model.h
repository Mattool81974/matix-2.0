#pragma once

#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

struct Shader_Program_Variable
{
	// Struct representing the values for a "in" variable in the shader program
	bool normalized = false;
	unsigned short type = 0; // 0 = GL_FLOAT
	unsigned short vector_size = 1;
};

class Shader_Program
{
	// Class representing a shader program interface
public:
	Shader_Program(std::string a_vertex_shader, std::string a_fragment_shader); // Shader_Program constructor
	void pass_variable(std::vector<Shader_Program_Variable> *variables); // Pass variable to the shader program
	void set_uniform1f_value(std::string name, float v1); // Change the value of a uniform float value
	void set_uniform2f_value(std::string name, float v1, float v2); // Change the value of a uniform vec2 float value
	void set_uniform3f_value(std::string name, float v1, float v2, float v3); // Change the value of a uniform vec3 float value
	void set_uniform4f_value(std::string name, float v1, float v2, float v3, float v4); // Change the value of a uniform vec4 float value
	void set_uniform4f_value(std::string name, glm::vec4 v); // Change the value of a uniform vec4 float value
	void set_uniform4fv_value(std::string name, glm::mat4 fv); // Change the value of a uniform mat4 float value
	void use(); // Start using the shader
	~Shader_Program(); // Shader_Program destructor

	// Getters and setters
	inline std::string get_fragment_shader() const { return fragment_shader; };
	inline std::string get_vertex_shader() const { return vertex_shader; };
private:
	unsigned int shader_program = 0; // Handle to the shader program

	std::string fragment_shader = ""; // Content of the fragment shader
	std::string vertex_shader = ""; // Content of the vertex shader
};

class VBO
{
	// Class representing a VBO interface
public:
	VBO(std::vector<Shader_Program_Variable> a_attributes, std::vector<float> a_datas, bool a_use_ebo = true); // VBO complete constructor
	VBO(std::vector<Shader_Program_Variable> a_attributes, bool fill_datas = true, bool a_use_ebo = true); // VBO constructor
	void bind(); // Bind the VBO into the GPU memory
	void bind_buffer(); // Bind the buffer data of the VBO
	unsigned int get_vertice_number(); // Returns the number of vertices into the VBO
	void load_from_file(std::string path); // Load the vertices from a file
	void unbind(); // Unbind the VBO from the GPU memory
	~VBO(); // VBO destructor

	// Getters and setters
	inline std::vector<Shader_Program_Variable> *get_attributes() { return &attributes; };
	inline std::vector<float> get_base_datas(std::vector<Shader_Program_Variable> a_attributes)
	{
		std::vector<float> a_datas = std::vector<float>();
		a_datas.push_back(0.5f);
		a_datas.push_back(0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(1.0f);
		a_datas.push_back(1.0f);

		if (a_attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(0.5f);
		a_datas.push_back(-0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(1.0f);
		a_datas.push_back(0.0f);

		if (a_attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(-0.5f);
		a_datas.push_back(0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(0.0f);
		a_datas.push_back(1.0f);

		if (a_attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(0.5f);
		a_datas.push_back(-0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(1.0f);
		a_datas.push_back(0.0f);

		if (a_attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(-0.5f);
		a_datas.push_back(-0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(0.0f);
		a_datas.push_back(0.0f);

		if (a_attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(-0.5f);
		a_datas.push_back(0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(0.0f);
		a_datas.push_back(1.0f);

		if (a_attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		return a_datas;
	}
	inline std::vector<float> get_datas() { return datas; };
	inline std::vector<unsigned int> get_indices() { return indices; };
	inline unsigned int* get_indices_in_array() { return indices.data(); };
	inline unsigned int& get_vbo() { return vbo; };
	inline bool is_using_vbo() { return use_ebo; };
private:
	unsigned int ebo = 0; // Handle to the EBO
	unsigned int vbo = 0; // Handle to the VBO

	bool use_ebo = true; // If the VBO use EBO
	std::vector<Shader_Program_Variable> attributes = std::vector<Shader_Program_Variable>(); // Each Shader_Program_Variable in the VBO
protected:
	std::vector<unsigned int> indices = std::vector<unsigned int>(); // Each EBOs in the VBO
	std::vector<float> datas = std::vector<float>(); // Each vertices in the VBO
};

class VAO
{
	// Class representing a VAO interface
public:
	VAO(std::string shader_path, std::vector<Shader_Program_Variable> a_attributes, std::string vbo_path = ""); // VAO constructor
	virtual void bind(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0)); // Bind the VAO into the GPU memory
	Shader_Program *load_shader_program(std::string shader_path); // Load and return a shader
	virtual void render(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0)); // Render the VAO
	unsigned int triangle_number(); // Returns the number of triangle to draw
	~VAO(); // VAO destructor


	// Getters and setters
	inline Shader_Program* get_shader_program() { return shader_program; };
	inline unsigned int& get_vao() { return vao; };
	inline VBO* get_vbo() { return vbo; };
protected:
	unsigned int vao; // Handle to the VAO

	Shader_Program *shader_program = 0; // Pointer to the shader program
	VBO *vbo = 0; // Pointer to the VBO
};

class Font_VAO: public VAO
{
	// Class representing a VAO of a font
public:
	Font_VAO(); // Font_VAO constructor
	void bind(glm::vec4 rect); // Bind the font VAO into the GPU memory
	void render(glm::vec4 rect); // Render the Font_VAO
	~Font_VAO(); // Font_VAO constructor

	// Getters and setters
	inline std::vector<Shader_Program_Variable> get_base_attributes()
	{
		std::vector<Shader_Program_Variable> hud_attributes = std::vector<Shader_Program_Variable>();
		Shader_Program_Variable v1 = Shader_Program_Variable();
		Shader_Program_Variable v2 = Shader_Program_Variable();
		v1.vector_size = 3;
		v2.vector_size = 2;
		hud_attributes.push_back(v1);
		hud_attributes.push_back(v2);

		return hud_attributes;
	};
private:
};

class Texture
{
	// Class representing a texture interface
public:
	Texture(std::string a_texture_path, bool a_resize = true); // Texture constructor
	void bind(); // Bind the texture into the GPU memory
	~Texture(); // Texture destructor

	// Getters and setters
	inline glm::vec2 get_texture_size() { return glm::vec2(width, height); };
	inline std::string get_texture_path() { return texture_path; };
	inline bool use_resize() { return resize; };
private:
	int height = 0; // Height of the texture
	bool resize = true; // If the shader resize the texture or not
	unsigned int texture_id = 0; // Handle to the texture
	std::string texture_path = ""; // Path of the texture
	int width = 0; // Width of the texture
};

class Font_Texture: public Texture
{
	// Class representing a font
public:
	Font_Texture(std::string a_font_texture_path); // Font_Texture constructor
	std::vector<float> get_character_data(char character); // Return the VBO datas for a character
	short get_character_place(char character); // Return the place of a character into the characters string
	glm::vec4 get_character_rect(char character); // Return the rect of the character on the texture
	glm::vec2 size(std::string text); // Return the size of the text
	~Font_Texture(); // Font_Texture destructor

	// Getters and setters
	static std::string get_characters() { return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-*/,;:!?./§\\éèàçù^¨#{]()}^<*%¨> =\'\"_@"; };
	static glm::vec2 get_character_size() { return glm::vec2(105, 200); };
private:
};