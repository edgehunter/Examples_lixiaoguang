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
	VaoUnit();
	~VaoUnit();

	void Init(int index, int FrameCountMax);
	bool Release();

	// 绑定数据到VAO
	void BindData2VaoUnit(char* DataPoints, char* DataColors);

	// 渲染当前VAO
	void RanderVAO();

	// 获取当前VAO已载入帧数
	int Get_FrameCount();

	// 当前VAO 重置
	bool Reset();

	// 判断 当前VAO已载入帧数，是否达到最大值
	bool IsFull();

	// 判断 当前VAO是否为空
	bool IsEmpty();

	unsigned int *GetVao();
	unsigned int *GetVbo();

	//获取当前VaoUnit对象在Vector中的编号
	int GetIndex();

private:

	// 默认使用VAO1个，VBO使用5个
	unsigned int VBO[5], VAO[1];

	// Points缓存
	char* Points;

	// colors 缓存
	char* Colors;

	//char* ptr_Points;
	//char* ptr_Colors;

	// 当前VAO边缘数据点坐标
	EdgePoint m_EdgePoint;

	// 当前VAO是否存在数据测量
	bool IsCurrentMeasure;

	//当前VaoUnit对象在Vector中的编号
	int index;

	// FrameCountMax，每一个VAO承载的帧数总量
	int FrameCountMax;

	// FrameCount，每一个VAO承载的帧数当前量
	int FrameCount;

	// CurrentBytes，当前数据量（字节）
	int CurrentBytes;
};

