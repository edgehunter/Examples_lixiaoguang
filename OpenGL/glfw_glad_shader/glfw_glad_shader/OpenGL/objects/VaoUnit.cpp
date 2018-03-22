#include "VaoUnit.h"


VaoUnit::VaoUnit(int index)
{
	this->index = index;
	this->m_EdgePoint = EdgePoint();
	this->IsCurrentMeasure = false;

	glGenVertexArrays(1, VAO);
	glGenBuffers(5, VBO);
}

VaoUnit::~VaoUnit()
{
}

void VaoUnit::Release()
{
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(5, VBO);
}

unsigned int* VaoUnit::GetVao()
{
	return VAO;
}

unsigned int* VaoUnit::GetVbo()
{

	return VBO;
}

//获取当前VaoUnit对象在Vector中的编号
int VaoUnit::GetIndex()
{
	return index;
}