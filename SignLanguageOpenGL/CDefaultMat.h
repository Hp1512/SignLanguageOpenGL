#pragma once
#include "CMaterial.h"
class CDefaultMat :public CMaterial
{
public:
	CDefaultMat(const char* vertexPath, const char* fragmentPath);
	CDefaultMat();
	
	~CDefaultMat();
	void SetColor(glm::vec3 color);

private:
	glm::vec3 Color;
};

