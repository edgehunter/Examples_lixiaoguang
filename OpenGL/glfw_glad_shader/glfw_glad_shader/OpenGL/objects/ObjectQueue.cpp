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

	IsMultipleFrame = true;
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
		else
		{
			delete (*it);
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

bool ObjectQueue::SubtractObject2Queue(int SubtractLength)
{
	if (SubtractLength > 0 && QueueLength - SubtractLength > 5)
	{
		QueueLength -= SubtractLength;

		while (SubtractLength > 0)
		{
			p_VaoUnit = ObjectDeque.front();
			ObjectDeque.pop_front();

			if (!p_VaoUnit->Release())
			{
				return false;
			}
			else
			{
				delete p_VaoUnit;
			}

			SubtractLength--;
		}

		printf("ObjectDeque Size[%d] \n", ObjectDeque.size());
		return true;

	} 
	else
	{
		printf("Error! SubtractLength must greater than zero! \n");
		return false;
	}

}


void ObjectQueue::AddData2Object(char* DataPoints)
{

	if (IsMultipleFrame)
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
	else
	{
		p_VaoUnit = ObjectDeque.front();

		if (p_VaoUnit->Reset())
		{
			p_VaoUnit->BindData2VaoUnit(DataPoints);
			FrameCount++;

			printf("ObjectDeque Size[%d] FrameCount[%d] Single Frame! \n", ObjectDeque.size(), FrameCount);
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

void ObjectQueue::SetMultipleFrame(bool Val)
{

	IsMultipleFrame = Val;

	if (!IsMultipleFrame)
	{
		for (auto it = ObjectDeque.cbegin(); it != ObjectDeque.cend(); ++it)
		{
			if (!(*it)->IsEmpty())
			{
				(*it)->Reset();
			}
			else
			{
				break;
			}			
		}

	} 
}


void ObjectQueue::UpdateFromInputs(GLFWwindow* Window)
{

	// GLFW_KEY_0
	if (glfwGetKey(Window, GLFW_KEY_0) == GLFW_PRESS)
	{
		SubtractObject2Queue(1);
	}

	// GLFW_KEY_9
	if (glfwGetKey(Window, GLFW_KEY_9) == GLFW_PRESS)
	{
		AddObject2Queue(1);
	}

	// GLFW_KEY_M
	if (glfwGetKey(Window, GLFW_KEY_M) == GLFW_PRESS)
	{
		SetMultipleFrame(true);
	}

	// GLFW_KEY_N
	if (glfwGetKey(Window, GLFW_KEY_N) == GLFW_PRESS)
	{
		SetMultipleFrame(false);
	}
}