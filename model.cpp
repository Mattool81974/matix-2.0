#include "model.h"
#include "base_struct.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include "matix/stb_image.h"

// Shader_Program constructor
Shader_Program::Shader_Program(std::string a_vertex_shader, std::string a_fragment_shader): fragment_shader(a_fragment_shader), vertex_shader(a_vertex_shader)
{
	// Convert the fragment and vertex shader (string) to an char*
	char* char_array_fragment = new char[get_fragment_shader().length() + 1];
	char_array_fragment[get_fragment_shader().length()] = '\0';
	for (int i = 0; i < get_fragment_shader().length(); i++) { char_array_fragment[i] = get_fragment_shader()[i]; }
	char* char_array_vertex = new char[get_vertex_shader().length() + 1];
	char_array_vertex[get_vertex_shader().length()] = '\0';
	for (int i = 0; i < get_vertex_shader().length(); i++) { char_array_vertex[i] = get_vertex_shader()[i]; }

	// Pass the fragment shader into the GPU and compile it
	unsigned int fragment = 0;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &char_array_fragment, NULL);
	glCompileShader(fragment);
	delete[] char_array_fragment;

	// Pass the vertex shader into the GPU and compile it
	unsigned int vertex = 0;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &char_array_vertex, NULL);
	glCompileShader(vertex);
	delete[] char_array_vertex;

	// Check if the shader compilation gave an error
	int  success = 0;
	char infoLog[512];
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	success = 0;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Generate the shader program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex);
	glAttachShader(shader_program, fragment);
	glLinkProgram(shader_program);

	// Check if the shader program didn't send an error
	success = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Delete useless ressources
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Pass variable to the shader program
void Shader_Program::pass_variable(std::vector<Shader_Program_Variable> *variables)
{
	// Define necessary variable for binding
	unsigned int total_size = 0;
	for (std::vector<Shader_Program_Variable>::iterator it = variables->begin(); it != variables->end(); it++)
	{
		unsigned int type_size = 0;
		if (it->type == 0) { type_size = sizeof(float); }
		total_size += it->vector_size * type_size;
	}
	unsigned int current_size = 0;
	unsigned short variable_number = 0;

	use();
	for (std::vector<Shader_Program_Variable>::iterator it = variables->begin(); it != variables->end(); it++)
	{
		unsigned int type_size = 0;
		if (it->type == 0) { type_size = sizeof(float); }

		glVertexAttribPointer(variable_number, 3, GL_FLOAT, GL_FALSE, total_size, (void*)current_size);
		glEnableVertexAttribArray(variable_number);
		current_size += it->vector_size * type_size;
		variable_number++;
	}
}

// Change the value of a uniform float value
void Shader_Program::set_uniform1f_value(std::string name, float v1)
{
	int uniform_location = glGetUniformLocation(shader_program, name.c_str());
	use();
	glUniform1f(uniform_location, v1);
}

// Change the value of a uniform vec2 float value
void Shader_Program::set_uniform2f_value(std::string name, float v1, float v2)
{
	int uniform_location = glGetUniformLocation(shader_program, name.c_str());
	use();
	glUniform2f(uniform_location, v1, v2);
}

// Change the value of a uniform vec3 float value
void Shader_Program::set_uniform3f_value(std::string name, float v1, float v2, float v3)
{
	int uniform_location = glGetUniformLocation(shader_program, name.c_str());
	use();
	glUniform3f(uniform_location, v1, v2, v3);
}

// Change the value of a uniform vec4 float value
void Shader_Program::set_uniform4f_value(std::string name, float v1, float v2, float v3, float v4)
{
	int uniform_location = glGetUniformLocation(shader_program, name.c_str());
	use();
	glUniform4f(uniform_location, v1, v2, v3, v4);
}

// Change the value of a matrix mat4 float value
void Shader_Program::set_uniform4fv_value(std::string name, glm::mat4 fv)
{
	use();
	int uniform_location = glGetUniformLocation(shader_program, name.c_str());
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(fv));
}

// Start using the shader
void Shader_Program::use()
{
	glUseProgram(shader_program);
}

// Shader_Program destructor
Shader_Program::~Shader_Program()
{
	glDeleteProgram(shader_program);
}

// VBO constructor
VBO::VBO(bool fill_datas, bool a_use_ebo): use_ebo(a_use_ebo)
{
	if(fill_datas)
	{
		vertices.push_back(0.5f);
		vertices.push_back(0.5f);
		vertices.push_back(0.0f);

		vertices_texture.push_back(1.0f);
		vertices_texture.push_back(1.0f);

		vertices.push_back(0.5f);
		vertices.push_back(-0.5f);
		vertices.push_back(0.0f);

		vertices_texture.push_back(1.0f);
		vertices_texture.push_back(0.0f);

		vertices.push_back(-0.5f);
		vertices.push_back(-0.5f);
		vertices.push_back(0.0f);

		vertices_texture.push_back(0.0f);
		vertices_texture.push_back(0.0f);

		vertices.push_back(-0.5f);
		vertices.push_back(0.5f);
		vertices.push_back(0.0f);

		vertices_texture.push_back(0.0f);
		vertices_texture.push_back(1.0f);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);

		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
	}

	Shader_Program_Variable v1 = Shader_Program_Variable();
	Shader_Program_Variable v2 = Shader_Program_Variable();
	v1.vector_size = 3;
	v2.vector_size = 2;
	attributes.push_back(v1);
	attributes.push_back(v2);

	glGenBuffers(1, &vbo);
	if(use_ebo)
	{
		glGenBuffers(1, &ebo);
	}
}

// Bind the VBO into the GPU memory
void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

// Bind the buffer data of the VBO
void VBO::bind_buffer()
{
	std::vector<float> datas = get_datas();
	float* arr = datas.data();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * datas.size(), arr, GL_STATIC_DRAW);

	if(use_ebo)
	{
		unsigned int* arr_2 = get_indices_in_array();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * get_indices().size(), arr_2, GL_STATIC_DRAW);
	}
}

// Return all the data into the VBO
std::vector<float> VBO::get_datas()
{
	std::vector<float> datas;

	for (int i = 0; i < get_vertices().size() / 3; i++)
	{
		datas.push_back(get_vertices()[i * 3]);
		datas.push_back(get_vertices()[i * 3 + 1]);
		datas.push_back(get_vertices()[i * 3 + 2]);
		datas.push_back(get_vertices_texture()[i * 2]);
		datas.push_back(get_vertices_texture()[i * 2 + 1]);
	}

	return datas;
}

// Unbind the VBO from the GPU memory
void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VBO destructor
VBO::~VBO()
{
	glDeleteBuffers(1, &vbo);
}

// Cube_VBO constructor
Cube_VBO::Cube_VBO(): VBO(false, false)
{
	float vertices_array[] = {
	// Face 1
	-0.5f, -0.5f, -0.5f,  0.33333f, 0.25f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.25f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
	-0.5f,  0.5f, -0.5f,  0.33333f, 0.5f,
	-0.5f, -0.5f, -0.5f,  0.33333f, 0.25f,

	// Face 3
	-0.5f, -0.5f,  0.5f,  0.66666f, 0.25f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.25f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.5f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.5f,
	-0.5f,  0.5f,  0.5f,  0.66666f, 0.5f,
	-0.5f, -0.5f,  0.5f,  0.66666f, 0.25f,


	// Face 2
	-0.5f, -0.5f, -0.5f,  0.33333f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.66666f, 0.25f,
	-0.5f,  0.5f, -0.5f,  0.33333f, 0.25f,
	-0.5f, -0.5f,  0.5f,  0.66666f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.66666f, 0.25f,
	-0.5f, -0.5f, -0.5f,  0.33333f, 0.0f,


	// Face 4
	 0.5f,  0.5f,  0.5f,  0.33333f, 0.75f,
	 0.5f,  0.5f, -0.5f,  0.66666f, 0.75f,
	 0.5f, -0.5f, -0.5f,  0.66666f, 0.5f,
	 0.5f, -0.5f, -0.5f,  0.66666f, 0.5f,
	 0.5f, -0.5f,  0.5f,  0.33333f, 0.5f,
	 0.5f,  0.5f,  0.5f,  0.33333f, 0.75f,


	 // Face 6
	-0.5f, -0.5f, -0.5f,  0.66666f, 0.5f,
	 0.5f, -0.5f, -0.5f,  0.33333f, 0.5f,
	 0.5f, -0.5f,  0.5f,  0.33333f, 0.25f,
	 0.5f, -0.5f,  0.5f,  0.33333f, 0.25f,
	-0.5f, -0.5f,  0.5f,  0.66666f, 0.25f,
	-0.5f, -0.5f, -0.5f,  0.66666f, 0.5f,

	// Face 5
	-0.5f,  0.5f, -0.5f,  0.66666f, 0.75f,
	 0.5f,  0.5f, -0.5f,  0.33333f, 0.75f,
	 0.5f,  0.5f,  0.5f,  0.33333f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.33333f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.66666f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.66666f, 0.75f
	};

	for (int i = 0; i < 36; i++)
	{
		vertices.push_back(vertices_array[i * 5]);
		vertices.push_back(vertices_array[i * 5 + 1]);
		vertices.push_back(vertices_array[i * 5 + 2]);

		vertices_texture.push_back(vertices_array[i * 5 + 3]);
		vertices_texture.push_back(vertices_array[i * 5 + 4]);
	}
}

// Cube_VBO destructor
Cube_VBO::~Cube_VBO()
{

}

// VAO constructor
VAO::VAO(std::string shader_path, std::string type)
{
	shader_program = load_shader_program(shader_path);

	glGenVertexArrays(1, &vao);
	if (type == "cube")
	{
		vbo = new Cube_VBO();
	}
	else
	{
		vbo = new VBO();
	}

	glBindVertexArray(vao);
	vbo->bind_buffer();

	shader_program->pass_variable(vbo->get_attributes());

	vbo->unbind();
	glBindVertexArray(0);
}

// Bind the VAO into the GPU memory
void VAO::bind()
{
	shader_program->use();
	shader_program->set_uniform4f_value("ourColor", 0, 0, (1.0 - 1.0 * sin(glfwGetTime())) / 2.0, 0);
	glBindVertexArray(vao);
}

// Load and return a shader
Shader_Program* VAO::load_shader_program(std::string shader_path)
{
	std::string vertex_content;
	std::string fragment_content;
	std::ifstream vertex_file;
	std::ifstream fragment_file;

	// ensure ifstream objects can throw exceptions:
	vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertex_file.open(shader_path + ".vert");
		fragment_file.open(shader_path + ".frag");
		std::stringstream vertex_stream, fragment_stream;

		// read file's buffer contents into streams
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		// close file handlers
		vertex_file.close();
		fragment_file.close();

		// convert stream into string
		vertex_content = vertex_stream.str();
		fragment_content = fragment_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vertex_code = vertex_content.c_str();
	const char* fragment_code = fragment_content.c_str();

	Shader_Program* shader = new Shader_Program(vertex_code, fragment_code);
	return shader;
}

// Render the VAO
void VAO::render()
{
	bind();
	if (get_vbo()->is_using_vbo())
	{
		glDrawElements(GL_TRIANGLES, vbo->get_indices().size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, get_vbo()->get_vertices().size() / 3.0);
	}
}

// VAO destructor
VAO::~VAO()
{
	delete shader_program;
	shader_program = 0;
	delete vbo;
	vbo = 0;
	glDeleteVertexArrays(1, &vao);
}

// Texture constructor
Texture::Texture(std::string a_texture_path): texture_path(a_texture_path)
{
	int nrChannels = 0;
	unsigned char* texture = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);

	// Load the texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texture);
}

// Bind the texture into the GPU memory
void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

// Texture destructor
Texture::~Texture()
{

}