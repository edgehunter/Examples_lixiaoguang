#pragma once
#include"ControlBase.h"

class MouseClick :public ControlBase
{
public:
	MouseClick();
	~MouseClick();

	void Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height);


	void Click(glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
	glm::vec3 Unprojection(glm::vec3 ScreenPos);

	void Release();

private:

	// ÁÚÓò¸öÊýNeighborhood*Neighborhood
	int Neighborhood;

	float  winX, winY;
	float* winZ;

	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

};

