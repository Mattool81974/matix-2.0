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
	// Define necessary variable size for binding
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

		glVertexAttribPointer(variable_number, it->vector_size, GL_FLOAT, GL_FALSE, total_size, (void*)current_size);
		glEnableVertexAttribArray(variable_number); // Pass each variables to the shader
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

VBO::VBO(std::vector<Shader_Program_Variable> a_attributes, std::vector<float> a_datas, bool a_use_ebo): attributes(a_attributes), datas(a_datas), use_ebo(a_use_ebo) // VBO complete constructor
{
	// Generate the VBO into the GPU memory
	glGenBuffers(1, &vbo);
	if (use_ebo)
	{
		glGenBuffers(1, &ebo);
	}
}

// VBO constructor
VBO::VBO(std::vector<Shader_Program_Variable> a_attributes, bool fill_datas, bool a_use_ebo): VBO(a_attributes, get_base_datas(a_attributes), a_use_ebo)
{
	
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

	// Bind the data into the GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * datas.size(), arr, GL_STATIC_DRAW);

	if(use_ebo) // Bind the EBO if the VBO use them
	{
		unsigned int* arr_2 = get_indices_in_array();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * get_indices().size(), arr_2, GL_STATIC_DRAW);
	}
}

// Returns the number of vertices into the VBO
unsigned int VBO::get_vertice_number()
{
	std::vector<float> datas = get_datas();
	unsigned int attribute_size = 0;
	for(int i = 0;i<get_attributes()->size();i++)
	{
		unsigned int type_size = 0;
		if ((*get_attributes())[i].type == 0) { type_size = sizeof(float); }
		attribute_size += (*get_attributes())[i].vector_size * type_size; // Get the total size of the datas
	}
	return (datas.size() * sizeof(float)) / attribute_size; // Divise the total size with the size of one float
}

// Load the vertices from a file
void VBO::load_from_file(std::string path)
{
	std::string content;
	std::ifstream file;

	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		file.open(path);
		std::stringstream stream;

		// read file's buffer contents into streams
		stream << file.rdbuf();

		// close file handlers
		file.close();

		// convert stream into string
		content = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Matrix game : error ! Can't open the VBO file \"" << path << "\"." << std::endl;
	}

	// Fill the datas
	datas.clear();
	std::vector<std::string> cutted = cut_string(content, " ");
	for (int i = 0; i < cutted.size(); i++)
	{
		datas.push_back(atof(cutted[i].c_str()));
	}
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

// VAO constructor
VAO::VAO(std::string shader_path, std::vector<Shader_Program_Variable> a_attributes, std::string vbo_path)
{
	shader_program = load_shader_program(shader_path); // Load the shader program

	// Create the VAO into the GPU memory
	glGenVertexArrays(1, &vao);
	if (vbo_path != "") // Create the VBO for this VAO
	{
		vbo = new VBO(a_attributes, false, false);
		vbo->load_from_file(vbo_path);
	}
	else
	{
		vbo = new VBO(a_attributes);
	}

	// Bind the VAO and the VBO
	glBindVertexArray(vao);
	vbo->bind_buffer();

	// Pass the "in" variables into the shader program
	shader_program->pass_variable(vbo->get_attributes());

	// Unbind all
	vbo->unbind();
	glBindVertexArray(0);
}

// Bind the VAO into the GPU memory
void VAO::bind(glm::vec3 scale)
{
	shader_program->use();
	shader_program->set_uniform3f_value("scale", scale.x, scale.y, scale.z);
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

	Shader_Program* shader = new Shader_Program(vertex_code, fragment_code); // Create the shader program
	return shader;
}

// Render the VAO
void VAO::render(glm::vec3 scale)
{
	bind(scale);
	if (get_vbo()->is_using_vbo()) // Render the VAO with a different function if the VBO use EBOs or not
	{
		glDrawElements(GL_TRIANGLES, vbo->get_indices().size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, triangle_number() * 3.0);
	}
}

// Returns the number of triangle to draw
unsigned int VAO::triangle_number()
{
	return get_vbo()->get_vertice_number() / 3.0;
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

// Font_Vao constructor
Font_VAO::Font_VAO(): VAO("../shaders/font", get_base_attributes())
{
	
}

// Bind the VAO into the GPU memory
void Font_VAO::bind(glm::vec4 rect)
{
	get_shader_program()->use();
	get_shader_program()->set_uniform4f_value("texture_rect", rect[0], rect[1], rect[2], rect[3]);
	glBindVertexArray(get_vao());
}

// Render the Font_VAO
void Font_VAO::render(glm::vec4 rect)
{
	bind(rect);
	if (get_vbo()->is_using_vbo()) // Render the VAO with a different function if the VBO use EBOs or not
	{
		glDrawElements(GL_TRIANGLES, get_vbo()->get_indices().size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, triangle_number() * 3.0);
	}
}

// Font_VAO destructor
Font_VAO::~Font_VAO()
{
	delete shader_program;
	shader_program = 0;
	delete vbo;
	vbo = 0;
	glDeleteVertexArrays(1, &vao);
}

// Texture constructor
Texture::Texture(std::string a_texture_path, bool a_resize): texture_path(a_texture_path), resize(a_resize)
{
	int nrChannels = 0;
	unsigned char* texture = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);

	// Load the texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Good
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Good

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

// Font_Texture constructor
Font_Texture::Font_Texture(std::string a_font_texture_path): Texture(a_font_texture_path, false)
{

}

// Return the VBO datas for a character
std::vector<float> Font_Texture::get_character_data(char character)
{
	glm::vec4 rect = get_character_rect(character);

	std::vector<float> a_datas = std::vector<float>();
	a_datas.push_back(0.5f);
	a_datas.push_back(0.5f);
	a_datas.push_back(0.0f);

	a_datas.push_back(rect[0] + rect[2]);
	a_datas.push_back(rect[1] + rect[3]);

	a_datas.push_back(0.5f);
	a_datas.push_back(-0.5f);
	a_datas.push_back(0.0f);

	a_datas.push_back(rect[0] + rect[2]);
	a_datas.push_back(rect[1]);

	a_datas.push_back(-0.5f);
	a_datas.push_back(-0.5f);
	a_datas.push_back(0.0f);

	a_datas.push_back(rect[0]);
	a_datas.push_back(rect[1]);

	a_datas.push_back(-0.5f);
	a_datas.push_back(0.5f);
	a_datas.push_back(0.0f);

	a_datas.push_back(rect[0]);
	a_datas.push_back(rect[1] + rect[3]);

	return a_datas;
}

// Return the place of a character into the characters string
short Font_Texture::get_character_place(char character)
{
	std::string characters = get_characters();
	// Browse each characters
	for (int i = 0; i < characters.size(); i++) { if (characters[i] == character) return i; }
	return -1; // If the character isn't here
}

// Return the rect of the character on the texture
glm::vec4 Font_Texture::get_character_rect(char character)
{
	short place = get_character_place(character);
	if (place == -1) { std::cout << "Matix game : error ! The character \"" << character << "\" doesn't exists in the font \"" << get_texture_path() << "\"." << std::endl; return glm::vec4(0, 0, 0, 0); }

	return glm::vec4((place % (int)glm::round(get_texture_size()[0] / get_character_size()[0])) * get_character_size()[0], glm::floor(place / (get_texture_size()[0] / get_character_size()[0])) * get_character_size()[0], get_character_size()[0], get_character_size()[1]);
}

// Font_Texture destructor
Font_Texture::~Font_Texture()
{

}