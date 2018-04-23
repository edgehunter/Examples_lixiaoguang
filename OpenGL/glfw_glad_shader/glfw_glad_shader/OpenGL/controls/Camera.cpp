#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::Init(Shader* p_Shader, GLFWwindow* Window, const int Screen_Width, const int Screen_Height)
{
	ControlBase::Init(p_Shader, Window, Screen_Width, Screen_Height);

	// Initial position : on +Z
	Position = glm::vec3(0, 0, 500);

	// Initial horizontal angle : toward -Z
	HorizontalAngle = -3.14f;

	// Initial vertical angle : none
	VerticalAngle = 0.0f;

	// Initial Field of View
	InitialFoV = 45.0f;

	Zoom = 0.0f;

	SCR_Left = 0;
	SCR_Top = 0;

}

glm::mat4 Camera::GetViewMatrix()
{
	return ViewMatrix;
}

void Camera::UpdateViewMatrix()
{
	// view transformations
	p_Shader->setMat4("View", ViewMatrix);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return ProjectionMatrix;
}

void Camera::UpdateProjectionMatrix()
{
	// projection transformations
	p_Shader->setMat4("Projection", ProjectionMatrix);
	
}

void Camera::ComputeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);


	//printf("deltaTime=(%f) ", deltaTime);


	// Reset mouse position for next frame
	//glfwSetCursorPos(window, 600 / 2, 400 / 2);

	glfwGetCursorPos(Window, &xPos, &yPos);

	// Reset mouse position for next frame
	//glfwSetCursorPos(window, 600 / 2, 400 / 2);

	// Compute new orientation
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		HorizontalAngle += MouseSpeed * float(xPos_old - xPos);
		VerticalAngle += MouseSpeed * float(yPos_old - yPos);
	}

	//Zoom
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		//InitialFoV += 1;
		Zoom += MouseSpeed * float(yPos_old - yPos);
	}


	xPos_old = xPos, yPos_old = yPos;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(VerticalAngle) * sin(HorizontalAngle),
		sin(VerticalAngle),
		cos(VerticalAngle) * cos(HorizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(HorizontalAngle - 3.14f / 2.0f),
		0,
		cos(HorizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Strafe up
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS){
		Position += up * deltaTime * KeySpeed;
	}
	// Strafe down
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS){
		Position -= up * deltaTime * KeySpeed;
	}
	// Strafe right
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS){
		Position += right * deltaTime * KeySpeed;
	}
	// Strafe left
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS){
		Position -= right * deltaTime * KeySpeed;
	}


	/**/

	// Move forward
	if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS){
		Position += direction * deltaTime * KeySpeed;
		//InitialFoV -= 1;
	}
	// Move backward
	if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS){
		Position -= direction * deltaTime * KeySpeed;
		//InitialFoV += 1;
	}


	float FoV = InitialFoV + 5*Zoom;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// GetWindowSize
	glfwGetWindowSize(Window, &Screen_Width, &Screen_Height);

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), (float)Screen_Width / (float)Screen_Height, 0.1f, 1000.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		Position,           // Camera is here
		Position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

	/*
	printf("horizontalAngle=(%f), verticalAngle=(%f) \n", HorizontalAngle, VerticalAngle);
	printf("position=(%f,%f,%f), direction=(%f,%f,%f), up=(%f,%f,%f) \n",
		Position.x, Position.y, Position.z, direction.x, direction.y, direction.z, up.x, up.y, up.z);

	*/

}

void Camera::UpdataViewPort()
{

	// SetWindowPos

	if (glfwGetKey(Window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		SCR_Left += 10;

		glfwSetWindowPos(Window,
			SCR_Left,
			SCR_Top);
	}

	if (glfwGetKey(Window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		SCR_Left -= 10;

		glfwSetWindowPos(Window,
			SCR_Left,
			SCR_Top);
	}

	if (glfwGetKey(Window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		SCR_Top += 10;

		glfwSetWindowPos(Window,
			SCR_Left,
			SCR_Top);
	}

	if (glfwGetKey(Window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		SCR_Top -= 10;

		glfwSetWindowPos(Window,
			SCR_Left,
			SCR_Top);
	}


	// SetWindowSize
	
	if (glfwGetKey(Window, GLFW_KEY_F5) == GLFW_PRESS)
	{
		Screen_Width += 10;

		glfwSetWindowSize(Window,
			Screen_Width,
			Screen_Height);
	}

	if (glfwGetKey(Window, GLFW_KEY_F6) == GLFW_PRESS)
	{
		Screen_Width -= 10;

		glfwSetWindowSize(Window,
			Screen_Width,
			Screen_Height);
	}

	if (glfwGetKey(Window, GLFW_KEY_F7) == GLFW_PRESS)
	{
		Screen_Height += 10;

		glfwSetWindowSize(Window,
			Screen_Width,
			Screen_Height);
	}

	if (glfwGetKey(Window, GLFW_KEY_F8) == GLFW_PRESS)
	{
		Screen_Height -= 10;

		glfwSetWindowSize(Window,
			Screen_Width,
			Screen_Height);
	}

}