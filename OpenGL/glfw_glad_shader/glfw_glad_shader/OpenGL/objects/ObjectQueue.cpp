#include "ObjectQueue.h"


ObjectQueue::ObjectQueue()
{
}


ObjectQueue::~ObjectQueue()
{
}

void ObjectQueue::Init(int ObjectNumber, int VaoUnitNumber)
{

	//һ�������ɶ��VAO�����ɣ��������һ��������VaoUnitNumber��VAO�����ɡ�
	this->VaoUnitNumber = VaoUnitNumber;

//ObjectNumber������ĸ���������Ҫ����
this->ObjectNumber = ObjectNumber;// 10;



for (int i = 0; i < ObjectNumber; i++)
{

	p_Object = new Object();
	p_Object->Init(VaoUnitNumber, i);

	ObjectDeque.emplace_back(p_Object);

	printf("ObjectNumber idx = %d \n", i);
}

Usage = 0;

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

void ObjectQueue::AddData2Object()
{
	bool IsFull = true;


	for (std::deque<Object *>::iterator it = ObjectDeque.begin(); it != ObjectDeque.end(); ++it)
	{
		if (!(*it)->IsFull())
		{
			(*it)->AddData2VaoUnit();
			IsFull = false;
			Usage++;
			break;
		}
	}

	if (!IsFull)
	{
		printf("ObjectDeque Size[%d] Usage[%d] \n", ObjectDeque.size(), Usage);
	}
	else
	{
		printf("ObjectDeque Size[%d] Usage[%d] FULL! \n", ObjectDeque.size(), Usage);
	}
	//p_Object = ObjectDeque.front();
	//p_Object->AddData2VaoUnit();

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