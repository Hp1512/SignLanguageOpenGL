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

#ifndef MESH_H
#define	MESH_H

#include <map>
#include <vector>
//#include "stb.h"
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ogldev_texture.h"

using namespace std;

// Stores an edge by its vertices and force an order between them
struct Edge
{
    Edge(unsigned int _a, unsigned int _b)
    {
        assert(_a != _b);
        
        if (_a < _b)
        {
            a = _a;
            b = _b;                   
        }
        else
        {
            a = _b;
            b = _a;
        }
    }

    void Print()
    {
        printf("Edge %d %d\n", a, b);
    }
    
    unsigned int a=0;
    unsigned int b=0;
};


struct Neighbors
{
    unsigned int n1=0;
    unsigned int n2=0;
    
    Neighbors()
    {
        n1 = n2 = (int)-1;
    }
    
    void AddNeigbor(int n)
    {
        if (n1 == -1) {
            n1 = n;
        }
        else if (n2 == -1) {
            n2 = n;
        }
        else {
            assert(0);
        }
    }
    
    int GetOther(int me) const
    {
        if (n1 == me) {
            return n2;
        }
        else if (n2 == me) {
            return n1;
        }
        else {
            assert(0);
        }

        return 0;
    }
};


struct CompareEdges
{
    bool operator()(const Edge& Edge1, const Edge& Edge2)
    {
        if (Edge1.a < Edge2.a) {
            return true;
        }
        else if (Edge1.a == Edge2.a) {
            return (Edge1.b < Edge2.b);
        }        
        else {
            return false;
        } 
        return true;
    }
};


struct CompareVectors
{
    bool operator()(const aiVector3D& a, const aiVector3D& b)
    {
        if (a.x < b.x) {
            return true;
        }
        else if (a.x == b.x) {
            if (a.y < b.y) {
                return true;
            }
            else if (a.y == b.y) {
                if (a.z < b.z) {
                    return true;
                }
            }
        }
        
        return false;
    }
};


struct Face
{
    int Indices[3];
    
    int GetOppositeIndex(const Edge& e) const
    {
        for (int i = 0 ; i < sizeof(Indices) / sizeof(Indices[0]); i++) {
            int Index = Indices[i];
            
            if (Index != e.a && Index != e.b) {
                return Index;
            }
        }
        
        assert(0);

        return 0;
    }
};

class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const string& Filename, bool WithAdjacencies);

    void Render();
	
    int NumBones() const
    {
        return m_NumBones;
    }
    
    void BoneTransform(float TimeInSeconds, vector<aiMatrix4x4>& Transforms);
    
private:
    #define NUM_BONES_PER_VEREX 4

    struct BoneInfo
    {
        aiMatrix4x4 BoneOffset;
        aiMatrix4x4 FinalTransformation;

        BoneInfo()
        {
            BoneOffset= aiMatrix4x4();
            FinalTransformation= aiMatrix4x4();
        }
    };
    
    struct VertexBoneData
    {        
        int IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            memset(IDs, 0, sizeof(IDs));;
            memset(Weights, 0, sizeof(Weights));  
        }
        
        void AddBoneData(int BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
    bool InitFromScene(const aiScene* pScene, const string& Filename);
    void FindAdjacencies(const aiMesh* paiMesh, vector<unsigned int>& Indices);
    void InitMesh(unsigned int MeshIndex,
                  const aiMesh* paiMesh,
                  vector<glm::vec3>& Positions,
                  vector<glm::vec3>& Normals,
                  vector<glm::vec2>& TexCoords,
                  vector<VertexBoneData>& Bones,
                  vector<unsigned int>& Indices);
    void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
    bool InitMaterials(const aiScene* pScene, const string& Filename);
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
     
    map<string,int> m_BoneMapping; // maps a bone name to its index
    int m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    aiMatrix4x4 m_GlobalInverseTransform;

    std::map<Edge, Neighbors, CompareEdges> m_indexMap;
    std::map<aiVector3D, int, CompareVectors> m_posMap;    
    std::vector<Face> m_uniqueFaces;
    bool m_withAdjacencies;

    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
};


#endif	/* MESH_H */

