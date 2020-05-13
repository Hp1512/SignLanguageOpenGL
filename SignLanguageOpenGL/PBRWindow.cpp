#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
//#include "Shader.h";

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ogldev_camera.h"
#include <map>
#include"stb.h"

#include "ogldev_util.h"

#include "skinning_technique.h"

#include "ogldev_skinned_mesh.h"
#include "skinning_technique.h"
#include "ogldev_pipeline.h"
#include "libkbvh2/bvh.h";
#include "BoneShader.h"
#include "Shader.h"
//#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//unsigned int loadTexture(const char* path);
//unsigned int loadCubemap(std::vector<std::string>faces);


const int ScreenWidth = 800;
const int ScreenHeight = 600;

//Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float lastX = 400, lastY = 300;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	SkinningTechnique m_pEffect;
	DirectionalLight m_directionalLight;
	SkinnedMesh m_mesh;
	SkinnedMesh m_SkinMesh;
	Camera* m_pGameCamera;
	PersProjInfo m_persProjInfo;
	Vector3f m_position;

	BoneShader m_BoneShader;

	m_BoneShader = BoneShader();

	if (!m_BoneShader.Init())
	{
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.55f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

	m_BoneShader.Enable();
	Vector3f Pos(0.0f, 3.0f, -1.0f);
	Vector3f Target(0.0f, 0.0f, 1.0f);
	Vector3f Up(0.0, 1.0f, 0.0f);


	m_position = Vector3f(0.0f, 0.0f, 6.0f);
	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = ScreenWidth;
	m_persProjInfo.Width = ScreenHeight;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_pGameCamera = new Camera(ScreenWidth, ScreenHeight, Pos, Target, Up);

	m_pEffect = SkinningTechnique();


	if (!m_pEffect.Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pEffect.Enable();

	m_pEffect.SetColorTextureUnit(0);
	m_pEffect.SetDirectionalLight(m_directionalLight);
	m_pEffect.SetMatSpecularIntensity(0.0f);
	m_pEffect.SetMatSpecularPower(0);

	//if (!m_mesh.LoadMesh("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\BVH\\ssa.fbx")) {
	//	printf("Mesh load failed\n");
	//	return false;
	//}

	string modepath = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\Content\\XinhuaNawsAgency\\shouyu_r_qianyi01\\FX\\shouyu_r_qianyi01_Bind.fbx";
	// = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\Content\\HuoJianHua.fbx";

	if (!m_SkinMesh.LoadMesh(modepath))
	{
		printf("Mesh load failed\n");
		return false;
	}

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	while (!glfwWindowShouldClose(window))
	{
		//输入
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		m_pGameCamera->OnRender();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//m_pEffect->Enable();
		vector<Matrix4f> SkeleteTransform;
		vector<Matrix4f> BoneTransform;

		Matrix4f t = Matrix4f(1, 0, 0, 1,
			0, 1, 0, 1,
			0, 0, 1, 1,
			0, 0, 0, 1);
		//	Matrix4f t1 = Matrix4f(1, 0, 0, 2,
		//		0, 1, 0, 1,
		//		0, 0, 1, 1,
		//		0, 0, 0, 1);

		//	float RunningTime = GetRunningTime();

		//	Transforms.push_back(t1);
		//	Transforms.push_back(t);
		//	m_SkinMesh.BoneTransform(currentFrame, Transforms);
		m_SkinMesh.SkeleteTransform(currentFrame, SkeleteTransform, BoneTransform);
		//Transforms.push_back(t);
		//Transforms.push_back(t1);



		//m_BoneShader->SetBoneTransform(0, Transforms1);
	//	m_mesh.Render();
	//	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

		Pipeline p;
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		p.Scale(0.005f, 0.005f, 0.005f);

		Vector3f Pos(m_position);
		p.WorldPos(Vector3f(-1.0f, 0.0f, 6.0f));
		p.Rotate(0.0f, 0.0f, 0.0f);


		m_pEffect.Enable();
		for (unsigned int i = 0; i < BoneTransform.size(); i++) {

			m_pEffect.SetBoneTransform(i, t);
		}
		m_pEffect.SetEyeWorldPos(m_pGameCamera->GetPos());
		m_pEffect.SetWVP(p.GetWVPTrans());
		m_pEffect.SetWorldMatrix(p.GetWorldTrans());

		m_SkinMesh.RenderMesh();


		m_BoneShader.Enable();
		for (unsigned int k = 0; k < SkeleteTransform.size(); k++) {

			//std::cout << Transforms[i] << std::endl;
			m_BoneShader.SetBoneTransform(k, t);

		}
		p.WorldPos(Vector3f(1.0f, 0.0f, 6.0f));
		m_BoneShader.SetWVP(p.GetWVPTrans());
		m_BoneShader.SetWorldMatrix(p.GetWorldTrans());
		m_SkinMesh.RenderSkelete();

		//检查事件
		glfwSwapBuffers(window);
		//交换缓冲
		glfwPollEvents();
	}

	//glDeleteVertexArrays(1, &VAO);


	//glDeleteBuffers(1, &VBO);



	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);*/

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	//	camera.ProcessMouseMovement(xoffset,yoffset);

}

void tmpProcess(JOINT* joint,
	std::vector<glm::vec4>& vertices,
	std::vector<GLshort>& indices,
	GLshort                 parentIndex = 0)
{
	// translated vertex is last column(row of transponse) joint's matrix
	glm::vec4 translatedVertex = joint->matrix[3];

	vertices.push_back(translatedVertex);

	GLshort myindex = vertices.size() - 1;

	if (parentIndex != myindex)
	{
		indices.push_back(parentIndex);
		indices.push_back(myindex);
	}

	for (auto& child : joint->children)
		tmpProcess(child, vertices, indices, myindex);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll((float)yoffset);
}

/*unsigned int loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path:" << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

unsigned int loadCubemap(std::vector<std::string>faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		}
		else
		{
			std::cout << "Cubemap texture failed to load at path" << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;

}*/