#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

class Shader
{
public:
	Shader();
	~Shader();

	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

private:

	GLuint ProgramID;
};

