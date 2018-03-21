#pragma once

// vector
#include <vector>

// glad
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

//当前帧 视线-z，四个边界点
struct EdgePoint
{
	//-x,-y 左下角（视线-z）
	glm::vec3 NxNy;

	//-x,+y 左上角（视线-z）
	glm::vec3 NxPy;

	//+x,-y 右下角（视线-z）
	glm::vec3 PxNy;

	//+x,+y 右上角（视线-z）
	glm::vec3 PxPy;

	EdgePoint()
	{
		this->NxNy = glm::vec3(0.0f, 0.0f, 0.0f);
		this->NxPy = glm::vec3(0.0f, 0.0f, 0.0f);
		this->PxNy = glm::vec3(0.0f, 0.0f, 0.0f);
		this->PxPy = glm::vec3(0.0f, 0.0f, 0.0f);
	}

};

struct VaoUnit
{
	// 默认使用VAO1个，VBO使用3个
	unsigned int VBO[5], VAO[1];
	EdgePoint m_EdgePoint;
	bool IsCurrentMeasure;

	VaoUnit()
	{
		this->m_EdgePoint = EdgePoint();
		this->IsCurrentMeasure = false;

		glGenVertexArrays(1, VAO);
		glGenBuffers(5, VBO);

	}

	void Release()
	{
		glDeleteVertexArrays(1, VAO);
		glDeleteBuffers(5, VBO);
	}

};



class Object
{
public:
	Object();
	~Object();

	void Init(int VaoUnitNumber);
	bool Release();

	void AddData2VaoUnit();

private:

	//一个物体由多个VAO组合完成，这里假设一个物体由VaoUnitNumber个VAO组合完成。
	int VaoUnitNumber;
	VaoUnit *p_VaoUnit;


	std::vector<VaoUnit *> VaoUnitVector;
};

