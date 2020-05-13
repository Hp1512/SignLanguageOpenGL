#include "CMaterial.h"


CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
}

CMaterial::CMaterial(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR:SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	}


	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(m_ID, geometry);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void CMaterial::Use()
{
	glUseProgram(m_ID);
}

void CMaterial::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void CMaterial::SetInt(const std::string& name, int value)const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CMaterial::SetFloat(const std::string& name, float value)const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CMaterial::SetVec4(const std::string& name,glm::vec4 vec4)const
{
	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()),1, glm::value_ptr(vec4));
}
void CMaterial::SetMat4(const std::string& name, glm::mat4 mat4)const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
}

void CMaterial::SetVec3(const std::string& name, glm::vec3 vec3)const
{
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vec3));
}

void CMaterial::SetProjection(glm::mat4 mat4)
{
	SetMat4("projection", mat4);
}
void CMaterial::SetView(glm::mat4 mat4)
{
	SetMat4("view", mat4);
}
void CMaterial::SetModel(glm::mat4 mat4)
{
	SetMat4("model", mat4);
}

void CMaterial::SetBones(std::vector<glm::mat4> mat4s)
{
	for (int i = 0; i < mat4s.size(); i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		sprintf(Name, "bones[%d]", i);
		SetMat4(Name, mat4s[i]);
	}
}


