#include "ObjectQueue.h"


ObjectQueue::ObjectQueue()
{
}


ObjectQueue::~ObjectQueue()
{
}

void ObjectQueue::Init(int ObjectNumber, int VaoUnitNumber)
{

	//一个物体由多个VAO组合完成，这里假设一个物体由VaoUnitNumber个VAO组合完成。
	this->VaoUnitNumber = VaoUnitNumber;

	//ObjectNumber，对象的个数根据需要扩充
	this->ObjectNumber = ObjectNumber;// 10;



	for (int i = 0; i < ObjectNumber; i++)
	{

		p_Object = new Object();
		p_Object->Init(VaoUnitNumber, i);

		ObjectDeque.emplace_back(p_Object);

		printf("ObjectNumber idx = %d \n", i);
	}

	FrameCount = 0;

}

bool ObjectQueue::Release()
{

	for (std::deque<Object *>::iterator it = ObjectDeque.begin(); it != ObjectDeque.end(); ++it)
	{

		if (!(*it)->Release())
		{
			return false;
		}

	}

	// erase the all elements:
	ObjectDeque.erase(ObjectDeque.begin(), ObjectDeque.end());

	if (ObjectDeque.empty())
	{
		printf("ObjectQueue Release Success! \n");
		return true;
	}
	else
	{
		return false;
	}

}

void ObjectQueue::AddObject(int AddNumber)
{

	for (int i = VaoUnitNumber; i < VaoUnitNumber + AddNumber; i++)
	{

		p_Object = new Object();
		p_Object->Init(VaoUnitNumber, i);

		ObjectDeque.emplace_back(p_Object);
	}

	VaoUnitNumber += AddNumber;
}

void ObjectQueue::AddData2Object(char* DataPoints, char* DataColors)
{
	bool IsFull = true;


	for (std::deque<Object *>::iterator it = ObjectDeque.begin(); it != ObjectDeque.end(); ++it)
	{
		if (!(*it)->IsFull())
		{
			(*it)->AddData2VaoUnit(DataPoints, DataColors);
			IsFull = false;
			FrameCount++;
			break;
		}
	}

	if (!IsFull)
	{
		printf("ObjectDeque Size[%d] FrameCount[%d] \n", ObjectDeque.size(), FrameCount);
	}
	else
	{
		p_Object = ObjectDeque.front();

		if (p_Object->Reset())
		{
			p_Object->AddData2VaoUnit(DataPoints, DataColors);
			IsFull = false;
			FrameCount++;

			ObjectDeque.pop_front();
			ObjectDeque.emplace_back(p_Object);

			printf("ObjectDeque Size[%d] FrameCount[%d] FULL & Reset! \n", ObjectDeque.size(), FrameCount);
		}
		else
		{
			printf("ObjectDeque Size[%d] FrameCount[%d] FULL & Reset Error! \n", ObjectDeque.size(), FrameCount);
		}
		
	}

}

void ObjectQueue::RenderObject()
{
	//p_Object = ObjectDeque.front();
	//p_Object->RenderVaoUnit();

	for (auto it = ObjectDeque.cbegin(); it != ObjectDeque.cend(); ++it)
	{
		if (!(*it)->IsEmpty())
		{
			(*it)->RenderVaoUnit();
		}
		else
		{
			break;
		}
	}

}