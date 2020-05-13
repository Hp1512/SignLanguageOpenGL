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

#ifndef OGLDEV_SKINNED_MESH_H
#define	OGLDEV_SKINNED_MESH_H

#include <map>
#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "ogldev_util.h"
#include "ogldev_math_3d.h"
#include "ogldev_texture.h"

using namespace std;

class SkinnedMesh
{
public:
    SkinnedMesh();

    ~SkinnedMesh();

    bool LoadMesh(const string& Filename);

    void RenderSkelete();
    void RenderMesh(int EntriesIndex);
	
    unsigned int NumBones() const
    {
        return m_NumBones;
    }
    
    void BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms);
   
    void SkeleteTransform(float TimeInSeconds, vector<glm::mat4>& SkeleteTransform, vector<glm::mat4>& BoneTransform);
    void SkeleteMapTransform(float TimeInSeconds, map<string, glm::mat4>& SkeleteTransform, map<string, glm::mat4>& BoneTransform);
    void MapTransform(float TimeInSeconds, map<string, glm::mat4>& SkeleteTransforms, map<string, glm::mat4>& BoneTransforms, vector<glm::mat4>& SkeleteTransform, vector<glm::mat4>& BoneTransform);
    
private:
    #define NUM_BONES_PER_VEREX 4

//动画中骨骼运动数据
    struct BoneInfo
    {
        string BoneName;
        Matrix4f BoneOffset;
        Matrix4f FinalTransformation;
        

        BoneInfo()
        {     
             BoneOffset.InitIdentity();
            FinalTransformation.InitIdentity();
        }
    };
    //骨架信息
    struct SkeleteInfo
    {
        int SkeleteID;
        Matrix4f BoneOffset;
        Matrix4f FinalTransformation;
        Matrix4f BoneRootTransform;
        const aiNode* parent;
        const aiNode* node;

        SkeleteInfo()
        {
            SkeleteID = 0;
            node = NULL;
            parent = NULL;
            BoneOffset.InitIdentity();
            FinalTransformation.InitIdentity();
            BoneRootTransform.InitIdentity();
        }
    };
    
    struct VertexBoneData
    {        
        unsigned int IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);        
        }
        
        void AddBoneData(unsigned int BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void SetSkeleteBufferData();
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
    void SkinnedMesh::ReadSkeleteHeirarchy(const aiNode* pNode, const Matrix4f& ParentTransform, const aiNode* parent);
    
    bool InitFromScene(const aiScene* pScene, const string& Filename);
    void InitMesh(unsigned int MeshIndex,
                  const aiMesh* paiMesh,
                  vector<Vector3f>& Positions,
                  vector<Vector3f>& Normals,
                  vector<Vector2f>& TexCoords,
                  vector<VertexBoneData>& Bones,
                  vector<unsigned int>& Indices);
    bool LoadBones();
    void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
    bool InitMaterials(const aiScene* pScene, const string& Filename);
    SkinnedMesh::SkeleteInfo* FindBoneInfos(SkeleteInfo t);
    bool SkinnedMesh::FindNoneSkeleteInfos(SkeleteInfo t);
    Vector4f test(const Matrix4f& Right, const Vector4f m);
    bool SkinnedMesh::InitSkelete();
    void SkinnedMesh::CheckSkelete();
    glm::mat4 TransformMatrix4f(Matrix4f t);

    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    unsigned int m_SkeleteVAO;
    unsigned int m_SkeleteBuffers;

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    vector<MeshEntry> m_Entries;
    vector<Texture*> m_Textures;
     
    map<string, unsigned int> m_BoneMapping; // maps a bone name to its index
    unsigned int m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    vector<SkeleteInfo> m_SkeleteInfo;
    vector<SkeleteInfo> m_NoneSkeleteInfo;
    unsigned int m_NumSkelete;
    Matrix4f m_GlobalInverseTransform;
    
    const aiScene* m_pScene;
    static Assimp::Importer m_Importer;
};


#endif	/* OGLDEV_SKINNED_MESH_H */

