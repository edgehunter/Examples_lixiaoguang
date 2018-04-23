#pragma once

#include <stdio.h>

//shader
#include "../shader/shader.h"

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ControlBase
{
public:
	ControlBase();
	~ControlBase();

	void Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height);

protected:

	Shader* p_Shader;
	GLFWwindow* Window;

	float KeySpeed;// = 3.0f; // 3 units / second
	float MouseSpeed;// = 0.005f;

	// Get mouse position
	double xPos;
	double yPos;

	double xPos_old;
	double yPos_old;

	int Screen_Width;
	int Screen_Height;
};

