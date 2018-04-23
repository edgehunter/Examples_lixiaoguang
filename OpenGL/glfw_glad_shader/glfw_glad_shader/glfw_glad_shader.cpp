// glfw_glad_shader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cmath>
#include <Strsafe.h>
#include <time.h>

// glad
#include <glad/glad.h>

#include "OpenGL/shader/shader.h"

// glfw
#include <GLFW/glfw3.h>

// controls
#include "OpenGL/controls/Camera.h"
#include "OpenGL/controls/MouseClick.h"
#include "OpenGL/controls/WorldModel.h"
#include "OpenGL/controls/PseudoColor.h"
#include "OpenGL/controls/ImguiControl.h"

// Object
#include "OpenGL/objects/ObjectQueue.h"

// AuxiliaryModel
#include "OpenGL/objects/Auxiliary/AuxiliaryModel.h"

#include "OpenGL/objects/assimp/mesh.h"
#include "OpenGL/objects/assimp/MeshModel.h"

// Buffer
#include "DataProcess/BufferCPU.h"


// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).

#define BUF_SIZE 255
#define MAX_THREADS 2

struct ThreadParam
{
	GLFWwindow* Window;
	const char* Title;
	DWORD ThreadID;
	//HANDLE  ThreadHandle;
	int Value;

	int DataNum;
	int PerDataSize_Points;
	int SemaphoreSize;//信号量最大值

	BufferCPU* p_BufferCPU_Points;

	HANDLE ghSemaphore_Empty;
	HANDLE ghSemaphore_Full;
};

bool running = true;


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}


void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


void ErrorHandler(LPTSTR lpszFunction);
DWORD WINAPI ThreadFunction_GeneratePoints(LPVOID lpParam);
DWORD WINAPI ThreadFunction_OpenGLRendering(LPVOID lpParam);


//////////////////////////////////////////////////////////////////////////
//
// main
//
//////////////////////////////////////////////////////////////////////////

int main(void)
{
	int DataNum = 128 * 128;//共128*128个数据点
	int PerDataSize_Points = 4;//每个数据点坐标包含3个float，x、y、z、w
	int SemaphoreSize = 10;//信号量最大值

	// Buffer
	BufferCPU* p_BufferCPU_Points;
	p_BufferCPU_Points = new BufferCPU(SemaphoreSize, DataNum*PerDataSize_Points* sizeof(float) + sizeof(OpenGLHeader));
	p_BufferCPU_Points->CreateList();


	// Semaphore
	HANDLE ghSemaphore_Empty;
	HANDLE ghSemaphore_Full;


	// Create a semaphore with initial and max counts of MAX_SEM_COUNT

	ghSemaphore_Empty = CreateSemaphore(
						NULL,           // default security attributes
						SemaphoreSize,  // initial count
						SemaphoreSize,  // maximum count
						NULL);          // unnamed semaphore

	if (ghSemaphore_Empty == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}


	// Create a semaphore with initial and max counts of 0

	ghSemaphore_Full = CreateSemaphore(
						NULL,           // default security attributes
						0,				// initial count
						SemaphoreSize,  // maximum count
						NULL);          // unnamed semaphore

	if (ghSemaphore_Full == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}







	// Init worker threads Handles.
	ThreadParam* pDataArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];

	// Create MAX_THREADS worker threads.

	for (int i = 0; i < MAX_THREADS; i++)
	{
		// Allocate memory for thread data.

		pDataArray[i] = (ThreadParam*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(ThreadParam));

		if (pDataArray[i] == NULL)
		{
			// If the array allocation fails, the system is out of memory
			// so there is no point in trying to print an error message.
			// Just terminate execution.
			ExitProcess(2);
		}

		// Generate unique data for each thread to work with.

		switch (i)
		{
		case 0:
			{
				pDataArray[i]->Value = i;

				// Create glfw Context
				pDataArray[i]->Window = NULL;

				// Create glfw Title
				pDataArray[i]->Title = NULL;

				//
				pDataArray[i]->DataNum = DataNum;
				pDataArray[i]->PerDataSize_Points = PerDataSize_Points;
				pDataArray[i]->SemaphoreSize = SemaphoreSize;

				pDataArray[i]->p_BufferCPU_Points = p_BufferCPU_Points;

				pDataArray[i]->ghSemaphore_Empty = ghSemaphore_Empty;
				pDataArray[i]->ghSemaphore_Full = ghSemaphore_Full;

				// Create the thread to begin execution on its own.

				hThreadArray[i] = CreateThread(
					NULL,                   // default security attributes
					0,                      // use default stack size  
					ThreadFunction_GeneratePoints,       // thread function name
					pDataArray[i],          // argument to thread function 
					0,                      // use default creation flags 
					&pDataArray[i]->ThreadID);   // returns the thread identifier 

				break;
			}


		case 1:
			{

				pDataArray[i]->Value = i;

				// Create glfw Context
				pDataArray[i]->Window = NULL;

				// Create glfw Title
				pDataArray[i]->Title = NULL;

				//
				pDataArray[i]->DataNum = DataNum;
				pDataArray[i]->PerDataSize_Points = PerDataSize_Points;
				pDataArray[i]->SemaphoreSize = SemaphoreSize;

				pDataArray[i]->p_BufferCPU_Points = p_BufferCPU_Points;

				pDataArray[i]->ghSemaphore_Empty = ghSemaphore_Empty;
				pDataArray[i]->ghSemaphore_Full = ghSemaphore_Full;

				// Create the thread to begin execution on its own.

				hThreadArray[i] = CreateThread(
					NULL,                   // default security attributes
					0,                      // use default stack size  
					ThreadFunction_OpenGLRendering,       // thread function name
					pDataArray[i],          // argument to thread function 
					0,                      // use default creation flags 
					&pDataArray[i]->ThreadID);   // returns the thread identifier 

				break;
			}
			

		default:
			break;
		}


		// Check the return value for success.
		// If CreateThread fails, terminate execution. 
		// This will automatically clean up threads and memory. 

		if (hThreadArray[i] == NULL)
		{
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	} // End of main thread creation loop.


	//system("pause");

	// Wait until all threads have terminated.

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.

	for (int i = 0; i < MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;    // Ensure address is not reused.
		}
	}

	p_BufferCPU_Points->ReleaseList();
	delete p_BufferCPU_Points;

	CloseHandle(ghSemaphore_Empty);
	CloseHandle(ghSemaphore_Full);

	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


//The callback function receives two - dimensional scroll offsets.
//A simple mouse wheel, being vertical, provides offsets along the Y - axis.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	printf("xoffset=(%f), yoffset=(%f) \n", xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);

	printf("width=(%d), height=(%d) \n", width, height);
}


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

DWORD WINAPI ThreadFunction_GeneratePoints(LPVOID lpParam)
{
	HANDLE hStdout;
	ThreadParam* pDataArray;

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

	pDataArray = (ThreadParam*)lpParam;

	// Print the parameter values using thread-safe functions.
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d, %s, %d, %d, %d, %d\n"),
		pDataArray->ThreadID, pDataArray->Value, pDataArray->Title, pDataArray->DataNum, pDataArray->PerDataSize_Points, pDataArray->SemaphoreSize);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);


	// 数据进入队列

	DWORD dwWaitResult;
	//int BufferDataCount = 0;
	float Xoffset = 0;

	char* BufferData_Points = new char[pDataArray->DataNum* pDataArray->PerDataSize_Points* sizeof(float) + sizeof(OpenGLHeader)];
	OpenGLHeader* p_OpenGLHeader = new OpenGLHeader();

	while (running)
	{
		// Try to enter the semaphore gate.

		dwWaitResult = WaitForSingleObject(
			pDataArray->ghSemaphore_Empty,   // handle to semaphore
			0L);           // zero-second time-out interval

		switch (dwWaitResult)
		{
			// The semaphore object was signaled.
		case WAIT_OBJECT_0:
			
			// TODO: Perform task
			//printf("Thread %d: wait succeeded\n", GetCurrentThreadId());


			// Simulate thread spending time on task

			BufferData_Points += sizeof(OpenGLHeader);

			Xoffset += 1.0f;

			srand(unsigned(time(0)));   //获取系统时间 
			
			for (size_t i = 0; i < pDataArray->DataNum; i++)
			{
				((float*)BufferData_Points)[i * 4] = (float)(rand() % 80) + Xoffset;
				((float*)BufferData_Points)[i * 4 + 1] = (float)(rand() % 80);
				((float*)BufferData_Points)[i * 4 + 2] = (float)(rand() % 80);
				((float*)BufferData_Points)[i * 4 + 3] = (float)(rand() % 80);

				//printf("VerticesPoints[%d] = %f \n", i, VerticesPoints[i]);
			}

			
			p_OpenGLHeader->mMotionVector.CurrentPosition.x = ((float*)BufferData_Points)[0];
			p_OpenGLHeader->mMotionVector.CurrentPosition.y = 0;// ((float*)BufferData_Points)[1];
			p_OpenGLHeader->mMotionVector.CurrentPosition.z = 0;// ((float*)BufferData_Points)[2];


			BufferData_Points -= sizeof(OpenGLHeader);
			memcpy(BufferData_Points, p_OpenGLHeader,sizeof(OpenGLHeader));

			pDataArray->p_BufferCPU_Points->EnList(BufferData_Points);
			//printf("EnList BufferData %d \n", ++BufferDataCount);

			// Release the semaphore when task is finished

			if (!ReleaseSemaphore(
				pDataArray->ghSemaphore_Full,  // handle to semaphore
				1,            // increase count by one
				NULL))       // not interested in previous count
			{
				printf("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;

			// The semaphore was nonsignaled, so a time-out occurred.
		case WAIT_TIMEOUT:
			//printf("Thread %d: wait timed out\n", GetCurrentThreadId());
			break;
		}

	}

	delete BufferData_Points;
	delete p_OpenGLHeader;

	return 0;
}

DWORD WINAPI ThreadFunction_OpenGLRendering(LPVOID lpParam)
{

	HANDLE hStdout;
	ThreadParam* pDataArray;

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

	pDataArray = (ThreadParam*)lpParam;

	// Print the parameter values using thread-safe functions.
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d, %s\n"),
		pDataArray->ThreadID, pDataArray->Value, pDataArray->Title);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);


	DWORD dwWaitResult;
	char* BufferData_Points = new char[pDataArray->DataNum* pDataArray->PerDataSize_Points* sizeof(float) + sizeof(OpenGLHeader)];

	//////////////////////////////////////////////////////////////////////////


	// glfw: initialize and configure
	// ------------------------------
	glfwSetErrorCallback(error_callback);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	// settings
	unsigned int SCR_WIDTH = 1280;
	unsigned int SCR_HEIGHT = 720;

	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set FPS
	// ---------------------------------------
	glfwSwapInterval(0);


	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	glClearDepth(1.0f);         //  深度缓存设置
	
	glEnable(GL_DEPTH_TEST);       //  打开深度测试
	
	glDepthFunc(GL_LEQUAL);        //  设置深度测试类型

	// Really Nice Perspective Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE); // cull face

	// cull back face
	//glCullFace(GL_BACK); 

	// GL_CCW for counter clock-wise
	//glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// build and compile shaders
	// -------------------------
	Shader m_Shader("OpenGL/shader/SimpleTransform.vertexshader", //D:/lixiaoguang_works/Examples_lixiaoguang/OpenGL/glfw_glad_shader/glfw_glad_shader/
		"OpenGL/shader/SingleColor.fragmentshader");

	// load models
	// -----------
	MeshModel m_MeshModel(std::string("OpenGL/data/car.obj"));//D:/lixiaoguang_works/Examples_lixiaoguang/OpenGL/glfw_glad_shader/glfw_glad_shader/

	// AuxiliaryModel
	// ---------------------------------------
	AuxiliaryModel m_AuxiliaryModel;
	m_AuxiliaryModel.Init();

	// OpenGLHeader
	// ---------------------------------------
	OpenGLHeader m_OpenGLHeader;
	m_OpenGLHeader = OpenGLHeader();

	//Camera
	// ---------------------------------------
	Camera m_Camera;
	m_Camera.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//MouseClick
	// ---------------------------------------
	MouseClick m_MouseClick;
	m_MouseClick.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//Model
	// ---------------------------------------
	WorldModel m_WorldModel;
	m_WorldModel.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//pseudo color
	// ---------------------------------------
	PseudoColor m_PseudoColor;
	m_PseudoColor.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//ImguiControl
	// ---------------------------------------
	ImguiControl m_ImguiControl;
	m_ImguiControl.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//Object
	// ---------------------------------------
	ObjectQueue m_ObjectQueue;
	m_ObjectQueue.Init(20, 100);

	
	double TimeStart_SwapBuffers;
	double TimeEnd_SwapBuffers;

	double TimeStart_Object_Add;
	double TimeEnd_Object_Add;

	double TimeStart_Object_Render;
	double TimeEnd_Object_Render;

	double TimeStart_Shader;
	double TimeEnd_Shader;


	//int FramebufferSizeWidth, FramebufferSizeHeight;
	//glfwGetFramebufferSize(window, &FramebufferSizeWidth, &FramebufferSizeHeight);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// input
		processInput(window);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Time
		//*********
		TimeStart_Shader = glfwGetTime() * 1000;

		// Use our shader
		m_Shader.use();

		// Compute Camera matrix from keyboard and mouse input
		m_Camera.ComputeMatricesFromInputs();
		m_Camera.UpdateProjectionMatrix();
		m_Camera.UpdateViewMatrix();
		m_Camera.UpdataViewPort();

		// Compute WorldModel matrix from keyboard and mouse input
		m_WorldModel.ComputeMatricesFromInputs();
		m_WorldModel.UpdateModelMatrix();

		// Time
		//*********
		TimeEnd_Shader = glfwGetTime() * 1000;
		cout << "Time Shader:" << TimeEnd_Shader - TimeStart_Shader << endl;

		// PseudoColor
		m_PseudoColor.UpdateThreshold();
		m_PseudoColor.UpdatePseudoColor();

		// Time
		//*********
		TimeStart_Object_Add = glfwGetTime() * 1000;

		// ObjectQueue
		m_ObjectQueue.UpdateFromInputs(window);

		// 数据出队列

		// Try to enter the semaphore gate.

		dwWaitResult = WaitForSingleObject(
			pDataArray->ghSemaphore_Full,   // handle to semaphore
			0L);           // zero-second time-out interval

		switch (dwWaitResult)
		{
			// The semaphore object was signaled.
		case WAIT_OBJECT_0:
			
			// TODO: Perform task
			//printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
			
			pDataArray->p_BufferCPU_Points->DeList(BufferData_Points);
			memcpy(&m_OpenGLHeader, BufferData_Points, sizeof(OpenGLHeader));

			// ObjectQueue
			m_ObjectQueue.AddData2Object(BufferData_Points + sizeof(OpenGLHeader));

			m_WorldModel.UpdateMotionCompensation(&m_OpenGLHeader);

			// Release the semaphore when task is finished

			if (!ReleaseSemaphore(
				pDataArray->ghSemaphore_Empty,  // handle to semaphore
				1,            // increase count by one
				NULL))       // not interested in previous count
			{
				printf("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;

			// The semaphore was nonsignaled, so a time-out occurred.
		case WAIT_TIMEOUT:
			//printf("Thread %d: wait timed out\n", GetCurrentThreadId());
			break;
		}


		// Time
		//*********
		TimeEnd_Object_Add = glfwGetTime() * 1000;
		cout << "Time Object_Add:" << TimeEnd_Object_Add - TimeStart_Object_Add << endl;
		TimeStart_Object_Render = glfwGetTime() * 1000;

		// ObjectQueue
		m_ObjectQueue.RenderObject();

		// MeshModel
		m_MeshModel.Draw(m_Shader);

		// AuxiliaryModel
		m_AuxiliaryModel.RanderVAO();

		// MouseClick
		m_MouseClick.Click(m_WorldModel.GetModelMatrix(), m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());

		// ImguiControl
		m_ImguiControl.Render();


		// Time
		//*********
		TimeEnd_Object_Render = glfwGetTime() * 1000;
		cout << "Time Object_Render:" << TimeEnd_Object_Render - TimeStart_Object_Render << endl;
		TimeStart_SwapBuffers = glfwGetTime() * 1000;


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();


		// Time
		//*********
		TimeEnd_SwapBuffers = glfwGetTime() * 1000;
		cout << "Time glfwSwapBuffers:" << TimeEnd_SwapBuffers - TimeStart_SwapBuffers << endl;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	// ObjectQueue
	m_ObjectQueue.Release();

	// AuxiliaryModel
	m_AuxiliaryModel.Release();

	// MouseClick
	m_MouseClick.Release();

	// ImguiControl
	m_ImguiControl.Shutdown();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();


	// 全局变量
	running = false;

	delete BufferData_Points;

	return 0;
}