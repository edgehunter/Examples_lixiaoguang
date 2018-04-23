#include "MouseClick.h"
//#include <GL\glu.h>


MouseClick::MouseClick()
{
}


MouseClick::~MouseClick()
{
}

void MouseClick::Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height)
{
	ControlBase::Init(p_Shader, Window, Screen_Width, Screen_Height);

	Neighborhood = 4;

	winZ = new float[(Neighborhood*Neighborhood)];
}

void MouseClick::Click(glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix)
{

	// GLFW_MOUSE_BUTTON_RIGHT
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		this->ModelMatrix = ModelMatrix;
		this->ViewMatrix = ViewMatrix;
		this->ProjectionMatrix = ProjectionMatrix;

		
		// GetWindowSize
		glfwGetWindowSize(Window, &Screen_Width, &Screen_Height);
		
		// GetCursorPos
		glfwGetCursorPos(Window, &xPos, &yPos);
		winX = (float)xPos;
		winY = (float)(Screen_Height - yPos);

		glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, winZ);
		
		if (winZ[0] != 1.0f)
		{
			Unprojection(glm::vec3(winX, winY, winZ[0]));
		} 
		else
		{
			//returns values from each pixel with lower left corner at(x + i, y + j) for 0 ¡Ü i < width and 0 ¡Ü j < height.This pixel is said to be the ith pixel in the jth row.
			//Pixels are returned in row order from the lowest to the highest row, left to right in each row.
			glReadPixels((int)(winX - Neighborhood / 2), (int)(winY - Neighborhood / 2), Neighborhood, Neighborhood, GL_DEPTH_COMPONENT, GL_FLOAT, winZ);

			glm::vec3 ScreenPos = glm::vec3(1.0f, 1.0f, 1.0f);
			for (size_t i = 0; i < Neighborhood; i++)
			{
				for (size_t j = 0; j < Neighborhood; j++)
				{
					if (winZ[i*Neighborhood + j]<ScreenPos.z)
					{
						ScreenPos.x = i;
						ScreenPos.y = j;
						ScreenPos.z = winZ[i*Neighborhood + j];
					}

				}				
			}

			if (ScreenPos.z !=1.0f)
			{
				ScreenPos.x += winX - Neighborhood / 2;
				ScreenPos.y += winY - Neighborhood / 2;
				Unprojection(ScreenPos);
			}
			else
			{
				printf("MouseClick Pick Error !!!!!! \n");
			}
		}
		
	}
}

glm::vec3 MouseClick::Unprojection(glm::vec3 ScreenPos)
{
	//glm::vec3 ScreenPos = glm::project(original, ModelMatrix*ViewMatrix, ProjectionMatrix, glm::vec4(0.0f, 0.0f, float(Screen_Width), float(Screen_Height)));

	glm::vec3 posVec = glm::unProject(
		ScreenPos,// glm::vec3(winX, winY, winZ[1]),
		ModelMatrix*ViewMatrix,
		ProjectionMatrix,
		glm::vec4(0.0f, 0.0f, float(Screen_Width), float(Screen_Height))
		);

		printf("MouseClick Pick (%f, %f, %f) (%f, %f, %f) \n", ScreenPos.x, ScreenPos.y, ScreenPos.z, posVec.x, posVec.y, posVec.z);
		
		return posVec;
}

void MouseClick::Release()
{
	delete winZ;
}