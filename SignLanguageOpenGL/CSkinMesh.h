#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CMaterial.h"
#include <assimp\mesh.h>
#include <assimp\scene.h>
#include "CSkeleton.h"
#include<map>
using namespace std;

#define NUM_BONES_PER_VEREX 4

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4
#define INVALID_MATERIAL 0xFFFFFFFF

/**
 * @brief �����Դ�������
 */
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs
};

/**
 * @brief ������������ӳ���
 */
struct VertexBoneData
{
    //! ������������
    unsigned int IDs[NUM_BONES_PER_VEREX]; 
    //! ��Ӧ��Ȩ�ر�
    float Weights[NUM_BONES_PER_VEREX];

    VertexBoneData()
    {
        Reset();
    };
    /**
    * @brief ��������
    *
    */
    void Reset()
    {
        for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
        {
            IDs[i] = 0;
            Weights[i] = 0;
        }
    }
    /**
     * @brief ��ӹ����붥���Ӧ��ϵ
     *
     */
    void AddBoneData(unsigned int BoneID, float Weight)
    {
        for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++) {
            if (Weights[i] == 0.0) {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            }
        }
    }
};
 
   /**
    * @brief ����ʵ��
    *
    */
struct MeshEntry {

    /**
    * @brief ����ʵ��
    *
    */
    MeshEntry()
    {
        NumIndices = 0;
        BaseVertex = 0;
        BaseIndex = 0;
        MaterialIndex = INVALID_MATERIAL;
    }

    //! ����������
    unsigned int NumIndices;
    //! ��������
    unsigned int BaseVertex;
    //! ������
    unsigned int BaseIndex;
    //! ��������
    unsigned int MaterialIndex;
};
/**
 * @brief ģ������
 */
class CSkinMesh
{
private:
    vector<glm::vec3>m_MeshPositions; //! ����������
    vector<glm::vec3>m_MeshNormals; //! ����������
    vector<glm::vec2>m_MeshTexCoords;//UV������
    vector<unsigned int>m_MeshIndexs;//������������
    vector<VertexBoneData> m_BoneMeshMaps;//��������������

    vector<MeshEntry> m_Entries;//����ʵ��������
    int MeshCount;//������

    map<GLuint, GLuint> m_MeshVAO; //������������
    GLuint m_MeshBuffers[NUM_VBs]; //���������Դ�
public:
   
    /**
     * @brief ��������
     * @param m_pScene ����
     * @param skeleton ��������
     */
    CSkinMesh(const aiScene* m_pScene, CSkeleton skeleton);
    CSkinMesh();
	~CSkinMesh();

    /**
     * @brief �󶨹�������
     * @param index ��������
     */
    void BindMeshesBufferData(int windowIndex);
    /**
     * @brief ��������
     * @param meshIndex    ��������
     * @param windowIndex    ��������
     */
    void Draw(int meshIndex,int windowIndex);
    /**
     * @brief �õ���������
     *
     * @return ��ǰ���������
     *     -<em>MeshCount</em> ֡����
     */
    const int GetMeshCount()const { return MeshCount; }
private:
   
    void __LoadMesh(unsigned int MeshIndex,
        const aiMesh* paiMesh,
        vector<glm::vec3>& Positions,
        vector<glm::vec3>& Normals,
        vector<glm::vec2>& TexCoords,
        vector<VertexBoneData>& BoneMeshMap,
        vector<unsigned int>& Indices, CSkeleton skeleton);


};


