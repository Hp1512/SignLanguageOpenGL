/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "ogldev_skinned_mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void SkinnedMesh::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i]     = BoneID;
            Weights[i] = Weight;
            //std::cout << IDs[i] << std::endl;
            return;
        }        
    }
    
    // should never get here - more bones than we have space for
   // assert(0);
}

SkinnedMesh::SkinnedMesh()
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = NULL;
}


SkinnedMesh::~SkinnedMesh()
{
    Clear();
}


void SkinnedMesh::Clear()
{
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }
       
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


bool SkinnedMesh::LoadMesh(const string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();
 
    // Create the VAO
    glGenVertexArrays(1, &m_VAO);   
    glBindVertexArray(m_VAO);
    
    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;    
  
    m_pScene = m_Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);
    
    if (m_pScene) {  
        m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();
        Ret = InitFromScene(m_pScene, Filename);
        SetSkeleteBufferData();
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
    }

   

    // Make sure the VAO is not changed from the outside
   // glBindVertexArray(0);	

    return Ret;
}


bool SkinnedMesh::InitFromScene(const aiScene* pScene, const string& Filename)
{  
   
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    if (!InitSkelete())
        cout << "no exit skelete" << endl;

    if (m_Entries.size() == 0)
    {
        if (!LoadBones())
            return false;
        return true;
    }
    vector<VertexBoneData> Bones;
    vector<Vector3f> Positions;
    vector<Vector3f> Normals;
    vector<Vector2f> TexCoords;
   
    vector<unsigned int> Indices;
       
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    
    // Count the number of vertices and indices
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
        m_Entries[i].NumIndices    = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex    = NumVertices;
        m_Entries[i].BaseIndex     = NumIndices;
        
        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);
        
    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
    }

   

 //   if (!InitMaterials(pScene, Filename)) {
  //      return false;
  //  }

   
    
   
    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    return GLCheckError();
}

void SkinnedMesh::SetSkeleteBufferData()
{
  

    Vector4f InitPos = Vector4f(0, 0, 0, 1);
    vector<float> points=vector<float>();

    for (int i = 0; i < m_SkeleteInfo.size(); i++)
    {
       
       cout << string(m_SkeleteInfo[i].node->mName.data) << m_SkeleteInfo[i].SkeleteID<< endl;

        if (m_SkeleteInfo[i].parent != NULL)
        {
            SkeleteInfo* ktemp = FindBoneInfos(m_SkeleteInfo[i]);

            if (ktemp != NULL)
            {
                Vector4f temp = test(m_SkeleteInfo[i].FinalTransformation, InitPos);
              
                points.push_back(temp.x);
                points.push_back(temp.y);
                points.push_back(temp.z);
                points.push_back(m_SkeleteInfo[i].SkeleteID);
                points.push_back(m_SkeleteInfo[i].SkeleteID);
               
              
                Vector4f temp1 = test(ktemp->FinalTransformation, InitPos);
                points.push_back(temp1.x);
                points.push_back(temp1.y);
                points.push_back(temp1.z);
                points.push_back(ktemp->SkeleteID);
                points.push_back(ktemp->SkeleteID);
              
       

               
            }
        }
    }

    for (int i = 0; i < points.size(); i=i+5)
    {
        cout <<"当前数据："<<"x:" <<points[i] << "y:" << points[i+1] << "z:" << points[i + 2] << "id:" << points[i + 3] << "id:" << points[i + 4] << endl;
    }


    if (points.size() >= 2)
    {
        glGenVertexArrays(1, &m_SkeleteVAO);
        glBindVertexArray(m_SkeleteVAO);
        // Create the buffers for the vertices attributes
        glGenBuffers(1, &m_SkeleteBuffers);
        glBindBuffer(GL_ARRAY_BUFFER, m_SkeleteBuffers);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size(), &points[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(points[0]), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(points[0]), (void*)(3 * sizeof(points[0])));
      
        //glBindVertexArray(0);
    }
    
}


SkinnedMesh::SkeleteInfo* SkinnedMesh::FindBoneInfos(SkeleteInfo t)
{
    for (int i = 0; i < m_SkeleteInfo.size(); i++)
    {
        if (t.parent->mName.data == m_SkeleteInfo[i].node->mName.data)
        {
            return &m_SkeleteInfo[i];
        }
    }
    return NULL;
}

bool SkinnedMesh::FindNoneSkeleteInfos(SkeleteInfo t)
{
    for (int i = 0; i < m_NoneSkeleteInfo.size(); i++)
    {
        if (t.node->mName.data == m_NoneSkeleteInfo[i].node->mName.data)
        {
          //  std::cout << "错误的骨骼节点："<<t.SkeleteID <<t.node->mName.data<< endl;
            return false;
        }
    }
    return true;
}

Vector4f SkinnedMesh::test(const Matrix4f& Right, const Vector4f m)
{
    Vector4f Ret;

    Ret.x = m.x * Right.m[0][0] + m.y * Right.m[0][1] + m.z * Right.m[0][2] + m.w * Right.m[0][3];
    Ret.y = m.x * Right.m[1][0] + m.y * Right.m[1][1] + m.z * Right.m[1][2] + m.w * Right.m[1][3];
    Ret.z = m.x * Right.m[2][0] + m.y * Right.m[2][1] + m.z * Right.m[2][2] + m.w * Right.m[2][3];
    Ret.w = m.x * Right.m[3][0] + m.y * Right.m[3][1] + m.z * Right.m[3][2] + m.w * Right.m[3][3];

    return Ret;
}



void SkinnedMesh::InitMesh(unsigned int MeshIndex,
                    const aiMesh* paiMesh,
                    vector<Vector3f>& Positions,
                    vector<Vector3f>& Normals,
                    vector<Vector2f>& TexCoords,
                    vector<VertexBoneData>& Bones,
                    vector<unsigned int>& Indices)
{    
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(Vector2f(pTexCoord->x, -pTexCoord->y));        
    }
    
    LoadBones(MeshIndex, paiMesh, Bones);
    
    // Populate the index buffer
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}
bool SkinnedMesh::LoadBones()
{
    if (m_SkeleteInfo.size() <= 0)
        return false;
    m_NumBones = m_SkeleteInfo.size();
    for (int i = 0; i < m_SkeleteInfo.size(); i++)
    {
        string BoneName(m_SkeleteInfo[i].node->mName.data);
        BoneInfo bi;
        bi.BoneName = BoneName;
        bi.BoneOffset = m_SkeleteInfo[i].BoneOffset;
        m_BoneInfo.push_back(bi);
        m_BoneMapping[BoneName] = i;
    }
    return true;
}

void SkinnedMesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones)
{
    for (unsigned int i = 0 ; i < pMesh->mNumBones ; i++) {                
        unsigned int BoneIndex = 0;        
        string BoneName(pMesh->mBones[i]->mName.data);
        
        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;            
	        BoneInfo bi;
            bi.BoneName = BoneName;
			m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;            
            m_BoneMapping[BoneName] = BoneIndex;
        }
        else {
            BoneIndex = m_BoneMapping[BoneName];
        }                      
        
        for (unsigned int j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++) {
            unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight  = pMesh->mBones[i]->mWeights[j].mWeight;
         //   std::cout << VertexID << std::endl;
        //    if(VertexID==4716)
         //       std::cout << VertexID << std::endl;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);

        }
    }    
}


bool SkinnedMesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
    // Extract the directory part from the file name
    string::size_type SlashIndex = Filename.find_last_of("\\");
    string Dir;
   // Dir = Filename.substr(0, SlashIndex);
    if (SlashIndex == string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                string p(Path.data);
                
                if (p.substr(0, 2) == ".\\") {                    
                    p = p.substr(2, p.size() - 2);
                }
                               
                string FullPath = Dir + "/" + p;
                    
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
                }
            }
        }
    }

    return Ret;
}


void SkinnedMesh::RenderSkelete()
{
   // glBindVertexArray(m_VAO);
    glBindVertexArray(m_SkeleteVAO);
    glDrawArrays(GL_LINES, 0, 1000);
  
   /* glBindVertexArray(m_VAO);
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());
        
        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

		glDrawElementsBaseVertex(GL_TRIANGLES, 
                                 m_Entries[i].NumIndices, 
                                 GL_UNSIGNED_INT, 
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex), 
                                 m_Entries[i].BaseVertex);
    }*/

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}

void SkinnedMesh::RenderMesh(int EntriesIndex)
{
    glBindVertexArray(m_VAO);
  //  for (unsigned int i = 0 ; i < 1; i++) {
     //   const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

    //    assert(MaterialIndex < m_Textures.size());

    //    if (m_Textures[MaterialIndex]) {
    //        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
    //    }

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[EntriesIndex].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_Entries[EntriesIndex].BaseIndex),
                                 m_Entries[EntriesIndex].BaseVertex);
 //   }
    glBindVertexArray(0);
}


unsigned int SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


unsigned int SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


unsigned int SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }
            
    unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    unsigned int NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }
    
    unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    unsigned int NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    unsigned int NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}




void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
{    
    string NodeName(pNode->mName.data);
    Matrix4f NodeTransformation(pNode->mTransformation);
    
    if (m_pScene->HasAnimations())
    {
        const aiAnimation* pAnimation = m_pScene->mAnimations[0];

      

        const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);


        if (pNodeAnim) {
            // Interpolate scaling and generate scaling transformation matrix
            aiVector3D Scaling;
            CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
            Matrix4f ScalingM;
            ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

            // Interpolate rotation and generate rotation transformation matrix
            aiQuaternion RotationQ;
            CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
            Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

            // Interpolate translation and generate translation transformation matrix
            aiVector3D Translation;
            CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
            Matrix4f TranslationM;
            TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

            // Combine the above transformations
            NodeTransformation = TranslationM * RotationM * ScalingM;
        }
    }
       
    Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;
    
    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {

       
        unsigned int BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
        m_BoneInfo[BoneIndex].BoneName = NodeName;
   //     m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation;
    //    cout << "当前骨骼索引:" << NodeName << "..." << m_BoneMapping[NodeName]<<endl;
    }
    
    for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}
void SkinnedMesh::ReadSkeleteHeirarchy(const aiNode* pNode, const Matrix4f& ParentTransform, const aiNode* parent)
{
   // m_BoneInfo.clear();
    string NodeName(pNode->mName.data);
    Matrix4f NodeTransformation(pNode->mTransformation);
    Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;
    SkeleteInfo bi= SkeleteInfo();
    bi.BoneOffset = pNode->mTransformation;
    bi.node = pNode;
    bi.FinalTransformation = m_GlobalInverseTransform *GlobalTransformation;
    bi.parent = parent;
    bi.SkeleteID = m_NumSkelete;
    m_SkeleteInfo.push_back(bi);
    m_NumSkelete = m_NumSkelete + 1;
   // GlobalTransformation = bi.FinalTransformation;

    for (int i = 0; i < pNode->mNumChildren; i++)
        ReadSkeleteHeirarchy(pNode->mChildren[i], GlobalTransformation, pNode);
}



void SkinnedMesh::BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms)
{
    Matrix4f Identity;
    Identity.InitIdentity();
    
    float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    Transforms.resize(m_NumBones);

    for (unsigned int i = 0 ; i < m_NumBones ; i++) {
        
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}

glm::mat4 SkinnedMesh::TransformMatrix4f(Matrix4f k)
{
   
    glm::mat4 t = glm::mat4(1.0f);

    float a = t[0][0];
 //   t[3][0] = -200;
    t[0][0] = k.m[0][0]; t[0][1] = k.m[1][0]; t[0][2] = k.m[2][0]; t[0][3] = k.m[3][0];
    t[1][0] = k.m[0][1]; t[1][1] = k.m[1][1]; t[1][2] = k.m[2][1]; t[1][3] = k.m[3][1];
    t[2][0] = k.m[0][2]; t[2][1] = k.m[1][2]; t[2][2] = k.m[2][2]; t[2][3] = k.m[3][2];
    t[3][0] = k.m[0][3]; t[3][1] = k.m[1][3]; t[3][2] = k.m[2][3]; t[3][3] = k.m[3][3];

    return t;

}


void SkinnedMesh::SkeleteMapTransform(float TimeInSeconds, map<string,glm::mat4>& SkeleteTransform, map<string,glm::mat4>& BoneTransform)
{
    Matrix4f Identity;
    Identity.InitIdentity();
    float AnimationTime = 0;

    if (m_pScene->HasAnimations())
    {



        float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
        float TimeInTicks = TimeInSeconds * TicksPerSecond;
        AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);


    }
    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);


    for (unsigned int i = 0; i < m_NumBones; i++)
    {
        BoneTransform[m_BoneInfo[i].BoneName] = TransformMatrix4f(m_BoneInfo[i].FinalTransformation);
    }

    for (unsigned int i = 0; i < m_NumSkelete; i++) {

        string NodeName(m_SkeleteInfo[i].node->mName.data);

        if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
        {
            SkeleteTransform[NodeName] = TransformMatrix4f(m_BoneInfo[m_BoneMapping[NodeName]].FinalTransformation);
            //      cout << "当前骨骼索引" << i << "," << "当前骨骼名：" << NodeName << "动画骨骼索引" << m_BoneMapping[NodeName] << endl;
        }
        else
        {
            if (m_SkeleteInfo[i].parent != NULL)
            {
                NodeName = m_SkeleteInfo[i].parent->mName.data;
                SkeleteTransform[NodeName] = TransformMatrix4f(m_BoneInfo[m_BoneMapping[NodeName]].FinalTransformation);
            }
            else
            {

                SkeleteTransform[NodeName] = TransformMatrix4f(m_BoneInfo[1].FinalTransformation);
            }
            //  cout << "当前骨骼索引" << i << "," << "当前骨骼名：" << NodeName << "动画骨骼索引" << m_BoneMapping[NodeName] << endl;
        }
    }
}

void SkinnedMesh::MapTransform(float TimeInSeconds, map<string, glm::mat4>& SkeleteTransforms, map<string, glm::mat4>& BoneTransforms, vector<glm::mat4>& SkeleteTransform, vector<glm::mat4>& BoneTransform)
{
    SkeleteTransform.resize(m_NumSkelete);
    BoneTransform.resize(m_NumBones);

    for (unsigned int i = 0; i < m_NumBones; i++)
    {
        if (BoneTransforms.find(m_BoneInfo[i].BoneName) != BoneTransforms.end())
        {
            BoneTransform[i] = BoneTransforms[m_BoneInfo[i].BoneName];
        }
        else
        {
            BoneTransform[i] = glm::mat4(1.0f);
        }
    }

    for (unsigned int i = 0; i < m_NumSkelete; i++) {

        string NodeName(m_SkeleteInfo[i].node->mName.data);

        if (SkeleteTransforms.find(NodeName) != SkeleteTransforms.end())
        {
            SkeleteTransform[i] = SkeleteTransforms[NodeName];
            //      cout << "当前骨骼索引" << i << "," << "当前骨骼名：" << NodeName << "动画骨骼索引" << m_BoneMapping[NodeName] << endl;
        }
        else
        {
            SkeleteTransform[i] = glm::mat4(1.0f);
            //  cout << "当前骨骼索引" << i << "," << "当前骨骼名：" << NodeName << "动画骨骼索引" << m_BoneMapping[NodeName] << endl;
        }
    }
}

void SkinnedMesh::SkeleteTransform(float TimeInSeconds, vector<glm::mat4>& SkeleteTransform, vector<glm::mat4>& BoneTransform)
{
    Matrix4f Identity;
    Identity.InitIdentity();
    float AnimationTime = 0;

    if (m_pScene->HasAnimations())
    {



        float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
        float TimeInTicks = TimeInSeconds * TicksPerSecond;
        AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

      
    }
    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    SkeleteTransform.resize(m_NumSkelete);
    BoneTransform.resize(m_NumBones);

    for (unsigned int i = 0; i < m_NumBones; i++)
    {
        BoneTransform[i] = TransformMatrix4f(m_BoneInfo[i].FinalTransformation);
    }

    for (unsigned int i = 0; i < m_NumSkelete; i++) {

        string NodeName(m_SkeleteInfo[i].node->mName.data);
   
        if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
        {
            SkeleteTransform[i] = TransformMatrix4f(m_BoneInfo[m_BoneMapping[NodeName]].FinalTransformation);
      //      cout << "当前骨骼索引" << i << "," << "当前骨骼名：" << NodeName << "动画骨骼索引" << m_BoneMapping[NodeName] << endl;
        }
        else
        {
            if (m_SkeleteInfo[i].parent != NULL)
            {
                NodeName = m_SkeleteInfo[i].parent->mName.data;
                SkeleteTransform[i] = TransformMatrix4f(m_BoneInfo[m_BoneMapping[NodeName]].FinalTransformation);
            }
            else
            {
                
                SkeleteTransform[i]= TransformMatrix4f(m_BoneInfo[1].FinalTransformation);
            }
          //  cout << "当前骨骼索引" << i << "," << "当前骨骼名：" << NodeName << "动画骨骼索引" << m_BoneMapping[NodeName] << endl;
        }
    }
}

bool SkinnedMesh::InitSkelete()
{
    m_NumSkelete = 0;
    m_SkeleteInfo = vector<SkeleteInfo>();
    Matrix4f Identity;
    Identity.InitIdentity();
    ReadSkeleteHeirarchy(m_pScene->mRootNode, Identity, NULL);
    if (m_SkeleteInfo.size() > 0)
        return true;
    return false;
}

void SkinnedMesh::CheckSkelete()
{
    m_NoneSkeleteInfo = vector<SkeleteInfo>();
    if (m_SkeleteInfo.size() == 0)
        return;
    if (m_BoneInfo.size() == 0)
        return;
    if (m_SkeleteInfo.size() <= m_BoneInfo.size())
        return;
    for (int i = 0; i < m_SkeleteInfo.size(); i++)
    {
        if (m_BoneMapping.find(m_SkeleteInfo[i].node->mName.data) == m_BoneMapping.end())
        {
            m_NoneSkeleteInfo.push_back(m_SkeleteInfo[i]);
        }
    } 

  
}



const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
    for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}