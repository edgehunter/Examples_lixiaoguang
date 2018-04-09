#include "ObjectQueue.h"


ObjectQueue::ObjectQueue()
{
}


ObjectQueue::~ObjectQueue()
{
}

void ObjectQueue::Init(int QueueLength, int VaoUnitSize)
{
	this->VaoUnitSize = VaoUnitSize;
	this->QueueLength = QueueLength;



	for (int i = 0; i < QueueLength; i++)
	{

		p_VaoUnit = new VaoUnit();
		p_VaoUnit->Init(i, VaoUnitSize);

		ObjectDeque.emplace_back(p_VaoUnit);

		printf("p_VaoUnit idx = %d \n", i);
	}

	FrameCount = 0;

}

bool ObjectQueue::Release()
{

	for (std::deque<VaoUnit *>::iterator it = ObjectDeque.begin(); it != ObjectDeque.end(); ++it)
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

void ObjectQueue::AddObject2Queue(int AddLength)
{

	for (int i = QueueLength; i < QueueLength + AddLength; i++)
	{

		p_VaoUnit = new VaoUnit();
		p_VaoUnit->Init(i, VaoUnitSize);

		ObjectDeque.emplace_back(p_VaoUnit);
	}

	QueueLength += AddLength;
}

void ObjectQueue::AddData2Object(char* DataPoints)
{
	bool IsFull = true;


	for (std::deque<VaoUnit *>::iterator it = ObjectDeque.begin(); it != ObjectDeque.end(); ++it)
	{
		if (!(*it)->IsFull())
		{
			(*it)->BindData2VaoUnit(DataPoints);
			IsFull = false;
			FrameCount++;
			break;
		}
	}

	//if (!IsFull)
	//{
	//	printf("ObjectDeque Size[%d] FrameCount[%d] \n", ObjectDeque.size(), FrameCount);
	//}
	//else
	//{
	//	printf("ObjectDeque Size[%d] FrameCount[%d] FULL \n", ObjectDeque.size(), FrameCount);
	//}
	

	if (!IsFull)
	{
		printf("ObjectDeque Size[%d] FrameCount[%d] \n", ObjectDeque.size(), FrameCount);
	}
	else
	{
		p_VaoUnit = ObjectDeque.front();

		if (p_VaoUnit->Reset())
		{
			p_VaoUnit->BindData2VaoUnit(DataPoints);
			FrameCount++;

			ObjectDeque.pop_front();
			ObjectDeque.emplace_back(p_VaoUnit);

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
	for (auto it = ObjectDeque.cbegin(); it != ObjectDeque.cend(); ++it)
	{
		if (!(*it)->IsEmpty())
		{
			(*it)->RanderVAO();
		}
		else
		{
			break;
		}
	}

}