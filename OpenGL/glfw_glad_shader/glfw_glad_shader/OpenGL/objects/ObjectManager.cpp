#include "ObjectManager.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::Init(int ObjectNumber, int VaoUnitNumber)
{

	//一个物体由多个VAO组合完成，这里假设一个物体由VaoUnitNumber个VAO组合完成。
	this->VaoUnitNumber = VaoUnitNumber;

	//ObjectNumber，对象的个数根据需要扩充
	this->ObjectNumber = ObjectNumber;// 10;


	for (int i = 0; i < (int)ObjectNumber; i++)
	{

		p_Object = new Object();
		p_Object->Init(VaoUnitNumber);

		ObjectVector.push_back(p_Object);

		printf("ObjectNumber idx = %d \n", i);
	}

}

bool ObjectManager::Release()
{

	//另一种方式全部释放
	/*
	while (!ObjectVector.empty())
	{
		p_Object = ObjectVector.back();
		
		if (!p_Object->Release())
		{
			return false;
		}
		
		ObjectVector.pop_back();
	}
	*/


	for (std::vector<Object *>::iterator it = ObjectVector.begin(); it != ObjectVector.end(); ++it)
	{

		if (!(*it)->Release())
		{
			return false;
		}

	}

	// erase the all elements:
	ObjectVector.erase(ObjectVector.begin(), ObjectVector.end());

	if (ObjectVector.empty())
	{
		printf("ObjectManager Release Success! \n");
		return true;
	}
	else
	{
		return false;
	}

}

void ObjectManager::AddObject(int AddNumber)
{

	for (int i = 0; i < AddNumber; i++)
	{

		p_Object = new Object();
		p_Object->Init(VaoUnitNumber);

		ObjectVector.push_back(p_Object);
	}
}

void ObjectManager::AddData2Object()
{
	//需要结构体，说明ObjectVector使用情况
	//以下为临时例子
	for (std::vector<Object *>::iterator it = ObjectVector.begin(); it != ObjectVector.end(); ++it)
	{

		p_Object->AddData2VaoUnit();

	}
}