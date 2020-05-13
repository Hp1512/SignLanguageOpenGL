#pragma once
#include "CMaterial.h"
#include <vector>


struct MaterialMap
{
	unsigned int AlbedoMap;
	unsigned int NormalMap;
	unsigned int MetallicMap;
	unsigned int RoughnessMap;
	unsigned int AOMap;
};

class FaceMaterial:public CMaterial
{

public:
	FaceMaterial();

	virtual ~FaceMaterial();

	void AddMap(const char* AlbedoPath, const char* NormalPath, const char* MetallicPath, const char* RoughnessPath, const char* AOPath);
	void SetCameraPos();
	void SetIrradianceMap();
	void SetPrefilterMap();
	void SetBrdfLUT();

	void SetLights(std::vector<glm::vec3>lightPosition, std::vector<glm::vec3>lightColor);

private:

	void renderCube();
	void renderQuad();
	unsigned int loadTexture(const char* path);

	unsigned int envCubemap;

	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;

	std::vector<MaterialMap> Maps;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	std::vector<glm::vec3>lightPositions;
	std::vector<glm::vec3>lightColors;


};

