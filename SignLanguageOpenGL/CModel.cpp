#include "CModel.h"

CModel::CModel(string path)
{
	Assimp::Importer m_Importer;
	const aiScene* m_pScene;
	m_pScene = m_Importer.ReadFile(path.c_str(), ASSIMP_LOAD_FLAGS);
	if (m_pScene) {

		m_Skeleton = CSkeleton(m_pScene);
		m_SkinMesh = CSkinMesh(m_pScene,m_Skeleton);
		int pos1 = path.find_last_of('\\');
		m_sName =path.substr(pos1 + 1);	
	}
	m_Transform = glm::mat4(1.0f);
	m_MeshMaterials = vector<CMaterial>();
	CDefaultMat t = CDefaultMat("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\default.vs", "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\default.fs");
	t.SetColor(glm::vec3(1, 0, 0));
	for (int i = 0; i < m_SkinMesh.GetMeshCount(); i++)
	{		
		m_MeshMaterials.push_back(t);
	}

	CMaterial t1 = CMaterial("I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\vSkeleton.vs", "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\fSkeleton.fs");
	
	m_SkeletonMaterial = t1;

//	m_SkeletonMaps = CSkeletonMaps();

	m_CAnimationController =new CAnimationController(m_Skeleton, m_SkeletonMaps);

	m_bShowSkeleton = false;
	//delete m_pScene;
//	m_pScene = NULL;
}

void CModel::TransformModel(glm::vec3 m_Translate, glm::vec3 m_Rotation, glm::vec3 m_Scale)
{
	glm::mat4 model(1.0f);
	
	model = glm::scale(model, m_Scale);
	model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	model = glm::translate(model, m_Translate);
	m_Transform = model;
}

void CModel::SetMeshMaterials(int index,CMaterial mat)
{
	m_MeshMaterials[index] = mat;
}

void CModel::Draw(int windowIndex)
{
	m_CAnimationController->UpdateAnimationState();
	for (int i = 0; i < m_SkinMesh.GetMeshCount(); i++)
	{
		m_MeshMaterials[i].Use();
		m_MeshMaterials[i].SetProjection(m_Projection);
		m_MeshMaterials[i].SetView(m_View);
		m_MeshMaterials[i].SetModel(m_Transform);
		m_MeshMaterials[i].SetBones(m_CAnimationController->GetCurrrntTransform());
		m_SkinMesh.Draw(i,windowIndex);
	}
	if (m_bShowSkeleton)
	{
		m_SkeletonMaterial.Use();
		m_SkeletonMaterial.SetProjection(m_Projection);
		m_SkeletonMaterial.SetView(m_View);
		m_SkeletonMaterial.SetModel(m_Transform);
		m_SkeletonMaterial.SetBones(m_CAnimationController->GetCurrrntTransform());
		m_Skeleton.Draw(windowIndex);
	}
}




CModel::CModel()
{
	m_Transform = glm::mat4(1.0f);
	m_Projection = glm::mat4(1.0f);
	m_View = glm::mat4(1.0f);
	m_bShowSkeleton = false;

}

CModel::~CModel()
{
	delete m_CAnimationController;
	m_CAnimationController = NULL;
}
