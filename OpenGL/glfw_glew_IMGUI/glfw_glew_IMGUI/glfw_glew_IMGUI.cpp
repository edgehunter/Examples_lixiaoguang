// glfw_glew_IMGUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define GLEW_STATIC

#include <GLEW/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

/* 注意：#define GLEW_STATIC 在 #include<GL/glew.h>前面，
* #include<GL/glew.h>在#include<GLFW/glfw3.h>前面
*/
using namespace std;

/*创建键盘输入的回调函数*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
	/*初始化GLFW*/
	glfwInit();
	/*配置GLFW窗口*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	/*创建窗口*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	/*通知GLFW将我们窗口的上下文设置为当前线程的主上下文*/
	glfwMakeContextCurrent(window);

	/*GLEW是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLEW*/
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}
	/*告诉OpenGL渲染窗口的尺寸大小，这样OpenGL才只能知道怎样相对于窗口大小显示数据和坐标*/
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/*注册回调函数*/
	glfwSetKeyCallback(window, key_callback);
	/*事件循环*/
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		/*函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），
		*然后调用对应的回调函数（可以通过回调方法手动设置）。
		我们一般在游戏循环的开始调用事件处理函数
		*/

		// 这里写渲染指令


		glfwSwapBuffers(window);
		/*函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲），
		*它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		*/
	}
	/*释放GLFW分配的内存*/
	glfwTerminate();

	return 0;
}
