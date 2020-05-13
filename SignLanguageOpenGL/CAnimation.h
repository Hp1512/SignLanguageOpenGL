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
 * @brief �������У�����Ƭ��
 */
struct AnimationClip
{
    float Time;/*!< ʱ�� */
    vector<glm::mat4> m_Transform;/*!< �������� */
};
/**
 * @brief �������ݶ�ȡ����װ
 * ͨ����ȡ�����ļ�������֡���ݣ���װ��ĳʱ���Ϲ������ݣ���ɶ������ݵĶ�ȡ����װ
 */
class CAnimation
{
private:
    //! ��������
    vector<AnimationClip> m_AnimationClips;
    //!��ǰ��ȡ�Ĺ�������
    vector<glm::mat4>m_Transform;
    //!����ӳ���
    map<string, string> m_BoneMap;
    //!���Բ�������
    float m_Interpolation;
    //!��������֡��
    int m_ClipCount;
public:
    CAnimation();
    ~CAnimation();
     /**
     * @brief ��װ֡����
     * @param animationPath    �����ļ�����·��
     *@param skelete    ģ������Ӧ�Ĺ���
     *@param m_BoneMap   ����ӳ���
     */
    CAnimation(string animationPath,CSkeleton skelete, map<string, string> m_BoneMap);
    /**
     * @brief ��ȡ����ĳ֡����
     * @param index    ֡����
     *
     * @return ����˵��
     *     -<em>AnimationClip</em> ֡����
     */
    AnimationClip GetAnimationClip(int index);
    /**
    * @brief ��ȡ�������ݵ���֡��
    * @param index    ֡����
    *
    * @return ��ǰ֡������
    *     -<em>m_ClipCount</em> ֡����
    */
    const int GetAnimationClipCount() { return m_ClipCount; }
    
private:
    //! ����
    CSkeleton m_Skeleton;
    //! ԭʼ����
    const aiScene* m_pScene;
    //! ������������ʱ��
    float m_TotalTime;
    //!�������ݲ������ܸ���
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

