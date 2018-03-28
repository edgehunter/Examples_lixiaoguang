#include "VaoEngine.h"
#include <time.h>


VaoEngine::VaoEngine()
{
}


VaoEngine::~VaoEngine()
{
}

void VaoEngine::Init(Shader* p_Shader)
{

	this->p_Shader = p_Shader;


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float VerticesTriangleEngine[] = {
		0.5f, -0.5f, -5.0f,  // bottom right
		-0.5f, -0.5f, -5.0f,  // bottom left
		0.0f, 0.5f, -5.0f   // top 
	};

	float ColorsTriangleEngine[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	float VerticesCube_T[] = {
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
	float ColorsCube_T[] = {
		0.583f, 0.771f, 0.014f, 0.4f,
		0.609f, 0.115f, 0.436f, 0.4f,
		0.327f, 0.483f, 0.844f, 0.4f,
		0.822f, 0.569f, 0.201f, 0.4f,
		0.435f, 0.602f, 0.223f, 0.4f,
		0.310f, 0.747f, 0.185f, 0.4f,
		0.597f, 0.770f, 0.761f, 0.4f,
		0.559f, 0.436f, 0.730f, 0.4f,
		0.359f, 0.583f, 0.152f, 0.4f,
		0.483f, 0.596f, 0.789f, 0.4f,
		0.559f, 0.861f, 0.639f, 0.4f,
		0.195f, 0.548f, 0.859f, 0.4f,
		0.014f, 0.184f, 0.576f, 0.4f,
		0.771f, 0.328f, 0.970f, 0.4f,
		0.406f, 0.615f, 0.116f, 0.4f,
		0.676f, 0.977f, 0.133f, 0.4f,
		0.971f, 0.572f, 0.833f, 0.4f,
		0.140f, 0.616f, 0.489f, 0.4f,
		0.997f, 0.513f, 0.064f, 0.4f,
		0.945f, 0.719f, 0.592f, 0.4f,
		0.543f, 0.021f, 0.978f, 0.4f,
		0.279f, 0.317f, 0.505f, 0.4f,
		0.167f, 0.620f, 0.077f, 0.4f,
		0.347f, 0.857f, 0.137f, 0.4f,
		0.055f, 0.953f, 0.042f, 0.4f,
		0.714f, 0.505f, 0.345f, 0.4f,
		0.783f, 0.290f, 0.734f, 0.4f,
		0.722f, 0.645f, 0.174f, 0.4f,
		0.302f, 0.455f, 0.848f, 0.4f,
		0.225f, 0.587f, 0.040f, 0.4f,
		0.517f, 0.713f, 0.338f, 0.4f,
		0.053f, 0.959f, 0.120f, 0.4f,
		0.393f, 0.621f, 0.362f, 0.4f,
		0.673f, 0.211f, 0.457f, 0.4f,
		0.820f, 0.883f, 0.371f, 0.4f,
		0.982f, 0.099f, 0.879f, 0.4f,
	};

	// cube
	// ---------------------------------------

	int temp = sizeof(VerticesCube_T)/sizeof(float);

	float *VerticesCubeEngine;
	float *ColorsCubeEngine;

	VerticesCubeEngine = new float[sizeof(VerticesCube_T) / sizeof(float)];
	ColorsCubeEngine = new float[sizeof(ColorsCube_T) / sizeof(float)];

	memcpy(VerticesCubeEngine, &VerticesCube_T, sizeof(VerticesCube_T));
	memcpy(ColorsCubeEngine, &ColorsCube_T, sizeof(ColorsCube_T));

	int tt = sizeof(VerticesCubeEngine);

	// points
	// ---------------------------------------

	pointNum = 128 * 128;
	VerticesPoints = new float[pointNum*3];
	ColorsPoints = new float[pointNum*4];

	srand(unsigned(time(0)));   //获取系统时间 

	for (size_t i = 0; i < pointNum; i++)
	{
		VerticesPoints[i * 3] = (float)(rand() % 80);
		VerticesPoints[i * 3 + 1] = (float)(rand() % 80);
		VerticesPoints[i * 3 + 2] = (float)(rand() % 80);

		ColorsPoints[i * 4] = (float)(rand() % 80 / (double)80);
		ColorsPoints[i * 4 + 1] = (float)(rand() % 80 / (double)80);
		ColorsPoints[i * 4 + 2] = (float)(rand() % 80 / (double)80);
		ColorsPoints[i * 4 + 3] = 1.0f;// rand() % 80 / (double)80;

		//printf("VerticesPoints[%d] = %f \n", i, VerticesPoints[i]);
	}

	glGenVertexArrays(3, VAO);
	glGenBuffers(9, VBO);

	//Triangle VAO
	// ---------------------------------------

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesTriangleEngine), VerticesTriangleEngine, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColorsTriangleEngine), ColorsTriangleEngine, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	//Cube VAO
	// ---------------------------------------

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesCube_T), VerticesCubeEngine, GL_DYNAMIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColorsCube_T), ColorsCubeEngine, GL_DYNAMIC_DRAW);

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



	//Points VAO
	// ---------------------------------------

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, pointNum * 3 * sizeof(float), VerticesPoints, GL_DYNAMIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, pointNum * 4 * sizeof(float), ColorsPoints, GL_DYNAMIC_DRAW);

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


	delete VerticesCubeEngine;
	delete ColorsCubeEngine;
}

void VaoEngine::Release()
{
	delete VerticesPoints;
	delete ColorsPoints;

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(6, VBO);
}

void VaoEngine::Render()
{

	// bind the VAO
	glBindVertexArray(VAO[0]);

	// render the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// unbind the VAO
	glBindVertexArray(0);



	// ------
	// bind the VAO
	glBindVertexArray(VAO[1]);

	// 12*3 indices starting at 0 -> 12 triangles
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	// unbind the VAO
	glBindVertexArray(0);


	// ------
	// bind the VAO
	glBindVertexArray(VAO[2]);

	// 12*3 indices starting at 0 -> 12 triangles
	glDrawArrays(GL_POINTS, 0, pointNum*3);

	// unbind the VAO
	glBindVertexArray(0);

}

