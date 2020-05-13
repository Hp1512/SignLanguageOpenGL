#include "CSkinMesh.h"


CSkinMesh::CSkinMesh(const aiScene* m_pScene,CSkeleton skeleton)
{
	MeshCount = m_pScene->mNumMeshes;
	m_Entries.resize(MeshCount);
	
	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	// Count the number of vertices and indices
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = m_pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += m_pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	m_MeshPositions.reserve(NumVertices);
	m_MeshNormals.reserve(NumVertices);
	m_MeshTexCoords.reserve(NumVertices);
	m_BoneMeshMaps.resize(NumVertices);
	m_MeshIndexs.reserve(NumIndices);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = m_pScene->mMeshes[i];
		__LoadMesh(i, paiMesh, m_MeshPositions, m_MeshNormals, m_MeshTexCoords, m_BoneMeshMaps, m_MeshIndexs, skeleton);
	}
	if (m_Entries.size() == 0)
		return;

}

CSkinMesh::CSkinMesh()
{

}

CSkinMesh::~CSkinMesh()
{

}

void CSkinMesh::__LoadMesh(unsigned int MeshIndex,
	const aiMesh* paiMesh,
	vector<glm::vec3>& Positions,
	vector<glm::vec3>& Normals,
	vector<glm::vec2>& TexCoords,
	vector<VertexBoneData>& BoneMeshMap,
	vector<unsigned int>& Indices,CSkeleton skeleton)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, -pTexCoord->y));
	}

	for (unsigned int i = 0; i < paiMesh->mNumBones; i++) {
		
		string BoneName(paiMesh->mBones[i]->mName.data);
		for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++) {
			unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + paiMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = paiMesh->mBones[i]->mWeights[j].mWeight;
			
			BoneMeshMap[VertexID].AddBoneData(skeleton.FindBones(BoneName), Weight);
		}
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}



void CSkinMesh::BindMeshesBufferData(int index)
{
	// Create the VAO
	m_MeshVAO[index] = 0;
	glGenVertexArrays(1, &m_MeshVAO[index]);
	glBindVertexArray(m_MeshVAO[index]);
	glGenBuffers(NUM_VBs, m_MeshBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_MeshBuffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_MeshPositions[0]) * m_MeshPositions.size(), &m_MeshPositions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_MeshBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_MeshTexCoords[0]) * m_MeshTexCoords.size(), &m_MeshTexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_MeshBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_MeshNormals[0]) * m_MeshNormals.size(), &m_MeshNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_MeshBuffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_BoneMeshMaps[0]) * m_BoneMeshMaps.size(), &m_BoneMeshMaps[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);

	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshBuffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_MeshIndexs[0]) * m_MeshIndexs.size(), &m_MeshIndexs[0], GL_STATIC_DRAW);
}

void CSkinMesh::Draw(int meshIndex,int windowIndex)
{
	glBindVertexArray(m_MeshVAO[windowIndex]);
	glDrawElementsBaseVertex(GL_TRIANGLES,
		m_Entries[meshIndex].NumIndices,
		GL_UNSIGNED_INT,
		(void*)(sizeof(unsigned int) * m_Entries[meshIndex].BaseIndex),
		m_Entries[meshIndex].BaseVertex);
	//glBindVertexArray(0);
}