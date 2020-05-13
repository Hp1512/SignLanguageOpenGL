#include "CSkeletonMaps.h"


CSkeletonMaps::CSkeletonMaps(string path)
{
	
	std::ifstream vMapFile;
	
	vMapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		vMapFile.open(path);
		
		std::stringstream vStream;
		vStream << vMapFile.rdbuf();
		vMapFile.close();
		m_MapJson = vStream.str();

		__ParseJson();
	
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

}

CSkeletonMaps::CSkeletonMaps()
{
}
CSkeletonMaps::~CSkeletonMaps()
{

}

void CSkeletonMaps::__ParseJson()
{
	cJSON* root;
	root = cJSON_Parse(m_MapJson.c_str());
	cJSON* character = root->child;
	while (character)
	{		
		m_SkeleteonMaps.insert(pair<string,string>(character->string, character->valuestring));
		character=character->next;
	}
	character = NULL;
}
