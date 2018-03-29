#include "VaoUnit.h"
#include <stdio.h>


VaoUnit::VaoUnit()
{

}

VaoUnit::~VaoUnit()
{
}

void VaoUnit::Init(int index, int FrameCountMax)
{

	this->index = index;
	this->FrameCountMax = FrameCountMax;
	FrameCount = 0;

	this->m_EdgePoint = EdgePoint();
	this->IsCurrentMeasure = false;

	glGenVertexArrays(1, VAO);
	glGenBuffers(5, VBO);


	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, FrameCountMax * 128 * 128 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	//glBufferSubData(GL_ARRAY_BUFFER, FrameCount * 128 * 128 * 3 * sizeof(float), 128 * 128 * 3 * sizeof(float), DataPoints);
	/*

	// 获取缓冲区的映射指针ptr
	void * ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// 拷贝我们的数据到指针所指向的位置
	memcpy(ptr, DataPoints, 128 * 128 * 3 * sizeof(float));

	// 使用完之后释放映射的指针
	glUnmapBuffer(GL_ARRAY_BUFFER);

	*/


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);



	// 2nd attribute buffer : colors

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, FrameCountMax * 128 * 128 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	//glBufferSubData(GL_ARRAY_BUFFER, FrameCount * 128 * 128 * 4 * sizeof(float), 128 * 128 * 4 * sizeof(float), DataColors);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

}

void VaoUnit::Release()
{
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(5, VBO);
}

// 绑定数据到VAO
void VaoUnit::BindData2VaoUnit(char* DataPoints, char* DataColors)
{
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferSubData(GL_ARRAY_BUFFER, FrameCount * 128 * 128 * 3 * sizeof(float), 128 * 128 * 3 * sizeof(float), DataPoints);
	/*

	// 获取缓冲区的映射指针ptr
	void * ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// 拷贝我们的数据到指针所指向的位置
	memcpy(ptr, DataPoints, 128 * 128 * 3 * sizeof(float));

	// 使用完之后释放映射的指针
	glUnmapBuffer(GL_ARRAY_BUFFER);

	*/



	// 2nd attribute buffer : colors

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	glBufferSubData(GL_ARRAY_BUFFER, FrameCount * 128 * 128 * 4 * sizeof(float), 128 * 128 * 4 * sizeof(float), DataColors);


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	


	FrameCount++;
}

// 渲染当前VAO
void VaoUnit::RanderVAO()
{
	// bind the VAO
	glBindVertexArray(VAO[0]);

	// 12*3 indices starting at 0 -> 12 triangles
	glDrawArrays(GL_POINTS, 0, FrameCount * 128 * 128 * 3);

	//printf("FrameCount[%d] \n", FrameCount);

	// unbind the VAO
	glBindVertexArray(0);

}

// 获取当前VAO已载入帧数
int VaoUnit::Get_FrameCount()
{

	return FrameCount;
}

// 当前VAO 重置
void VaoUnit::Reset()
{
	FrameCount = 0;
}

// 判断 当前VAO已载入帧数，是否达到最大值
bool VaoUnit::IsFull()
{
	if (FrameCount == FrameCountMax)
	{
		return true;
	}
	else
	{
		return false;
	}

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