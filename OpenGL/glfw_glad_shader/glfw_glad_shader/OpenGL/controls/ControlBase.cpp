#include "ControlBase.h"


ControlBase::ControlBase()
{
}


ControlBase::~ControlBase()
{
}


void ControlBase::Init(Shader* p_Shader, GLFWwindow* Window, const unsigned int Screen_Width, const unsigned int Screen_Height)
{
	this->p_Shader = p_Shader;
	this->Window = Window;

	KeySpeed = 40.0f; // 3 units / second
	MouseSpeed = 0.005f;

	// Get mouse position
	xPos = Screen_Width / 2;
	yPos = Screen_Height / 2;

	xPos_old = Screen_Width / 2;
	yPos_old = Screen_Height / 2;

}