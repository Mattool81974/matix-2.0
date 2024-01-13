#pragma once

#include "base_struct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class Shader_Program
{
public:
	Shader_Program(std::string a_vertex_shader, std::string a_fragment_shader);
	void pass_variable();
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
	std::vector<float> get_datas();
	void unbind();
	~VBO();

	inline std::vector<unsigned int> get_indices() { return indices; };
	inline unsigned int* get_indices_in_array() { return indices.data(); };
	inline unsigned int& get_vbo() { return vbo; };
	inline std::vector<float> get_vertices() { return vertices; };
	inline float* get_vertices_in_array() { return vertices.data(); };
	inline std::vector<float> get_vertices_texture() { return vertices_texture; };
	inline bool is_using_vbo() { return use_ebo; };
private:
	unsigned int ebo = 0;
	unsigned int vbo = 0;

	bool use_ebo = true;
protected:
	std::vector<unsigned int> indices = std::vector<unsigned int>();
	std::vector<float> vertices = std::vector<float>();
	std::vector<float> vertices_texture = std::vector<float>();
};

class Cube_VBO : public VBO
{
public:
	Cube_VBO();
	~Cube_VBO();
private:
};

class VAO
{
public:
	VAO(std::string shader_path);
	void bind();
	Shader_Program *load_shader_program(std::string shader_path);
	void render();
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
	Texture(std::string a_texture_path);
	void bind();
	~Texture();
private:
	int height = 0;
	unsigned int texture_id = 0;
	std::string texture_path = "";
	int width = 0;
};