#include "CSkeleton.h"
#include "CSkinMesh.h"
#include <glad\glad.h>

CSkeleton::CSkeleton(const aiScene* m_pScene)
{
    m_uNumBones = 0;
    m_Bones = vector<Bone>();
    for (int i = 0; i < m_pScene->mNumMeshes; i++)
    {
        __LoadBones(i, m_pScene->mMeshes[i]);
    }
    __GetBonesData(m_pScene->mRootNode, glm::mat4(1.0f), NULL);
    __GetBoneLines();
    //__BindSkeleteBufferData();
}

void CSkeleton::__GetBoneLines()
{
    m_BoneLines = vector<float>();
    glm::vec4 InitPos = glm::vec4(0, 0, 0, 1);
    for (int i = 0; i < m_Bones.size(); i++)
    {
        cout << m_Bones[i].m_sBoneName <<i<< endl;
        if (m_Bones[i].m_ParentNode != NULL)
        {
            int index = FindBones(m_Bones[i].m_ParentNode->mName.data);

            if (index != -1)
            {
                glm::vec4 temp = m_Bones[i].m_BoneGlobalOffset * InitPos;

                m_BoneLines.push_back(temp.x);
                m_BoneLines.push_back(temp.y);
                m_BoneLines.push_back(temp.z);
                m_BoneLines.push_back((float)m_Bones[i].m_fBoneID);
                m_BoneLines.push_back((float)m_Bones[i].m_fBoneID);
                glm::vec4 temp1 = m_Bones[index].m_BoneGlobalOffset * InitPos;
                m_BoneLines.push_back(temp1.x);
                m_BoneLines.push_back(temp1.y);
                m_BoneLines.push_back(temp1.z);
                m_BoneLines.push_back((float)m_Bones[index].m_fBoneID);
                m_BoneLines.push_back((float)m_Bones[index].m_fBoneID);

            }
        }
    }
}

void CSkeleton::BindSkeleteBufferData(int windowIndex)
{
    if (m_BoneLines.size() >= 2)
    {
        m_BonesVAO[windowIndex] = 0;
        glGenVertexArrays(1, &m_BonesVAO[windowIndex]);
        glBindVertexArray(m_BonesVAO[windowIndex]);
        // Create the buffers for the vertices attributes
        glGenBuffers(1, &m_BonesBuffers);
        glBindBuffer(GL_ARRAY_BUFFER, m_BonesBuffers);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_BoneLines[0]) * m_BoneLines.size(), &m_BoneLines[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(m_BoneLines[0]), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(m_BoneLines[0]), (void*)(3 * sizeof(m_BoneLines[0])));
    }

}
void CSkeleton::__GetBonesData(const aiNode* pNode, glm::mat4 ParentTransform, const aiNode* parent)
{

    string NodeName(pNode->mName.data);
    glm::mat4 NodeTransformation = __TransformMat4(pNode->mTransformation);
    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    int index = FindBones(NodeName);
    if (index != -1)
    {
        m_Bones[index].m_Node = pNode;
        m_Bones[index].m_ParentNode = parent;
        m_Bones[index].m_BoneGlobalOffset = glm::mat4(1.0f) * GlobalTransformation;
    }
 
    for (int i = 0; i < pNode->mNumChildren; i++)
        __GetBonesData(pNode->mChildren[i], GlobalTransformation, pNode);
}

void CSkeleton::__LoadBones(unsigned int MeshIndex, const aiMesh* pMesh)
{
    for (unsigned int i = 0; i < pMesh->mNumBones; i++) {

        string NodeName(pMesh->mBones[i]->mName.data);
        int index = FindBones(NodeName);
        if (index == -1)
        {
            Bone bi = Bone();
            bi.m_BoneOffset = __TransformMat4(pMesh->mBones[i]->mOffsetMatrix);
            bi.m_fBoneID = m_uNumBones;
            bi.m_sBoneName = NodeName;
            m_Bones.push_back(bi);
            m_uNumBones = m_uNumBones + 1;
        }
    }
}



int CSkeleton::FindBones(string boneName)
{
    for (int i = 0; i < m_Bones.size(); i++)
    {
        if (m_Bones[i].m_sBoneName == boneName)
            return i;
    }
    return -1;
}

int CSkeleton::GetBoneCount()
{
    return (int)m_uNumBones;
}

glm::mat4 CSkeleton::__TransformMat4(aiMatrix4x4 AssimpMatrix)
{
    glm::mat4 m(1.0f);
    m[0][0] = AssimpMatrix.a1; m[1][0] = AssimpMatrix.a2; m[2][0] = AssimpMatrix.a3; m[3][0] = AssimpMatrix.a4;
    m[0][1] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[2][1] = AssimpMatrix.b3; m[3][1] = AssimpMatrix.b4;
    m[0][2] = AssimpMatrix.c1; m[1][2] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[3][2] = AssimpMatrix.c4;
    m[0][3] = AssimpMatrix.d1; m[1][3] = AssimpMatrix.d2; m[2][3] = AssimpMatrix.d3; m[3][3] = AssimpMatrix.d4;

    return m;
}

void CSkeleton::Draw(int windowIndex)
{
    glBindVertexArray(m_BonesVAO[windowIndex]);
    glDrawArrays(GL_LINES, 0, m_BoneLines.size());
  //  glBindVertexArray(0);
}

CSkeleton::CSkeleton()
{

}
CSkeleton::~CSkeleton()
{

}