#pragma once

#include"ControlBase.h"


class PseudoColor :public ControlBase
{
public:
	PseudoColor();
	~PseudoColor();

	void Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height);


	// update shader uniform TransparencyColor
	void UpdateThreshold();

	void UpdatePseudoColor();

private:
		
	bool IsStrengthPseudoColor;
	float ThresholdValue;

	int PseudoColorIndex;
	float AltitudePseudoColorStride;

	float StrengthPseudoColorStride;
};

