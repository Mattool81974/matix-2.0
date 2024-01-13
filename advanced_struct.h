#pragma once

#include "base_struct.h"
#include <map>
#include "model.h"

class Advanced_Struct
{
public:
	Advanced_Struct(Base_Struct *a_base_struct);
	~Advanced_Struct();

	inline std::map<std::string, VAO*> *get_all_vaos() { return &all_vaos; };
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline std::map<std::string, std::string>* get_type() { return &types; };
private:
	Base_Struct* base_struct = 0;
	std::map<std::string, std::string> types = std::map<std::string, std::string>();
	std::map<std::string, VAO*> all_vaos = std::map<std::string, VAO*>();
};