#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
using namespace std;
class CRenderWindow
{
private:
	int m_Width;
	int m_Height;
	bool m_bAlpha;
	int m_PosX;
	int m_PosY;
	



public:
	CRenderWindow();
	void BindContext();
	void CRenderWindow::SetRenderView();

	~CRenderWindow();
	
};

