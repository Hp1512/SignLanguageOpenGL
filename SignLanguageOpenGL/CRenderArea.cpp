#include "CRenderArea.h"

CRenderArea::CRenderArea()
{
    m_PosX = 0;
    m_PosY = 0;
    m_Width = 200;
    m_Height = 200;
	//int count;
	//GLFWmonitor** monitors = glfwGetMonitors(&count);
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
	glfwSetMonitorCallback(monitor_callback);

    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    glfwGetMonitorWorkarea(primary, &m_PosX, &m_PosY, &m_Width, &m_Height);

    const char* name = glfwGetMonitorName(primary);
}

void CRenderArea::monitor_callback(GLFWmonitor* monitor, int event)
{
    if (event == GLFW_CONNECTED)
    {
        int a = 0;
        // The monitor was connected
    }
    else if (event == GLFW_DISCONNECTED)
    {
        int a = 0;
        // The monitor was disconnected
    }
}

CRenderArea::~CRenderArea()
{

}