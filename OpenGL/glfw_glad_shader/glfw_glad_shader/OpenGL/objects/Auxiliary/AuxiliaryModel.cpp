#include "AuxiliaryModel.h"

AuxiliaryModel::AuxiliaryModel()
{
}


AuxiliaryModel::~AuxiliaryModel()
{
}

void AuxiliaryModel::Init()
{

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	float VerticesCube[] = {
		-10.0f, -10.0f, 170.0f, 1.0f,	
		-10.0f, 10.0f, 170.0f, 1.0f,
		10.0f, 10.0f, 170.0f, 1.0f,
		10.0f, -10.0f, 170.0f, 1.0f,
		//
		
		-40.0f, -40.0f, 1.0f + 110.0f, 1.0f,
		-40.0f, 40.0f, 1.0f + 110.0f, 1.0f,
		40.0f, 40.0f, 1.0f + 110.0f, 1.0f,
		40.0f, -40.0f, 1.0f + 110.0f, 1.0f
		
	};

	// One color for each vertex. They were generated randomly.
	float ColorsCube[] = {
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,

		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f
	};

	GLuint IndicesCube[] = {
		// 起始于0!
		0, 1, 3, // z前
		1, 2, 3, // 


		4, 5, 7,// z后
		5, 6, 7,

		1, 2, 5,// y上
		2, 5, 6,

		0, 3, 4,// y下
		3, 4, 7,

		2, 3, 6,//x右
		3, 6, 7,

		0, 1, 4,//x右
		1, 4, 5
	};


	glGenVertexArrays(1, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, EBO);
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesCube), VerticesCube, GL_STATIC_DRAW);

	//GL_MAP_PERSISTENT_BIT // 处在Mapped状态也能使用
	//GL_MAP_COHERENT_BIT; // 数据对GPU立即可见

	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(VerticesCube), VerticesCube, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	

	// 2nd attribute buffer : colors

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColorsCube), ColorsCube, GL_STATIC_DRAW);

	//GL_MAP_PERSISTENT_BIT // 处在Mapped状态也能使用
	//GL_MAP_COHERENT_BIT; // 数据对GPU立即可见

	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(ColorsCube), ColorsCube, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);


	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
	3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	4,                                // size
	GL_FLOAT,                         // type
	GL_FALSE,                         // normalized?
	0,                                // stride
	(void*)0                          // array buffer offset
	);

	

	// 3rd attribute buffer : index

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesCube), IndicesCube, GL_STATIC_DRAW);

	//GL_MAP_PERSISTENT_BIT // 处在Mapped状态也能使用
	//GL_MAP_COHERENT_BIT; // 数据对GPU立即可见

	//glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesCube), IndicesCube, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//解绑EBO  
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//解绑VBO  
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

}

bool AuxiliaryModel::Release()
{
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, EBO);

	return true;

}

// 渲染当前VAO
void AuxiliaryModel::RanderVAO()
{

	// ------
	// bind the VAO
	glBindVertexArray(VAO[0]);

	// GL_TRIANGLE_FAN
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// unbind the VAO
	glBindVertexArray(0);
}