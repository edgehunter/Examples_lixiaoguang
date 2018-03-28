#include "WorldModel.h"


WorldModel::WorldModel()
{
}


WorldModel::~WorldModel()
{
}


void WorldModel::Init(Shader* p_Shader, GLFWwindow* Window, const unsigned int Screen_Width, const unsigned int Screen_Height)
{
	this->p_Shader = p_Shader;
	this->Window = Window;

	KeySpeed = 3.0f; // 3 units / second
	MouseSpeed = 0.005f;

	// Get mouse position
	xPos = Screen_Width / 2;
	yPos = Screen_Height / 2;

	xPos_old = Screen_Width / 2;
	yPos_old = Screen_Height / 2;


	//Scale by factor 1.0
	ScaleFactor = 1.0f;
	ModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(ScaleFactor));

	//Translate
	ModelTranslateMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	Rotate = glm::vec3(0.0f, 0.0f, 0.0f);

}

void WorldModel::SetRotate(glm::vec3 Rotate)
{
	this->Rotate = Rotate;
}


void WorldModel::SetScaleFactor(float ScaleFactor)
{
	this->ScaleFactor = ScaleFactor;
}

glm::mat4 WorldModel::GetModelMatrix()
{

	return ModelMatrix;
}

void WorldModel::UpdateModelMatrix()
{

	p_Shader->setMat4("WorldModel", ModelMatrix);
}

void WorldModel::ComputeMatricesFromInputs()
{

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	//printf("deltaTime=(%f) ", deltaTime);

	glfwGetCursorPos(Window, &xPos, &yPos);

	// Compute new orientation
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		Rotate.y -= MouseSpeed * float(xPos_old - xPos);
		Rotate.x -= MouseSpeed * float(yPos_old - yPos);

		//printf("Rotate=(%f,%f,%f) \n", Rotate.x, Rotate.y, Rotate.z);
	}

	if (glfwGetKey(Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Rotate.z -= MouseSpeed * float(xPos_old - xPos);

		//printf("Rotate=(%f,%f,%f) \n", Rotate.x, Rotate.y, Rotate.z);
	}


	xPos_old = xPos, yPos_old = yPos;
















	//Scale
	ModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(ScaleFactor));

	//Translate
	ModelTranslateMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	ViewRotateX = glm::rotate(
		ModelTranslateMatrix,
		Rotate.x,
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	ViewRotateY = glm::rotate(
		ViewRotateX,
		Rotate.y,
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	ViewRotateZ = glm::rotate(
		ViewRotateY,
		Rotate.z,
		glm::vec3(0.0f, 0.0f, 1.0f)
		);

	RotationMatrix = ViewRotateZ;//glm::rotate(ModelTranslateMatrix, 0.9f, glm::vec3(0.0f, 1.0f, 0.0f)); // where x, y, z is axis of rotation (e.g. 0 1 0)

	ModelMatrix = RotationMatrix*ModelScaleMatrix;

}