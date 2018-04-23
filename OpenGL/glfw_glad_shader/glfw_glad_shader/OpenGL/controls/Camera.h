#pragma once

#include"ControlBase.h"

class Camera:public ControlBase
{
public:
	Camera();
	~Camera();
	
	void Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height);

	glm::mat4 GetViewMatrix();
	void UpdateViewMatrix();

	glm::mat4 GetProjectionMatrix();
	void UpdateProjectionMatrix();

	void ComputeMatricesFromInputs();

	void UpdataViewPort();

private:

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	// Initial position : on +Z
	glm::vec3 Position;
	
	// Initial horizontal angle : toward -Z
	float HorizontalAngle;// = -3.14f;
	
	// Initial vertical angle : none
	float VerticalAngle;// = 0.0f;
	
	// Initial Field of View
	float InitialFoV;// = 45.0f;

	float Zoom;

	int SCR_Left, SCR_Top;
};

