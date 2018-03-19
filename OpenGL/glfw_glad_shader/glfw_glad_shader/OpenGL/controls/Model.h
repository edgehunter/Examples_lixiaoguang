#pragma once

#include <stdio.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:
	Model();
	~Model();

	void Init(GLFWwindow* Window, const unsigned int Screen_Width, const unsigned int Screen_Height);
	void SetRotate(glm::vec3 Rotate);
	void SetScaleFactor(float ScaleFactor);
	glm::mat4 GetModelMatrix();

	void ComputeMatricesFromInputs();


private:

	GLFWwindow* Window;

	float KeySpeed;// = 3.0f; // 3 units / second
	float MouseSpeed;// = 0.005f;

	// Get mouse position
	double xPos;// = 600 / 2;
	double yPos;// = 400 / 2;

	double xPos_old;// = 600 / 2;
	double yPos_old;// = 400 / 2;


	glm::mat4 ModelMatrix;// = glm::mat4(1.0);

	//Scale by factor 1.0
	float ScaleFactor;
	glm::mat4 ModelScaleMatrix;// = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

	//Translate
	glm::mat4 ModelTranslateMatrix;// = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	//Rotate
	glm::vec3 Rotate;

	glm::mat4 ViewRotateX;
	glm::mat4 ViewRotateY;
	glm::mat4 ViewRotateZ;

	glm::mat4 RotationMatrix;// = glm::rotate(ModelTranslateMatrix, 0.9f, glm::vec3(0.0f, 1.0f, 0.0f)); // where x, y, z is axis of rotation (e.g. 0 1 0)



};

