#include "CDefaultMat.h"

CDefaultMat::CDefaultMat(const char* vertexPath, const char* fragmentPath) :CMaterial(vertexPath, fragmentPath)
{
	Color = glm::vec3(1, 0, 0);
	Use();
	SetVec3("color", Color);
}
CDefaultMat::CDefaultMat()
{

}

CDefaultMat::~CDefaultMat()
{

}

void CDefaultMat::SetColor(glm::vec3 color)
{
	Color = color;
	Use();
	SetVec3("color", Color);
}