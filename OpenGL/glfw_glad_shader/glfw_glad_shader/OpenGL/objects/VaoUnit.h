#pragma once

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



class VaoUnit
{
public:
	VaoUnit(int index);
	~VaoUnit();

	void Release();

	unsigned int *GetVao();
	unsigned int *GetVbo();

	//获取当前VaoUnit对象在Vector中的编号
	int GetIndex();

private:

	// 默认使用VAO1个，VBO使用3个
	unsigned int VBO[5], VAO[1];
	EdgePoint m_EdgePoint;
	bool IsCurrentMeasure;

	//当前VaoUnit对象在Vector中的编号
	int index;

};

