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
#include "PBRMaterial.h"
#include "TCamera.h"
//#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
//unsigned int loadCubemap(std::vector<std::string>faces);


const int ScreenWidth=800;
const int ScreenHeight=600;

//Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float lastX = 400, lastY = 300;

TCamera Tcamera;

int main1()
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

	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// set depth function to less than AND equal for skybox depth trick.
	//glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

//	SkinningTechnique m_pEffect;
//	DirectionalLight m_directionalLight;
	//SkinnedMesh m_mesh;
	SkinnedMesh m_SkinMesh;
	SkinnedMesh m_SkeleteMesh;
	//Camera* m_pGameCamera;
	//PersProjInfo m_persProjInfo;
	//Vector3f m_position;

	//BoneShader m_BoneShader;

	//m_BoneShader = BoneShader();

	//if (!m_BoneShader.Init())
	//{
	//	printf("Error initializing the lighting technique\n");
	//	return false;
//	}

//	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
//	m_directionalLight.AmbientIntensity = 0.55f;
//	m_directionalLight.DiffuseIntensity = 0.9f;
	//m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

	//m_BoneShader.Enable();
	//Vector3f Pos(0.0f, 3.0f, -1.0f);
	//Vector3f Target(0.0f, 0.0f, 1.0f);
	//Vector3f Up(0.0, 1.0f, 0.0f);

	
	//m_position = Vector3f(0.0f, 0.0f, 6.0f);
	//m_persProjInfo.FOV = 60.0f;
	//m_persProjInfo.Height = ScreenWidth;
	//m_persProjInfo.Width = ScreenHeight;
	//m_persProjInfo.zNear = 1.0f;
	//m_persProjInfo.zFar = 100.0f;

	//m_pGameCamera = new Camera(ScreenWidth, ScreenHeight, Pos, Target, Up);

//	m_pEffect = SkinningTechnique();

	
	//if (!m_pEffect.Init()) {
	//	printf("Error initializing the lighting technique\n");
	//	return false;
	//}

//	m_pEffect.Enable();

//	m_pEffect.SetColorTextureUnit(0);
  //  m_pEffect.SetDirectionalLight(m_directionalLight);
	//m_pEffect.SetMatSpecularIntensity(0.0f);
   // m_pEffect.SetMatSpecularPower(0);

	//if (!m_mesh.LoadMesh("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\BVH\\ssa.fbx")) {
	//	printf("Mesh load failed\n");
	//	return false;
	//}

	//Shader test("i:\\LearnOpenGL\\LearnOpenGL\\Shader\\vPBR.vs", "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\Test.fs");

	//unsigned int a = loadTexture("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\virtual_r_xiaoyun01_Face01.TGA");

	//test.use();
	//glUniform1i(glGetUniformLocation(test.ID, "albedoMap"), 0);


	string modepath = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\shouyu_r_qianyi01_Bind.fbx";
	               // = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\Content\\HuoJianHua.fbx";

	if (!m_SkinMesh.LoadMesh(modepath))
	{
		printf("Mesh load failed\n");
		return false;
	}

	string skeletepath = "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\BVH\\ssa.fbx";
	if (!m_SkeleteMesh.LoadMesh(skeletepath))
	{
		printf("Skelete load failed\n");
		return false;
	}

	Shader SkeleteShader = Shader("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\default.vs", "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\default.fs");




	PBRMaterial pbrMat;

	pbrMat = PBRMaterial();

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_Face01(b01).tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_Face01_N03.png",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_Face01_MADS.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_Face01_SSS.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_Face01_MADS.tga"
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiaoyun01_Fur01_N.tga",
		"",
		"",
		"",
		""
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_N.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_MetallicOcculusionSmoothness.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_MetallicOcculusionSmoothness.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_MetallicOcculusionSmoothness.tga"
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01.tga",
		"",
		"",
		"",
		""
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiaoyun01_Hair01.tga",
		"",
		"",
		"",
		""
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_suit01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_suit01_N(b02).tga",
		"",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYi\\Texture\\shouyu_r_qianyi01_suit01_MADS.tga",
		""
	);

	/*pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_hand01.tga",
		"",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_hand01_MetallicOcculusionSmoothness01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_hand01_MetallicOcculusionSmoothness01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_hand01_MetallicOcculusionSmoothness01.tga"
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_N.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_MetallicOcculusionSmoothness.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_MetallicOcculusionSmoothness.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_eye01_MetallicOcculusionSmoothness.tga"
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_foot01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_foot01_N.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_foot01_MetallicOcculusionSmoothness01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_foot01_MetallicOcculusionSmoothness01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_foot01_MetallicOcculusionSmoothness01.tga"
	);

	pbrMat.AddMap("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_cloak01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_cloak01_N.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_cloak01_MetallicOcculusionSmoothness01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_cloak01_MetallicOcculusionSmoothness01.tga",
		"I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Model\\QianYu\\Textures\\virtual_r_xiayun01_cloak01_MetallicOcculusionSmoothness01.tga"
	);
	*/




	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	Tcamera = TCamera(cameraPos);

	glm::mat4 projection = glm::perspective(glm::radians(Tcamera.Zoom), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
	pbrMat.SetProjection(projection);

	SkeleteShader.use();
	SkeleteShader.setMat4("projection", projection);


	vector<glm::vec3> lightPositions = vector<glm::vec3>();
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, -10.0f));

    vector<glm::vec3> lightColors = vector<glm::vec3>();
	lightColors.push_back(glm::vec3(389.0f, 347.0f, 335.0f));
	lightColors.push_back(glm::vec3(389.0f, 347.0f, 335.0f));
	


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	while (!glfwWindowShouldClose(window))
	{
		//输入
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
	//	m_pGameCamera->OnRender();
	
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//m_pEffect->Enable();
	//	vector<Matrix4f> SkeleteTransform;
	//	vector<Matrix4f> BoneTransform;

	//	Matrix4f t=Matrix4f(1,0,0,1,
	//	                    0,1,0,1,
	//	                    0,0,1,1,
	//	                    0,0,0,1);
	//	Matrix4f t1 = Matrix4f(1, 0, 0, 2,
	//		0, 1, 0, 1,
	//		0, 0, 1, 1,
	//		0, 0, 0, 1);

	//	float RunningTime = GetRunningTime();

	//	Transforms.push_back(t1);
	//	Transforms.push_back(t);
	//	m_SkinMesh.BoneTransform(currentFrame, Transforms);
		//m_SkinMesh.SkeleteTransform(currentFrame,SkeleteTransform,BoneTransform);
		//Transforms.push_back(t);
		//Transforms.push_back(t1);

	
		
		//m_BoneShader->SetBoneTransform(0, Transforms1);
	//	m_mesh.Render();
	//	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	//	Pipeline p;
	//	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	//	p.SetPerspectiveProj(m_persProjInfo);
//		p.Scale(0.01f, 0.01f, 0.01f);

	//	Vector3f Pos(m_position);
	//	p.WorldPos(Vector3f(-1.0f, 0.0f, 6.0f));
	//	p.Rotate(90.0f, 180.0f, 180.0f);
		

		//m_pEffect.Enable();
	//	for (unsigned int i = 0; i < BoneTransform.size(); i++) {

	//		m_pEffect.SetBoneTransform(i, t);
	//	}
	//	m_pEffect.SetEyeWorldPos(m_pGameCamera->GetPos());
	//	m_pEffect.SetWVP(p.GetWVPTrans());
	//	m_pEffect.SetWorldMatrix(p.GetWorldTrans());	
	

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model,glm::vec3(0.01, 0.01, 0.01));
		model = glm::rotate(model,(GLfloat)glm::radians(0.0f), glm::normalize(glm::vec3(1,0,0)));
		model = glm::translate(model, glm::vec3(0, -100, 00));
		glm::mat4 view = Tcamera.GetViewMatrix();
	//	test.use();
	//	glm::mat4 projection = glm::perspective(Tcamera.Zoom, (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);

	//	glUniformMatrix4fv(glGetUniformLocation(test.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	//	glUniformMatrix4fv(glGetUniformLocation(test.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//	glUniformMatrix4fv(glGetUniformLocation(test.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//	glActiveTexture(GL_TEXTURE0);
	////	glBindTexture(GL_TEXTURE_2D, a);

		
		pbrMat.Enable(view, cameraPos, model,0);
		pbrMat.SetLights(lightPositions, lightColors);

		m_SkinMesh.RenderMesh(0);

		pbrMat.Enable(view, cameraPos, model, 1);
		m_SkinMesh.RenderMesh(1);

		pbrMat.Enable(view, cameraPos, model, 2);
		m_SkinMesh.RenderMesh(2);

	

		pbrMat.Enable(view, cameraPos, model, 3);
		m_SkinMesh.RenderMesh(3);

		pbrMat.Enable(view, cameraPos, model, 4);
		m_SkinMesh.RenderMesh(4);

		pbrMat.Enable(view, cameraPos, model, 5);
		m_SkinMesh.RenderMesh(5);

	//	pbrMat.Enable(view, cameraPos, model, 6);
	//	m_SkinMesh.RenderMesh(6);

	//	pbrMat.Enable(view, cameraPos, model, 7);
	//	m_SkinMesh.RenderMesh(7);

		//pbrMat.Enable(view, cameraPos, model, 8);
		//m_SkinMesh.RenderMesh(8);

		//pbrMat.Enable(view, cameraPos, model, 9);
		//m_SkinMesh.RenderMesh(9);


		map<string,glm::mat4> SkeleteTransform;
		map<string,glm::mat4> BoneTransform;

		vector<glm::mat4> tt;
		vector<glm::mat4> kk;

		m_SkeleteMesh.SkeleteMapTransform(currentFrame, SkeleteTransform, BoneTransform);
	//	m_SkeleteMesh.SkeleteTransform(currentFrame, tt, kk);
		m_SkinMesh.MapTransform(currentFrame, SkeleteTransform, BoneTransform, tt, kk);


		glm::mat4 gg(1.0f);

		SkeleteShader.use();
		model = glm::translate(model, glm::vec3(100, 0, 0));
		SkeleteShader.setMat4("model", model);
		SkeleteShader.setMat4("view", view);
		for (unsigned int k = 0; k < tt.size(); k++) {

			//std::cout << Transforms[i] << std::endl;
			char Name[128];
			memset(Name, 0, sizeof(Name));
			sprintf(Name, "SkeleteTransform[%d]", k);
			SkeleteShader.setMat4(Name, tt[k]);
		}

		m_SkinMesh.RenderSkelete();

		SkeleteShader.use();
		model = glm::translate(model, glm::vec3(-200, 0, 0));
		SkeleteShader.setMat4("model", model);
		SkeleteShader.setMat4("view", view);

		map<string, glm::mat4>::iterator itr;
		int mg = 0;
		for (itr = SkeleteTransform.begin(); itr != SkeleteTransform.end();itr++) {

			//std::cout << Transforms[i] << std::endl;
			char Name[128];
			memset(Name, 0, sizeof(Name));
			sprintf(Name, "SkeleteTransform[%d]", mg);
			SkeleteShader.setMat4(Name, tt[mg]);
			mg++;
		}

	   m_SkeleteMesh.RenderSkelete();





		
	//	pbrMat.SkyBox(view);
		
	//	m_BoneShader.Enable();
	//	for (unsigned int k = 0; k < SkeleteTransform.size(); k++) {

			//std::cout << Transforms[i] << std::endl;
		//	m_BoneShader.SetBoneTransform(k, t);

	//	}
	//	p.WorldPos(Vector3f(1.0f,0.0f,6.0f));
	//	m_BoneShader.SetWVP(p.GetWVPTrans());
	//	m_BoneShader.SetWorldMatrix(p.GetWorldTrans());
	//	m_SkinMesh.RenderSkelete();

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Tcamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Tcamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Tcamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Tcamera.ProcessKeyboard(RIGHT, deltaTime);

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

	Tcamera.ProcessMouseMovement(xoffset,yoffset);
	
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

unsigned int loadTexture(const char* path)
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

/*unsigned int loadCubemap(std::vector<std::string>faces)
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