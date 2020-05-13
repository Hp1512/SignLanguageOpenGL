#pragma once
#include<string>
#include<vector>
#include<map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CMaterial.h"
#include <assimp\scene.h>

using namespace std;
struct Bone
{
	
	string m_sBoneName;
	int m_fBoneID;

	glm::mat4 m_BoneOffset;
	glm::mat4 m_BoneGlobalOffset;

	const aiNode* m_Node;
	const aiNode* m_ParentNode;


};


class CSkeleton
{
private:
	Bone m_RootBone;
	
	vector<float>m_BoneLines;
	unsigned int m_uNumBones;

	map<unsigned int,unsigned int> m_BonesVAO;
	unsigned int m_BonesBuffers;


	//CMaterial m_Material;
public:
	vector<Bone>m_Bones;
	void Draw(int windowIndex);
	int  FindBones(string boneName);
	int GetBoneCount();
	void BindSkeleteBufferData(int windowIndex);
	CSkeleton(const aiScene* m_pScene);
	CSkeleton();
	~CSkeleton();

private:
	void CSkeleton::__LoadBones(unsigned int MeshIndex, const aiMesh* pMesh);
	void __GetBonesData(const aiNode* pNode, glm::mat4 ParentTransform, const aiNode* parent);
	
	glm::mat4 __TransformMat4(aiMatrix4x4 mat);
	void __GetBoneLines();
	

};

