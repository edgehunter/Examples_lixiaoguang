#pragma once

// glad
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

class AuxiliaryModel
{
public:
	AuxiliaryModel();
	~AuxiliaryModel();

	void Init();
	bool Release();

	// 渲染当前VAO
	void RanderVAO();


private:

	// 默认使用VAO1个，EBO使用1个，VBO使用2个
	unsigned int VBO[2], EBO[1], VAO[1];
};

