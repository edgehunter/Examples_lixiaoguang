#pragma once

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

	void Init();
	void Release();

	void Render();

private:

	unsigned int VBO[6], VAO[2];
	
	float *VerticesCubeEngine;
	float *ColorsCubeEngine;
};

