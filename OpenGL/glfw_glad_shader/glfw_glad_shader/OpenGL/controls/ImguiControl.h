#pragma once

#include"ControlBase.h"

// imgui
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"


class ImguiControl :public ControlBase
{
public:
	ImguiControl();
	~ImguiControl();

	void Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height);
	void Render();

	void Shutdown();

private:

	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

};

