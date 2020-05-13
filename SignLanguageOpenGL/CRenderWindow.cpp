#include "CRenderWindow.h"

CRenderWindow::CRenderWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	m_Window = glfwCreateWindow(640, 480, "My Title", glfwGetPrimaryMonitor(), NULL);
}

void CRenderWindow::BindContext()
{
	glfwMakeContextCurrent(m_Window);
}

void CRenderWindow::SetRenderView()
{

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(m_Window, NULL);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CRenderWindow::Render()
{

}


CRenderWindow::~CRenderWindow()
{
	glfwTerminate();
}