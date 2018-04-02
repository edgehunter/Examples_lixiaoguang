#pragma once

#include <deque>
#include "VaoUnit.h"

class ObjectQueue
{
public:
	ObjectQueue();
	~ObjectQueue();

	void Init(int QueueLength, int VaoUnitNumber = 4);
	bool Release();
	void AddObject2Queue(int AddLength);

	void AddData2Object(char* DataPoints, char* DataColors);

	void RenderObject();

private:

	//一个VAO由多帧数据组合完成，一个VAO大小VaoUnitSize
	int VaoUnitSize;

	// 每一个对象，封装成VaoUnit
	VaoUnit* p_VaoUnit;

	//QueueLength，队列长度
	int QueueLength;


	//使用队列，Double ended queue
	std::deque<VaoUnit *> ObjectDeque;
	unsigned int FrameCount;
};

