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

// imgui
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

// controls
#include "OpenGL/controls/Camera.h"
#include "OpenGL/controls/WorldModel.h"
#include "OpenGL/controls/PseudoColor.h"

// Object
#include "OpenGL/objects/ObjectQueue.h"

// Vao Engine
//#include "VaoEngine.h"

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
	p_BufferCPU_Points = new BufferCPU(SemaphoreSize, DataNum*PerDataSize_Points* sizeof(float));
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
	int BufferDataCount = 0;
	float Xoffset = 0;
	float* BufferData_Points = new float[pDataArray->DataNum* pDataArray->PerDataSize_Points];

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

			Xoffset += 1.0f;

			srand(unsigned(time(0)));   //获取系统时间 
			
			for (size_t i = 0; i < pDataArray->DataNum; i++)
			{
				BufferData_Points[i * 4] = (float)(rand() % 80) + Xoffset;
				BufferData_Points[i * 4 + 1] = (float)(rand() % 80);
				BufferData_Points[i * 4 + 2] = (float)(rand() % 80);
				BufferData_Points[i * 4 + 3] = (float)(rand() % 80);

				//printf("VerticesPoints[%d] = %f \n", i, VerticesPoints[i]);
			}

			pDataArray->p_BufferCPU_Points->EnList((char*)BufferData_Points);
			//printf("EnList BufferData %d \n", ++BufferDataCount);


			//if (BufferDataCount++ > 4000)
			//{
			//	running = false;
			//}

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
	float* BufferData_Points = new float[pDataArray->DataNum* pDataArray->PerDataSize_Points];

	int BufferDataCount = 0;
	//////////////////////////////////////////////////////////////////////////


	// glfw: initialize and configure
	// ------------------------------
	glfwSetErrorCallback(error_callback);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

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
	Shader m_Shader("OpenGL/shader/SimpleTransform.vertexshader", "OpenGL/shader/SingleColor.fragmentshader");

	// load models
	// -----------
	MeshModel m_MeshModel(std::string("OpenGL/data/car.obj"));//"OpenGL/data/nanosuit/nanosuit.obj"


	// Vao Engine
	// ---------------------------------------
	//VaoEngine m_VaoEngine;
	//m_VaoEngine.Init(&m_Shader);

	//Camera
	// ---------------------------------------
	Camera m_Camera;
	m_Camera.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//Model
	// ---------------------------------------
	WorldModel m_WorldModel;
	m_WorldModel.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//pseudo color
	// ---------------------------------------
	PseudoColor m_PseudoColor;
	m_PseudoColor.Init(&m_Shader, window, SCR_WIDTH, SCR_HEIGHT);

	//Object
	// ---------------------------------------
	ObjectQueue m_ObjectQueue;
	m_ObjectQueue.Init(20, 100);

	//m_ObjectQueue.Release();
	//m_ObjectQueue.AddData2Object();

	// Setup ImGui binding
	// ---------------------------------------
	ImGui_ImplGlfwGL3_Init(window, true);

	// Setup style
	// ---------------------------------------
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'extra_fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	double TimeStart_IMGUI;
	double TimeEnd_IMGUI;

	double TimeStart_SwapBuffers;
	double TimeEnd_SwapBuffers;

	double TimeStart_Object_Add;
	double TimeEnd_Object_Add;

	double TimeStart_Object_Render;
	double TimeEnd_Object_Render;

	double TimeStart_While;
	double TimeEnd_While;

	double TimeStart_Shader;
	double TimeEnd_Shader;

	double TimeStart_Input_And_Clear;
	double TimeEnd_Input_And_Clear;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		TimeStart_Input_And_Clear = glfwGetTime() * 1000;
		
		// input
		// -----
		processInput(window);

		// render

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TimeEnd_Input_And_Clear = glfwGetTime() * 1000;
		cout << "Time Input_And_Clear:" << TimeEnd_Input_And_Clear - TimeStart_Input_And_Clear << endl;

		TimeStart_Shader = glfwGetTime() * 1000;

		// Use our shader
		m_Shader.use();

		// Compute the matrix from keyboard and mouse input

		m_Camera.ComputeMatricesFromInputs();
		m_Camera.UpdateProjectionMatrix();
		m_Camera.UpdateViewMatrix();

		m_WorldModel.ComputeMatricesFromInputs();
		m_WorldModel.UpdateModelMatrix();


		TimeEnd_Shader = glfwGetTime() * 1000;
		cout << "Time Shader:" << TimeEnd_Shader - TimeStart_Shader << endl;

		m_PseudoColor.UpdateThreshold();
		m_PseudoColor.UpdatePseudoColor();



		TimeStart_While = glfwGetTime() * 1000;
		TimeStart_Object_Add = glfwGetTime() * 1000;

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
			
			// Simulate thread spending time on task


			pDataArray->p_BufferCPU_Points->DeList((char*)BufferData_Points);

			m_ObjectQueue.AddData2Object((char*)BufferData_Points);



			//printf("DeList BufferData %d Size(%d)\n", ++BufferDataCount, pDataArray->p_BufferCPU_Points->LengthList());

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

		TimeEnd_Object_Add = glfwGetTime() * 1000;
		cout << "Time Object_Add:" << TimeEnd_Object_Add - TimeStart_Object_Add << endl;
		TimeStart_Object_Render = glfwGetTime() * 1000;

		//m_VaoEngine.Render();

		m_ObjectQueue.RenderObject();

		m_MeshModel.Draw(m_Shader);

		TimeEnd_Object_Render = glfwGetTime() * 1000;
		cout << "Time Object_Render:" << TimeEnd_Object_Render - TimeStart_Object_Render << endl;

		TimeStart_IMGUI = glfwGetTime() * 1000;

		

		// ImGui
		// ------
		ImGui_ImplGlfwGL3_NewFrame();

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			static float f = 0.0f;
			ImGui::Text("Hello, world!");                           // Some text (you can use a format string too)
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats as a color
			if (ImGui::Button("Demo Window"))                       // Use buttons to toggle our bools. We could use Checkbox() as well.
				show_demo_window ^= 1;
			if (ImGui::Button("Another Window"))
				show_another_window ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			ImGui::End();
		}

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow().
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		// ImGui Render
		ImGui::Render();

		

		TimeEnd_IMGUI = glfwGetTime() * 1000;
		cout << "Time IMGUI:" << TimeEnd_IMGUI - TimeStart_IMGUI << endl;
		TimeStart_SwapBuffers = glfwGetTime() * 1000;


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		TimeEnd_SwapBuffers = glfwGetTime() * 1000;
		cout << "Time glfwSwapBuffers:" << TimeEnd_SwapBuffers - TimeStart_SwapBuffers << endl;

		TimeEnd_While = glfwGetTime() * 1000;
		cout << "Time While:" << TimeEnd_While - TimeStart_While << endl;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//m_VaoEngine.Release();
	m_ObjectQueue.Release();

	// ImGui Shutdown
	// ------------------------------------------------------------------
	ImGui_ImplGlfwGL3_Shutdown();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();


	// 全局变量
	running = false;

	delete BufferData_Points;

	return 0;
}