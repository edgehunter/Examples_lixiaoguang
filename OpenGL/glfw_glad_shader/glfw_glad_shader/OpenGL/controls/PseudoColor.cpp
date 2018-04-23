#include "PseudoColor.h"


PseudoColor::PseudoColor()
{
}


PseudoColor::~PseudoColor()
{
}

void PseudoColor::Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height)
{

	ControlBase::Init(p_Shader, Window, Screen_Width, Screen_Height);

	IsStrengthPseudoColor = false;
	ThresholdValue = 0.0f;
	PseudoColorIndex = 0;

	AltitudePseudoColorStride = 50.0f;
	StrengthPseudoColorStride = 20.0f;
}

void PseudoColor::UpdateThreshold()
{

	// GLFW_KEY_UP
	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		ThresholdValue += 1.0f;
	}

	// GLFW_KEY_DOWN
	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		ThresholdValue -= 1.0f;
	}

	if (ThresholdValue < 0.0f)
	{
		ThresholdValue = 0.0f;
	}

	// TransparencyColor uniforms
	p_Shader->setFloat("ThresholdValue", ThresholdValue);
	
}

void PseudoColor::UpdatePseudoColor()
{
	// GLFW_KEY_KP_0
	if (glfwGetKey(Window, GLFW_KEY_KP_0) == GLFW_PRESS)
	{
		PseudoColorIndex = 0;
		
	}

	// GLFW_KEY_KP_1
	if (glfwGetKey(Window, GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		PseudoColorIndex = 1;

	}

	// GLFW_KEY_KP_2
	if (glfwGetKey(Window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		PseudoColorIndex = 2;

	}

	// GLFW_KEY_KP_3
	if (glfwGetKey(Window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		PseudoColorIndex = 3;

	}

	// GLFW_KEY_KP_4
	if (glfwGetKey(Window, GLFW_KEY_KP_4) == GLFW_PRESS)
	{
		PseudoColorIndex = 4;

	}

	// GLFW_KEY_KP_5
	if (glfwGetKey(Window, GLFW_KEY_KP_5) == GLFW_PRESS)
	{
		PseudoColorIndex = 5;
	
	}

	// GLFW_KEY_KP_ADD
	if (glfwGetKey(Window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
	{
		if (IsStrengthPseudoColor)
		{
			StrengthPseudoColorStride += 10.0f;
		}
		else
		{
			AltitudePseudoColorStride += 1.0f;
		}
		
	}

	// GLFW_KEY_KP_SUBTRACT
	if (glfwGetKey(Window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
	{
		if (IsStrengthPseudoColor)
		{
			StrengthPseudoColorStride -= 10.0f;
		}
		else
		{
			AltitudePseudoColorStride -= 1.0f;
		}
		

	}

	// GLFW_KEY_KP_MULTIPLY
	if (glfwGetKey(Window, GLFW_KEY_KP_MULTIPLY) == GLFW_PRESS)
	{
		IsStrengthPseudoColor = true;
	}

	// GLFW_KEY_KP_DIVIDE
	if (glfwGetKey(Window, GLFW_KEY_KP_DIVIDE) == GLFW_PRESS)
	{
		IsStrengthPseudoColor = false;
	}
	
	p_Shader->setInt("PseudoColorIndex", PseudoColorIndex);
	p_Shader->setFloat("AltitudePseudoColorStride", AltitudePseudoColorStride);
	p_Shader->setFloat("StrengthPseudoColorStride", StrengthPseudoColorStride);
	p_Shader->setBool("IsStrengthPseudoColor", IsStrengthPseudoColor);
}