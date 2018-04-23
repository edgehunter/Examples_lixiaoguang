#pragma once

#include"ControlBase.h"

struct MotionVector
{
	//当前帧，位置坐标值（第一个数据点）
	glm::vec3 CurrentPosition;

	//相对上一帧，运动偏移量
	glm::vec3 RelativeValue;

	MotionVector()
	{
		CurrentPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		RelativeValue = glm::vec3(0.0f, 0.0f, 0.0f);
	}

};

struct OpenGLHeader
{
	MotionVector mMotionVector;


	OpenGLHeader()
	{
		mMotionVector = MotionVector();

	}
};


class WorldModel:public ControlBase
{
public:
	WorldModel();
	~WorldModel();

	void Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height);
	void SetRotate(glm::vec3 Rotate);
	void SetScaleFactor(float ScaleFactor);

	void ComputeMatricesFromInputs();
	void UpdateModelMatrix();

	glm::mat4 GetModelMatrix();

	void UpdateMotionCompensation(OpenGLHeader* p_OpenGLHeader);

private:

	//Shader* p_Shader;
	//GLFWwindow* Window;

	//float KeySpeed;// = 3.0f; // 3 units / second
	//float MouseSpeed;// = 0.005f;

	//// Get mouse position
	//double xPos;// = 600 / 2;
	//double yPos;// = 400 / 2;

	//double xPos_old;// = 600 / 2;
	//double yPos_old;// = 400 / 2;


	glm::mat4 ModelMatrix;// = glm::mat4(1.0);

	//Scale by factor 1.0
	float ScaleFactor;
	glm::mat4 ModelScaleMatrix;// = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

	//Translate
	glm::mat4 ModelTranslateMatrix;// = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	//Rotate
	glm::vec3 Rotate;

	glm::mat4 ViewRotateX;
	glm::mat4 ViewRotateY;
	glm::mat4 ViewRotateZ;

	glm::mat4 RotationMatrix;// = glm::rotate(ModelTranslateMatrix, 0.9f, glm::vec3(0.0f, 1.0f, 0.0f)); // where x, y, z is axis of rotation (e.g. 0 1 0)

	//运动补偿，当前帧 保持坐标在原点
	glm::vec3 MotionCompensationValue;

	// 模型运动补偿
	glm::vec3 MotionCompensationValueOffset;

	// 运动补偿因子
	float MotionCompensationFactor;

};

