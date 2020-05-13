#ifndef CANIMATION_H
#define CANIMATION_H
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CSkeleton.h"
using namespace std;

/**
 * @brief 动画序列，动画片段
 */
struct AnimationClip
{
    float Time;/*!< 时间 */
    vector<glm::mat4> m_Transform;/*!< 骨骼数据 */
};
/**
 * @brief 动画数据读取，封装
 * 通过读取动画文件，或获得帧数据，封装成某时间上骨骼数据，完成动画数据的读取及封装
 */
class CAnimation
{
private:
    //! 动画序列
    vector<AnimationClip> m_AnimationClips;
    //!当前获取的骨骼数据
    vector<glm::mat4>m_Transform;
    //!骨骼映射表
    map<string, string> m_BoneMap;
    //!线性采样参数
    float m_Interpolation;
    //!动画序列帧数
    int m_ClipCount;
public:
    CAnimation();
    ~CAnimation();
     /**
     * @brief 封装帧数据
     * @param animationPath    动画文件所在路径
     *@param skelete    模型所对应的骨骼
     *@param m_BoneMap   骨骼映射表
     */
    CAnimation(string animationPath,CSkeleton skelete, map<string, string> m_BoneMap);
    /**
     * @brief 获取动画某帧数据
     * @param index    帧索引
     *
     * @return 返回说明
     *     -<em>AnimationClip</em> 帧数据
     */
    AnimationClip GetAnimationClip(int index);
    /**
    * @brief 获取动画数据的总帧数
    * @param index    帧索引
    *
    * @return 当前帧的总数
    *     -<em>m_ClipCount</em> 帧数据
    */
    const int GetAnimationClipCount() { return m_ClipCount; }
    
private:
    //! 骨骼
    CSkeleton m_Skeleton;
    //! 原始数据
    const aiScene* m_pScene;
    //! 动画数据中总时长
    float m_TotalTime;
    //!动画数据采样的总个数
    int m_SampleCount;

private:
    void __SamplingClip();
	unsigned int __FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int __FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int __FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

    void __CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    
    void __CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
  
    void __CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
 
    void __ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);

    const aiNodeAnim* __FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);

    glm::mat4 __TransformMat4(aiMatrix4x4 AssimpMatrix);

    glm::mat4 __TransformMat3(aiMatrix3x3 AssimpMatrix);
};
#endif

