// glfw_MultipleThread_NoEvent.cpp : Defines the entry point for the console application.
//


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stdafx.h"
#include <stdlib.h>

#include <strsafe.h>

#include "linmath.h"
#include <math.h>

/////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////
#define BUF_SIZE 255
#define MAX_THREADS 10

DWORD WINAPI GL_ThreadFunction(LPVOID lpParam);
void ErrorHandler(LPTSTR lpszFunction);

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).

struct GL_ThreadData 
{
	GLFWwindow* Window;
	const char* Title;
	DWORD ThreadID;
	//HANDLE  ThreadHandle;
	int Value;
};

static volatile int running = GLFW_TRUE;

void ErrorHandler(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code.

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message.

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	// Free error-handling buffer allocations.

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


/////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////
static const struct
{
	float x, y;
	float r, g, b;
} vertices[3] =
{
	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
	{ 0.6f, -0.4f, 0.f, 1.f, 0.f },
	{ 0.f, 0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void update_window_title(GLFWwindow* window, double frame_rate)
{
	char title[256];

	sprintf_s(title, "glfw_MultipleThread_NoEvent ( %0.1f Hz)", frame_rate);

	glfwSetWindowTitle(window, title);
}

DWORD WINAPI GL_ThreadFunction(LPVOID lpParam)
{
	HANDLE hStdout;
	GL_ThreadData* pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	// Make sure there is a console to receive output results. 

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because 
	// it was checked for NULL before the thread was created.

	//memcpy(pDataArray, lpParam, sizeof(GL_ThreadData));
	pDataArray = (GL_ThreadData*)lpParam;

	// glfw
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	unsigned long frame_count = 0;
	double last_time, current_time;
	double frame_rate;

	//glfwSetKeyCallback(pDataArray->Window, key_callback);
	//glfwSetErrorCallback(error_callback);

	glfwMakeContextCurrent(pDataArray->Window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	last_time = glfwGetTime();

	// NOTE: OpenGL error checks have been omitted for brevity

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
		sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(float) * 5, (void*)(sizeof(float) * 2));

	while (running)
	{
		float ratio;
		int width, height;
		mat4x4 m, p, mvp;

		glfwGetFramebufferSize(pDataArray->Window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		mat4x4_identity(m);
		mat4x4_rotate_Z(m, m, (float)glfwGetTime());
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(pDataArray->Window);
		glfwPollEvents();

		frame_count++;

		current_time = glfwGetTime();
		if (current_time - last_time > 1.0)
		{
			frame_rate = frame_count / (current_time - last_time);
			frame_count = 0;
			last_time = current_time;
			//update_window_title(pDataArray->Window, frame_rate);

			// Print the parameter values using thread-safe functions.
			StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d, %s, %f\n"),
				pDataArray->ThreadID, pDataArray->Value, pDataArray->Title, frame_rate);
			StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
			WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
		}

	}

	//glfwDestroyWindow(pDataArray->Window);

	//glfwTerminate();
	//exit(EXIT_SUCCESS);

	glfwMakeContextCurrent(NULL);

	return 0;
}

/////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////

int main(void)
{
	// Init glfw
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	// Init worker threads Handles.
	GL_ThreadData* pDataArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];

	// Create MAX_THREADS worker threads.

	for (int i = 0; i<MAX_THREADS; i++)
	{
		// Allocate memory for thread data.

		pDataArray[i] = (GL_ThreadData*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(GL_ThreadData));

		if (pDataArray[i] == NULL)
		{
			// If the array allocation fails, the system is out of memory
			// so there is no point in trying to print an error message.
			// Just terminate execution.
			ExitProcess(2);
		}

		// Generate unique data for each thread to work with.

		pDataArray[i]->Value = i;

		// Create glfw Context
		pDataArray[i]->Window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
		if (!pDataArray[i]->Window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwShowWindow(pDataArray[i]->Window);

		// Create the thread to begin execution on its own.

		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			GL_ThreadFunction,       // thread function name
			pDataArray[i],          // argument to thread function 
			0,                      // use default creation flags 
			&pDataArray[i]->ThreadID);   // returns the thread identifier 


		// Check the return value for success.
		// If CreateThread fails, terminate execution. 
		// This will automatically clean up threads and memory. 

		if (hThreadArray[i] == NULL)
		{
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	} // End of main thread creation loop.

	//glfwMakeContextCurrent(pDataArray[0]->Window);
	//gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwMakeContextCurrent(NULL);

	while (running)
	{
		glfwWaitEvents();

		for (int i = 0; i < MAX_THREADS; i++)
		{
			if (glfwWindowShouldClose(pDataArray[i]->Window))
				running = GLFW_FALSE;
		}
	}

	for (int i = 0; i < MAX_THREADS; i++)
	{
		glfwHideWindow(pDataArray[i]->Window);
	}
		
	// Wait until all threads have terminated.

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.

	for (int i = 0; i<MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;    // Ensure address is not reused.
		}
	}

	return 0;
}

//! [code]

