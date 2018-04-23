#pragma once

#include <deque>
#include "VaoUnit.h"

// Include GLFW
#include <GLFW/glfw3.h>

class ObjectQueue
{
public:
	ObjectQueue();
	~ObjectQueue();

	void Init(int QueueLength, int VaoUnitNumber = 4);
	bool Release();

	void AddObject2Queue(int AddLength);

	bool SubtractObject2Queue(int SubtractLength);
	void AddData2Object(char* DataPoints);
	void SetMultipleFrame(bool Val);
	void UpdateFromInputs(GLFWwindow* Window);


	void RenderObject();

private:

	//一个VAO由多帧数据组合完成，一个VAO大小VaoUnitSize
	int VaoUnitSize;

	// 每一个对象，封装成VaoUnit
	VaoUnit* p_VaoUnit;

	//QueueLength，队列长度
	int QueueLength;

	// 单/多帧切换
	bool IsMultipleFrame;

	//使用队列，Double ended queue
	std::deque<VaoUnit *> ObjectDeque;
	unsigned int FrameCount;
};

