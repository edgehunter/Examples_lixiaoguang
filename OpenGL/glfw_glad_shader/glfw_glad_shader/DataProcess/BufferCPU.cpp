
#include "BufferCPU.h"


BufferCPU::BufferCPU(int Size_List, int DataLength)
{

	this->Size_List = Size_List;
	this->DataLength = DataLength;
}


BufferCPU::~BufferCPU()
{
}

//采用尾插法建立循环链表
void BufferCPU::CreateList()
{
	Front = new List;
	Front->data = new char[DataLength];
	Rear = Front;

	for (int i = 1; i < Size_List; i++)
	{
		Rear->next = new List;
		Rear = Rear->next;
		Rear->data = new char[DataLength];
	}
	Rear->next = Front;
	Rear = Front;
}

//在循环循环链表尾部插入新的节点
void BufferCPU::PushRear()
{
	PList newList = new List;
	newList->data = new char[DataLength];

	newList->next = Rear->next;
	Rear->next = newList;

	Rear = Rear->next;
}


int BufferCPU::LengthList()
{
	int i = 0;
	PList temp = Front; // temp 指向头结点
	while (temp != Rear)
	{
		i++;
		temp = temp->next;
	}
	return i;
}

bool BufferCPU::isEmpty()
{
	if (Front == Rear) // 空 
		return true;
	else
		return false;
}

bool BufferCPU::isFull()
{
	if (Front == Rear->next) // 满 
		return true;
	else
		return false;
}




bool BufferCPU::EnList(char* P_char)
{
	if (isFull())
	{
		memcpy(Rear->data, P_char, DataLength*sizeof(char));
		PushRear();
	}
	else
	{
		memcpy(Rear->data, P_char, DataLength*sizeof(char));
		Rear = Rear->next;
	}

	return true;
}

bool BufferCPU::DeList(char* P_char)
{
	memcpy(P_char, Front->data, DataLength*sizeof(char));
	Front = Front->next;
	return true;
}

bool BufferCPU::ReleaseList()
{
	PList temp = Front->next;
	PList P_Release;

	while (temp != Front)
	{
		P_Release = temp;
		temp = temp->next;
		delete[]  P_Release->data;
		delete P_Release;
	}

	delete[]  Front->data;
	delete Front;

	return true;
}

void BufferCPU::ShowStatus()
{
	int Length_List = this->LengthList();

	printf("BufferCPU Current List = %d , Max List = %d , Usage Rate = %f \n", Length_List, Size_List, (float)Length_List / (float)Size_List);
}