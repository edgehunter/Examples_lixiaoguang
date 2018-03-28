#pragma once

//shader
#include "OpenGL/shader/shader.h"

// glad
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;



class VaoEngine
{
public:
	VaoEngine();
	~VaoEngine();

	void Init(Shader* p_Shader);
	void Release();

	void Render();

private:

	unsigned int VBO[9], VAO[3];

	Shader* p_Shader;


	int pointNum;// = 128 * 128;
	float *VerticesPoints;
	float *ColorsPoints;
};

