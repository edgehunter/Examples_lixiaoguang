#pragma once

#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	);



bool loadAssImp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
	);



//≥ı ºªØ

/*

// 3DMax Obj simple
// ---------------------------------------

// Read our .obj file
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // Won't be used at the moment.
bool res = loadOBJ("OpenGL/data/cube.obj", vertices, uvs, normals);

// Init VAO VBO
GLuint vertexarray;
glGenVertexArrays(1, &vertexarray);

GLuint vertexbuffer;
glGenBuffers(1, &vertexbuffer);

GLuint uvbuffer;
glGenBuffers(1, &uvbuffer);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
glBindVertexArray(vertexarray);

// Load it into a VBO
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

// 1rst attribute buffer : vertices
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glVertexAttribPointer(
	0,                  // attribute
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

// 2nd attribute buffer : UVs
glEnableVertexAttribArray(2);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glVertexAttribPointer(
	2,                                // attribute
	2,                                // size
	GL_FLOAT,                         // type
	GL_FALSE,                         // normalized?
	0,                                // stride
	(void*)0                          // array buffer offset
	);


// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
glBindVertexArray(0);

*/

//‰÷»æ

/*

// bind the VAO
glBindVertexArray(vertexarray);

// Draw the triangle !
glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

// unbind the VAO
glBindVertexArray(0);

*/