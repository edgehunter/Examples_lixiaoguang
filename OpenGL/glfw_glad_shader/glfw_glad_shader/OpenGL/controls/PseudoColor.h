#pragma once

#include"ControlBase.h"


class PseudoColor :public ControlBase
{
public:
	PseudoColor();
	~PseudoColor();

	void Init(Shader* p_Shader, GLFWwindow* Window, const unsigned int Screen_Width, const unsigned int Screen_Height);


	// update shader uniform TransparencyColor
	void UpdateThreshold();

	void UpdatePseudoColor();

private:
		
	float ThresholdValue;

	int PseudoColorIndex;
	float PseudoColorStride;
};

