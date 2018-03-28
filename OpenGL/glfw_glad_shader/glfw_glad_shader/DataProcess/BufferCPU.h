#pragma once

#include <stdio.h>
#include <tchar.h>
#include <atlstr.h>

//循环链表节点
typedef struct list
{
	char* data;
	list *next;
}List, *PList;

class BufferCPU
{
public:
	BufferCPU(int Size_List, int DataLength);
	~BufferCPU();


private:
	int DataLength;
	int Size_List;
	PList Front, Rear;

public:

	//采用尾插法建立循环链表
	void CreateList();

	//在循环循环链表尾部插入新的节点
	void PushRear();

	int LengthList();
	bool isEmpty();
	bool isFull();
	bool EnList(char* P_char);//存储单元长度为 DataLength
	bool DeList(char* P_char);//存储单元长度为 DataLength

	bool ReleaseList();
	void ShowStatus();//显示当前容量状态
};