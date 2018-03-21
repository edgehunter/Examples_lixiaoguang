#include "Object.h"


Object::Object()
{
}


Object::~Object()
{
}


void Object::Init(int VaoUnitNumber)
{
	this->VaoUnitNumber = VaoUnitNumber;

	for (int i = 0; i < VaoUnitNumber; i++)
	{
		p_VaoUnit = new VaoUnit();
		VaoUnitVector.push_back(p_VaoUnit);

		printf("VaoUnitNumber idx = %d \n", i);
	}

}

bool Object::Release()
{
	//另一种方式全部释放
	/*
	while (!VaoUnitVector.empty())
	{
		p_VaoUnit = VaoUnitVector.back();
		delete p_VaoUnit;
		VaoUnitVector.pop_back();
	}
	*/

	for (std::vector<VaoUnit *>::iterator it = VaoUnitVector.begin(); it != VaoUnitVector.end(); ++it)
	{

		//去掉VAO，VBO等
		(*it)->Release();
		delete *it;

		//printf("Release VaoUnit \n");
	}

	// erase the all elements:
	VaoUnitVector.erase(VaoUnitVector.begin(), VaoUnitVector.end());

	if (VaoUnitVector.empty())
	{
		printf("Object Release Success! \n");
		return true;
	}
	else
	{
		return false;
	}

}

void Object::AddData2VaoUnit()
{
	//需要一个结构体，说明VaoUnitVector使用情况
	printf("AddData To VaoUnit \n");
}













/*

// erasing from vector
#include <iostream>

int main()
{
	std::vector<int> myvector;

	// set some values (from 1 to 10)
	for (int i = 1; i <= 10; i++) myvector.push_back(i);

	// erase the 6th element
	myvector.erase(myvector.begin() + 5);

	// erase the first 3 elements:
	myvector.erase(myvector.begin(), myvector.begin() + 3);

	std::cout << "myvector contains:";
	for (unsigned i = 0; i<myvector.size(); ++i)
		std::cout << ' ' << myvector[i];
	std::cout << '\n';

	return 0;
}

*/