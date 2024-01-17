#pragma once

#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

struct Shader_Program_Variable
{
	bool normalized = false;
	unsigned short type = 0; // 0 = GL_FLOAT
	unsigned short vector_size = 1;
};

class Shader_Program
{
public:
	Shader_Program(std::string a_vertex_shader, std::string a_fragment_shader);
	void pass_variable(std::vector<Shader_Program_Variable> *variables);
	void set_uniform1f_value(std::string name, float v1);
	void set_uniform2f_value(std::string name, float v1, float v2);
	void set_uniform3f_value(std::string name, float v1, float v2, float v3);
	void set_uniform4f_value(std::string name, float v1, float v2, float v3, float v4);
	void set_uniform4fv_value(std::string name, glm::mat4 fv);
	void use();
	~Shader_Program();

	inline std::string get_fragment_shader() const { return fragment_shader; };
	inline std::string get_vertex_shader() const { return vertex_shader; };
private:
	unsigned int shader_program = 0;

	std::string fragment_shader = "";
	std::string vertex_shader = "";
};

class VBO
{
public:
	VBO(bool fill_datas = true, bool a_use_ebo = true);
	void bind();
	void bind_buffer();
	unsigned int get_vertice_number();
	void load_from_file(std::string path);
	void unbind();
	~VBO();

	inline std::vector<Shader_Program_Variable> *get_attributes() { return &attributes; };
	inline std::vector<float> get_datas() { return datas; };
	inline std::vector<unsigned int> get_indices() { return indices; };
	inline unsigned int* get_indices_in_array() { return indices.data(); };
	inline unsigned int& get_vbo() { return vbo; };
	inline bool is_using_vbo() { return use_ebo; };
private:
	unsigned int ebo = 0;
	unsigned int vbo = 0;

	bool use_ebo = true;
	std::vector<Shader_Program_Variable> attributes = std::vector<Shader_Program_Variable>();
protected:
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	std::vector<float> datas = std::vector<float>();
};

class VAO
{
public:
	VAO(std::string shader_path, std::string vbo_path = "");
	void bind(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
	Shader_Program *load_shader_program(std::string shader_path);
	void render(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
	unsigned int triangle_number();
	~VAO();

	inline Shader_Program* get_shader_program() { return shader_program; };
	inline unsigned int& get_vao() { return vao; };
	inline VBO* get_vbo() { return vbo; };
private:
	unsigned int vao;

	Shader_Program *shader_program = 0;
	VBO *vbo = 0;
};

class Texture
{
public:
	Texture(std::string a_texture_path, bool a_resize = true);
	void bind();
	~Texture();

	inline bool use_resize() { return resize; };
private:
	int height = 0;
	bool resize = true;
	unsigned int texture_id = 0;
	std::string texture_path = "";
	int width = 0;
};