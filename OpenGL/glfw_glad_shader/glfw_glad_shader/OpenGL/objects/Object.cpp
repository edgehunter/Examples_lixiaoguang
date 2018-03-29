#include "Object.h"


Object::Object()
{
}


Object::~Object()
{
}


void Object::Init(int VaoUnitNumber, int Index)
{
	this->VaoUnitNumber = VaoUnitNumber;

	for (int i = 0; i < VaoUnitNumber; i++)
	{
		p_VaoUnit = new VaoUnit();
		p_VaoUnit->Init(i);
		VaoUnitVectorIdle.emplace_back (p_VaoUnit);

		//printf("VaoUnitNumber idx = %d \n", i);
	}

	this->Index = Index;
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

	// Release VaoUnitVectorIdle
	for (std::vector<VaoUnit *>::iterator it = VaoUnitVectorIdle.begin(); it != VaoUnitVectorIdle.end(); ++it)
	{
		(*it)->Release();
		delete *it;

		//printf("Release VaoUnitVectorIdle \n");
	}

	// Release VaoUnitVectorShow
	for (std::vector<VaoUnit *>::iterator it = VaoUnitVectorShow.begin(); it != VaoUnitVectorShow.end(); ++it)
	{
		(*it)->Release();
		delete *it;

		//printf("Release VaoUnitVectorShow \n");
	}

	// erase VaoUnitVectorIdle all elements:
	VaoUnitVectorIdle.erase(VaoUnitVectorIdle.begin(), VaoUnitVectorIdle.end());

	// erase VaoUnitVectorShow all elements:
	VaoUnitVectorShow.erase(VaoUnitVectorShow.begin(), VaoUnitVectorShow.end());


	if (VaoUnitVectorIdle.empty() && VaoUnitVectorShow.empty())
	{
		printf("Object Release Success! \n");
		return true;
	}
	else
	{
		return false;
	}

}


bool Object::AddData2VaoUnit(char* DataPoints, char* DataColors)
{

	// 进入VAO中，一个VAO包含多帧数据，由VaoUnit中 FrameCountMax
	if (VaoUnitVectorShow.size() > 0 && !VaoUnitVectorShow.back()->IsFull())
	{
		p_VaoUnit = VaoUnitVectorShow.back();
		p_VaoUnit->BindData2VaoUnit(DataPoints, DataColors);

		return true;
	}
	else if (!VaoUnitVectorIdle.empty())
	{
		p_VaoUnit = VaoUnitVectorIdle.back();
		VaoUnitVectorIdle.pop_back();

		//加载数据
		p_VaoUnit->BindData2VaoUnit(DataPoints, DataColors);

		//进入渲染队列
		VaoUnitVectorShow.emplace_back (p_VaoUnit);

		//printf("AddData To VaoUnit Success\n");
		return true;
	}
	else
	{
		printf("AddData To VaoUnit faild ! its full \n");
		return false;
	}
}

bool Object::IsFull()
{
	if (VaoUnitVectorShow.size() == VaoUnitNumber && VaoUnitVectorShow.back()->IsFull())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Object::IsEmpty()
{
	if (VaoUnitVectorIdle.size() == VaoUnitNumber)
	{
		return true;
	}
	else
	{
		return false;
	}
}




void Object::RenderVaoUnit()
{

	// Render VaoUnitVectorShow
	for (auto it = VaoUnitVectorShow.cbegin(); it != VaoUnitVectorShow.cend(); ++it)
	{
		(*it)->RanderVAO();

		//printf("VaoUnitVectorShow.size[%d] \n", VaoUnitVectorShow.size());
	}

}

bool Object::Reset()
{

	while (!VaoUnitVectorShow.empty())
	{
		p_VaoUnit = VaoUnitVectorShow.back();
		VaoUnitVectorShow.pop_back();

		// 重置VaoUnit已载入帧数
		p_VaoUnit->Reset();

		//进入渲染队列
		VaoUnitVectorIdle.emplace_back(p_VaoUnit);

	}

	if (VaoUnitVectorShow.empty() && VaoUnitVectorIdle.size() == VaoUnitNumber)
	{
		return true;
	} 
	else
	{
		return false;
	}
}