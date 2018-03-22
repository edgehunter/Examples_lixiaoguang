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
		p_VaoUnit = new VaoUnit(i);
		VaoUnitVectorIdle.emplace_back (p_VaoUnit);

		printf("VaoUnitNumber idx = %d \n", i);
	}

	InitSampleData();
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

	ReleaseSampleData();

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

void Object::InitSampleData()
{

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	float VerticesCube_T[]{
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	float ColorsCube_T[]{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};

	int TT = sizeof(VerticesCube_T) / sizeof(float);
	
	VerticesCubeSample = new float[sizeof(VerticesCube_T) / sizeof(float)];
	ColorsCubeSample = new float[sizeof(ColorsCube_T) / sizeof(float)];

	memcpy(VerticesCubeSample, VerticesCube_T, sizeof(VerticesCube_T));
	memcpy(ColorsCubeSample, ColorsCube_T, sizeof(ColorsCube_T));

	//for (size_t i = 0; i < sizeof(VerticesCube_T) / sizeof(float); i++)
	//{
	//	printf("VerticesCubeSample[%d] = %f \n", i, VerticesCubeSample[i]);
	//	
	//}

	printf("InitSampleData \n");
}

void Object::ReleaseSampleData()
{

	delete VerticesCubeSample;
	delete ColorsCubeSample;
}


bool Object::AddData2VaoUnit()
{
	if (!VaoUnitVectorIdle.empty())
	{
		p_VaoUnit = VaoUnitVectorIdle.back();
		VaoUnitVectorIdle.pop_back();

		//加载数据
		BindData2VaoUnit();

		//进入渲染队列
		VaoUnitVectorShow.emplace_back (p_VaoUnit);

		printf("AddData To VaoUnit Success\n");
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
	if (VaoUnitVectorShow.size() == VaoUnitNumber)
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


void Object::BindData2VaoUnit()
{
	

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	float VerticesCube_T[]{
		-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	float ColorsCube_T[]{
		0.583f, 0.771f, 0.014f,
			0.609f, 0.115f, 0.436f,
			0.327f, 0.483f, 0.844f,
			0.822f, 0.569f, 0.201f,
			0.435f, 0.602f, 0.223f,
			0.310f, 0.747f, 0.185f,
			0.597f, 0.770f, 0.761f,
			0.559f, 0.436f, 0.730f,
			0.359f, 0.583f, 0.152f,
			0.483f, 0.596f, 0.789f,
			0.559f, 0.861f, 0.639f,
			0.195f, 0.548f, 0.859f,
			0.014f, 0.184f, 0.576f,
			0.771f, 0.328f, 0.970f,
			0.406f, 0.615f, 0.116f,
			0.676f, 0.977f, 0.133f,
			0.971f, 0.572f, 0.833f,
			0.140f, 0.616f, 0.489f,
			0.997f, 0.513f, 0.064f,
			0.945f, 0.719f, 0.592f,
			0.543f, 0.021f, 0.978f,
			0.279f, 0.317f, 0.505f,
			0.167f, 0.620f, 0.077f,
			0.347f, 0.857f, 0.137f,
			0.055f, 0.953f, 0.042f,
			0.714f, 0.505f, 0.345f,
			0.783f, 0.290f, 0.734f,
			0.722f, 0.645f, 0.174f,
			0.302f, 0.455f, 0.848f,
			0.225f, 0.587f, 0.040f,
			0.517f, 0.713f, 0.338f,
			0.053f, 0.959f, 0.120f,
			0.393f, 0.621f, 0.362f,
			0.673f, 0.211f, 0.457f,
			0.820f, 0.883f, 0.371f,
			0.982f, 0.099f, 0.879f
	};

	for (size_t i = 0; i < sizeof(VerticesCube_T)/sizeof(float); i += 3)
	{
		//printf("VerticesCube_T[%d] = %f \n", i, VerticesCube_T[i]); 
		VerticesCube_T[i] += -p_VaoUnit->GetIndex()*4;
		VerticesCube_T[i + 2] += -Index * 4;
	}

	//memcpy(VerticesCubeSample, VerticesCube_T, sizeof(VerticesCube_T));
	//memcpy(ColorsCubeSample, ColorsCube_T, sizeof(ColorsCube_T));


	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(p_VaoUnit->GetVao()[0]);

	glBindBuffer(GL_ARRAY_BUFFER, p_VaoUnit->GetVbo()[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesCube_T), VerticesCube_T, GL_STATIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, p_VaoUnit->GetVbo()[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColorsCube_T), ColorsCube_T, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

}

void Object::RenderVaoUnit()
{

	// Render VaoUnitVectorShow
	for (auto it = VaoUnitVectorShow.cbegin(); it != VaoUnitVectorShow.cend(); ++it)
	{

		// bind the VAO
		glBindVertexArray((*it)->GetVao()[0]);

		// 12*3 indices starting at 0 -> 12 triangles
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		// unbind the VAO
		glBindVertexArray(0);

		//printf("VaoUnitVectorShow.size[%d] \n", VaoUnitVectorShow.size());
	}

}