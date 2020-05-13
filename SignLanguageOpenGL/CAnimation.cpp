#include "CAnimation.h"




CAnimation::CAnimation()
{
    m_Interpolation = 0.1F;
}
CAnimation::~CAnimation()
{
    m_Interpolation = 0.1F;
    m_pScene = NULL;
}
CAnimation::CAnimation(string animationPath, CSkeleton skelete, map<string, string> mBoneMap)
{  
    m_BoneMap = mBoneMap;
    Assimp::Importer m_Importer;
    m_pScene = m_Importer.ReadFile(animationPath.c_str(), 0);
    m_Skeleton = skelete;
    m_Transform = vector<glm::mat4>();
    for (int i = 0; i < skelete.GetBoneCount(); i++)
    {
        m_Transform.push_back(glm::mat4(1.0f));
    }
    m_TotalTime = m_pScene->mAnimations[0]->mDuration;
    m_Interpolation = 0.1F;
    __SamplingClip();
   // m_Transform.resize(skelete.GetBoneCount());
}

void CAnimation::__SamplingClip()
{
    glm::mat4 Identity(1.0f);
  
    m_AnimationClips=vector<AnimationClip>();

    float AnimationTime = 0;
    while (AnimationTime < m_TotalTime)
    {
        __ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);
        AnimationClip temp;
        temp.m_Transform = m_Transform;
        temp.Time = AnimationTime;
        m_AnimationClips.push_back(temp);
        AnimationTime += m_Interpolation;
    }
    m_ClipCount = m_AnimationClips.size();
}


AnimationClip CAnimation::GetAnimationClip(int index)
{
    if (index > m_ClipCount)
    {
        return m_AnimationClips[m_ClipCount - 1];
    }
    return m_AnimationClips[index];
}

unsigned int CAnimation::__FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    return 0;
}


unsigned int CAnimation::__FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    return 0;
}


unsigned int CAnimation::__FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    return 0;
}


void CAnimation::__CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    unsigned int PositionIndex = __FindPosition(AnimationTime, pNodeAnim);
    unsigned int NextPositionIndex = (PositionIndex + 1);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;

    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void CAnimation::__CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    unsigned int RotationIndex = __FindRotation(AnimationTime, pNodeAnim);
    unsigned int NextRotationIndex = (RotationIndex + 1);

    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void CAnimation::__CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int ScalingIndex = __FindScaling(AnimationTime, pNodeAnim);
    unsigned int NextScalingIndex = (ScalingIndex + 1);

    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

glm::mat4 CAnimation::__TransformMat3(aiMatrix3x3 AssimpMatrix)
{
    glm::mat4 m(1.0f);
    m[0][0] = AssimpMatrix.a1; m[1][0] = AssimpMatrix.a2; m[2][0] = AssimpMatrix.a3; m[3][0] = 0;
    m[0][1] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[2][1] = AssimpMatrix.b3; m[3][1] = 0;
    m[0][2] = AssimpMatrix.c1; m[1][2] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[3][2] = 0;
    m[0][3] = 0;               m[1][3] = 0;               m[2][3] = 0;               m[3][3] = 1;

    return m;
}

glm::mat4 CAnimation::__TransformMat4(aiMatrix4x4 AssimpMatrix)
{
    glm::mat4 m(1.0f);
    m[0][0] = AssimpMatrix.a1; m[1][0] = AssimpMatrix.a2; m[2][0] = AssimpMatrix.a3; m[3][0] = AssimpMatrix.a4;
    m[0][1] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[2][1] = AssimpMatrix.b3; m[3][1] = AssimpMatrix.b4;
    m[0][2] = AssimpMatrix.c1; m[1][2] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[3][2] = AssimpMatrix.c4;
    m[0][3] = AssimpMatrix.d1; m[1][3] = AssimpMatrix.d2; m[2][3] = AssimpMatrix.d3; m[3][3] = AssimpMatrix.d4;

    return m;
}


void CAnimation::__ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
    string NodeName(pNode->mName.data);
    glm::mat4 NodeTransformation= __TransformMat4(pNode->mTransformation);

    if (m_pScene->HasAnimations())
    {
        const aiAnimation* pAnimation = m_pScene->mAnimations[0];



        const aiNodeAnim* pNodeAnim = __FindNodeAnim(pAnimation, NodeName);


        if (pNodeAnim) {
            // Interpolate scaling and generate scaling transformation matrix
            aiVector3D Scaling;
            __CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
            glm::mat4 ScalingM(1.0f);
            ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));
        

            // Interpolate rotation and generate rotation transformation matrix
            aiQuaternion RotationQ;
            __CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
            glm::mat4 RotationM = __TransformMat3(RotationQ.GetMatrix());

            // Interpolate translation and generate translation transformation matrix
            aiVector3D Translation;
            __CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
            glm::mat4 TranslationM(1.0f);
            TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

            // Combine the above transformations
            NodeTransformation = TranslationM * RotationM* ScalingM;
        }
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;
    
    int index = -1;
    if (m_BoneMap.size() > 0)
    {
       index = m_Skeleton.FindBones(m_BoneMap[NodeName]);
    }
    else
        index = m_Skeleton.FindBones(NodeName);
    if (index!=-1) {
        m_Transform[index] = glm::mat4(1.0f)* GlobalTransformation*m_Skeleton.m_Bones[index].m_BoneOffset;
    }
    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        __ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}

const aiNodeAnim* CAnimation::__FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
    for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    return NULL;
}


