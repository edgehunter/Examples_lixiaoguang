#include "WorldModel.h"


WorldModel::WorldModel()
{
}


WorldModel::~WorldModel()
{
}


void WorldModel::Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height)
{

	ControlBase::Init(p_Shader, Window, Screen_Width, Screen_Height);

	//Scale by factor 1.0
	ScaleFactor = 1.0f;
	ModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(ScaleFactor));

	//Translate
	ModelTranslateMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	Rotate = glm::vec3(0.0f, 0.0f, 0.0f);

	MotionCompensationValue = glm::vec3(0.0f, 0.0f, 0.0f);
	MotionCompensationFactor = 1.0f;

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
		Rotate.z -= MouseSpeed * float(yPos_old - yPos);

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


void WorldModel::UpdateMotionCompensation(OpenGLHeader* p_OpenGLHeader)
{

	// GLFW_KEY_RIGHT
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		MotionCompensationFactor += 0.01f;
	}

	// GLFW_KEY_LEFT
	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		MotionCompensationFactor -= 0.01f;
	}

	MotionCompensationValue = p_OpenGLHeader->mMotionVector.CurrentPosition* MotionCompensationFactor;
	MotionCompensationValueOffset = MotionCompensationValue - p_OpenGLHeader->mMotionVector.CurrentPosition;

	p_Shader->setVec3("MotionCompensation", MotionCompensationValue);
	p_Shader->setVec3("MotionCompensationOffset", MotionCompensationValueOffset);
}