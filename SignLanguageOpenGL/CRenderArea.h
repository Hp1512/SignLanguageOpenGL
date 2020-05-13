#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
class CRenderArea
{
private:
	int m_PosX;
	int m_PosY;
	int m_Height;
	int m_Width;
public:
	CRenderArea();
	static void monitor_callback(GLFWmonitor* monitor, int event);
	~CRenderArea();
};

