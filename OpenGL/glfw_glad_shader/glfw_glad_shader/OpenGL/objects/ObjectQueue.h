#pragma once

#include <deque>
#include "Object.h"

class ObjectQueue
{
public:
	ObjectQueue();
	~ObjectQueue();

	void Init(int ObjectNumber, int VaoUnitNumber = 100);
	bool Release();
	void AddObject(int AddNumber);

	void AddData2Object();

	void RenderObject();

private:

	//一个物体由多个VAO组合完成，这里假设一个物体由VaoUnitNumber个VAO组合完成。
	int VaoUnitNumber;
	Object* p_Object;

	//ObjectNumber，对象的个数根据需要扩充
	int ObjectNumber;


	//使用队列，Double ended queue
	std::deque<Object *> ObjectDeque;
	unsigned int Usage;
};

