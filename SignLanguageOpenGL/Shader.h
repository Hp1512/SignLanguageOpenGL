#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>;
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public :
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath,const char* geometryPath=nullptr);
	Shader();
	~Shader();
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, float vec0, float vec1, float vec2, float vec3)const;
	void setMat4(const std::string& name, glm::mat4 mat4)const;
	void setVec3(const std::string& name, glm::vec3 vec3)const;
};
#endif

