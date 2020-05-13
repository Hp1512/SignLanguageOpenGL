#pragma once
#include<glad/glad.h>
#define GLFW_EXPOSE_NATIVE_NSGL
#include <GLFW/glfw3native.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include"stb.h"

#include "BoneShader.h"
#include "PBRMaterial.h"
#include "CCamera.h"
#include "CModel.h"
#include "CAnimation.h"
#include "CSkeletonMaps.h"
#include "CRenderArea.h"
#include <Windows.h> 


void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void SetRenderWindowParent(GLFWwindow* window, HWND hwnd);
void SetRenderWindowRect(GLFWwindow* window, RECT sWndRect);
const int ScreenWidth = 200;
const int ScreenHeight = 400;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
CCamera Ccamera;

CModel* HuoJianHua;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
	glfwSetWindowPos(window, 100, 300);
	if (window == NULL)
	{
		std::cout << "Failed to Creat GLFW window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	glfwSetFramebufferSizeCallback(window, NULL);

	glEnable(GL_DEPTH_TEST);
	
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//设置摄像机
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	Ccamera = CCamera(cameraPos);

	//加载模型
	string modelPath = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\shouyu_r_qianyi01_Bind.fbx";
	HuoJianHua = new CModel(modelPath);
	HuoJianHua->BindModelBufferData(0);

	
	string animationPath = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\BVH\\_001.bvh";
	CAnimationController* controller= HuoJianHua->GetAnimationController();
	controller->AddAnimations(animationPath);
	controller->SetLoop(true);
	controller->SetAnimationSpeed(2);
	controller->StartAnimations();

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	


	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);





/*	GLFWwindow* second_window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, window);
	glfwSetWindowPos(second_window, 500, 300);
	if (second_window == NULL)
	{
		std::cout << "Failed to Creat GLFW window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(second_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	glfwSetFramebufferSizeCallback(second_window, NULL);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	HuoJianHua.BindModelBufferData(1);

	glfwSetCursorPosCallback(second_window, mouse_callback);

	glfwGetFramebufferSize(second_window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	HuoJianHua.SetShowSkeletonState(true);*/

	HuoJianHua->TransformModel(glm::vec3(0, -100, 0), glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f));

	while (!glfwWindowShouldClose(window))
	{
		//输入
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window);
		glfwMakeContextCurrent(window);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(Ccamera.GetZoom()), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
		HuoJianHua->SetProjection(projection);
		HuoJianHua->SetView(Ccamera.GetViewMatrix());	
		HuoJianHua->Draw(0);
		glfwSwapInterval(1);
		glfwSwapBuffers(window);
		//交换缓冲
		glfwPollEvents();

	/*	processInput(second_window);
		glfwMakeContextCurrent(second_window);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		projection = glm::perspective(glm::radians(Ccamera.GetZoom()), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
		HuoJianHua.SetProjection(projection);
		HuoJianHua.SetView(Ccamera.GetViewMatrix());
		HuoJianHua.TransformModel(glm::vec3(0, -100, 0), glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f));
		HuoJianHua.Draw(1);
		glfwSwapInterval(1);
		glfwSwapBuffers(second_window);
		//交换缓冲
		glfwPollEvents();*/

	}

	glfwTerminate();
	return 0;
}




void SetRenderWindowParent(GLFWwindow* window, HWND hwnd)
{
	RECT sWndRect;
	GetWindowRect(hwnd, &sWndRect);
	SetRenderWindowRect(window, sWndRect);

}


void SetRenderWindowRect(GLFWwindow* window, RECT sWndRect)
{
	glfwSetWindowPos(window, sWndRect.left, sWndRect.top);
	int iWidth = sWndRect.right - sWndRect.left;
	int iHeight= sWndRect.top - sWndRect.bottom;
	glfwGetWindowSize(window, &iWidth, &iHeight);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
/*	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Ccamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Ccamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Ccamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Ccamera.ProcessKeyboard(RIGHT, deltaTime);*/

}

int i = 0;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	i++;
	cout << "当前索引为：" << i << endl;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (state == GLFW_PRESS)
	{
		HuoJianHua->TransformModel(glm::vec3(0, -100, 0), glm::vec3(0, 90*i, 0), glm::vec3(0.01f, 0.01f, 0.01f));
	}
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	
}