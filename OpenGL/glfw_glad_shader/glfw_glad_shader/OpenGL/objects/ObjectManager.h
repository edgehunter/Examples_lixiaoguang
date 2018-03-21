#pragma once
#include "Object.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Init(int ObjectNumber, int VaoUnitNumber = 100);
	bool Release();
	void AddObject(int AddNumber);

	void AddData2Object();

private:

	//一个物体由多个VAO组合完成，这里假设一个物体由VaoUnitNumber个VAO组合完成。
	int VaoUnitNumber;
	Object* p_Object;

	//ObjectNumber，对象的个数根据需要扩充
	int ObjectNumber;

	std::vector<Object *> ObjectVector;
};

