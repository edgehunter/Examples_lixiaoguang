#pragma once

// vector
#include <vector>

#include "VaoUnit.h"

class Object
{
public:
	Object();
	~Object();

	void Init(int VaoUnitNumber, int Index);
	bool Release();

	

	//目前使用模拟数据，需要采用真实数据，由指针传入
	//Idle出栈，Show入栈
	bool AddData2VaoUnit(char* DataPoints, char* DataColors);
	bool IsFull();
	bool IsEmpty();
	
	void RenderVaoUnit();

protected:
	void InitSampleData();
	void ReleaseSampleData();

	void BindData2VaoUnit(char* DataPoints, char* DataColors);

private:

	//一个物体由多个VAO组合完成，这里假设一个物体由VaoUnitNumber个VAO组合完成。
	int VaoUnitNumber;
	VaoUnit *p_VaoUnit;

	// 空闲队列VaoUnitVectorIdle，用于加载点云数据
	std::vector<VaoUnit *> VaoUnitVectorIdle;

	// 显示队列VaoUnitVectorShow，用于显示点云数据
	std::vector<VaoUnit *> VaoUnitVectorShow;

	//当前对象在Queue中的编号
	int Index;

	//Cube例子，用于测试VAO队列
	float* VerticesCubeSample;
	float* ColorsCubeSample;
};

