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
 * @brief 数据显存索引表
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
 * @brief 骨骼关联顶点映射表
 */
struct VertexBoneData
{
    //! 关联骨骼索引
    unsigned int IDs[NUM_BONES_PER_VEREX]; 
    //! 对应的权重表
    float Weights[NUM_BONES_PER_VEREX];

    VertexBoneData()
    {
        Reset();
    };
    /**
    * @brief 重置数据
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
     * @brief 添加骨骼与顶点对应关系
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
    * @brief 网格实体
    *
    */
struct MeshEntry {

    /**
    * @brief 网格实体
    *
    */
    MeshEntry()
    {
        NumIndices = 0;
        BaseVertex = 0;
        BaseIndex = 0;
        MaterialIndex = INVALID_MATERIAL;
    }

    //! 面索引总数
    unsigned int NumIndices;
    //! 顶点数据
    unsigned int BaseVertex;
    //! 面索引
    unsigned int BaseIndex;
    //! 材质索引
    unsigned int MaterialIndex;
};
/**
 * @brief 模型网格
 */
class CSkinMesh
{
private:
    vector<glm::vec3>m_MeshPositions; //! 顶点总数据
    vector<glm::vec3>m_MeshNormals; //! 法线总数据
    vector<glm::vec2>m_MeshTexCoords;//UV总数据
    vector<unsigned int>m_MeshIndexs;//面索引总数据
    vector<VertexBoneData> m_BoneMeshMaps;//骨骼网格总数据

    vector<MeshEntry> m_Entries;//网格实体总数据
    int MeshCount;//网格数

    map<GLuint, GLuint> m_MeshVAO; //网格数据数组
    GLuint m_MeshBuffers[NUM_VBs]; //网格数据显存
public:
   
    /**
     * @brief 网格数据
     * @param m_pScene 场景
     * @param skeleton 骨骼数据
     */
    CSkinMesh(const aiScene* m_pScene, CSkeleton skeleton);
    CSkinMesh();
	~CSkinMesh();

    /**
     * @brief 绑定骨骼数据
     * @param index 窗口索引
     */
    void BindMeshesBufferData(int windowIndex);
    /**
     * @brief 绘制网格
     * @param meshIndex    网格索引
     * @param windowIndex    窗口索引
     */
    void Draw(int meshIndex,int windowIndex);
    /**
     * @brief 得到网格总数
     *
     * @return 当前网格的总数
     *     -<em>MeshCount</em> 帧数据
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


