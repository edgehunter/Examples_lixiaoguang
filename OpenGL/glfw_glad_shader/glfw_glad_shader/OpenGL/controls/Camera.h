#pragma once

#include <stdio.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();
	
	void Init(GLFWwindow* Window, const unsigned int Screen_Width, const unsigned int Screen_Height);

	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix();

	void ComputeMatricesFromInputs();

private:

	GLFWwindow* Window;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	// Initial position : on +Z
	glm::vec3 Position;// = glm::vec3(0, 0, 5);
	
	// Initial horizontal angle : toward -Z
	float HorizontalAngle;// = -3.14f;
	
	// Initial vertical angle : none
	float VerticalAngle;// = 0.0f;
	
	// Initial Field of View
	float InitialFoV;// = 45.0f;

	float Zoom;

	float KeySpeed;// = 3.0f; // 3 units / second
	float MouseSpeed;// = 0.005f;

	// Get mouse position
	double xPos;// = 600 / 2;
	double yPos;// = 400 / 2;

	double xPos_old;// = 600 / 2;
	double yPos_old;// = 400 / 2;

};

