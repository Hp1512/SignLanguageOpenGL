#include "CAnimationController.h"

CAnimationController::CAnimationController()
{
	m_bLoop = false;
	m_Speed = 1;
	m_AnimationState = None;
}
CAnimationController::CAnimationController(CSkeleton skelete,CSkeletonMaps maps)
{
	m_bLoop = false;
	m_Speed = 1;
	m_AnimationState = None;
	m_Animations = vector<CAnimation>();
	m_Skeleton = skelete;
	m_BoneMaps = maps.GetSkeleteonMaps();
	m_InitTransform = vector<glm::mat4>();
	for (int i = 0; i < skelete.GetBoneCount(); i++)
	{
		m_InitTransform.push_back(glm::mat4(1.0f));
	}
}
CAnimationController::~CAnimationController()
{

}

void CAnimationController::AddAnimations(string animationPath)
{
	CAnimation tmp = CAnimation(animationPath, m_Skeleton, m_BoneMaps);
	m_Animations.push_back(tmp);
}

void CAnimationController::ClearAnimations()
{
	m_AnimationState = None;
	m_CurrentTransform = m_InitTransform;
	m_Animations.clear();
}
void CAnimationController::StopAnimations()
{
	m_AnimationState = Stop;
}
void CAnimationController::StartAnimations()
{	
	m_AnimationState = Start;	
}
void CAnimationController::ReStartAnimations()
{
	m_AnimationState = ReStart;
}
void CAnimationController::PauseAnimations()
{
	m_AnimationState = Pause;
}

void CAnimationController::__UpdateAnimationIndex()
{
	m_CurrentClipIndex+=m_Speed;
	if (m_CurrentAnimIndex < m_Animations.size())
	{
		if (m_CurrentClipIndex < m_Animations[m_CurrentAnimIndex].GetAnimationClipCount())
		{
			m_CurrentTransform = m_Animations[m_CurrentAnimIndex].GetAnimationClip(m_CurrentClipIndex).m_Transform;
		}
		else
		{
			m_CurrentAnimIndex += m_Speed;
			if (m_CurrentAnimIndex < m_Animations.size())
			{
				m_CurrentClipIndex = 0;
				m_CurrentTransform = m_Animations[m_CurrentAnimIndex].GetAnimationClip(m_CurrentClipIndex).m_Transform;
				m_CurrentClipIndex += m_Speed;
			}
			else
			{
				if (m_bLoop)
				{
					m_CurrentAnimIndex = 0;
					m_CurrentClipIndex = 0;
					m_CurrentTransform = m_Animations[m_CurrentAnimIndex].GetAnimationClip(m_CurrentClipIndex).m_Transform;
				}
			}
		}
	}
}

void CAnimationController::UpdateAnimationState()
{
	switch (m_AnimationState)
	{
	case Start:
		__UpdateAnimationIndex();
		break;
	case ReStart:
		m_CurrentClipIndex = 0;
		m_CurrentAnimIndex = 0;
		__UpdateAnimationIndex();
		break;
	case Stop:
		m_CurrentClipIndex = 0;
		m_CurrentAnimIndex = 0;
		m_CurrentTransform = m_InitTransform;
		break;
	case Pause:
		m_CurrentTransform = m_Animations[m_CurrentAnimIndex].GetAnimationClip(m_CurrentClipIndex).m_Transform;
		break;
	case None:
		m_CurrentClipIndex = 0;
		m_CurrentAnimIndex = 0;
		m_CurrentTransform= m_InitTransform;
		break;
	default:
		m_CurrentClipIndex = 0;
		m_CurrentAnimIndex = 0;
		m_CurrentTransform = m_Animations[m_CurrentAnimIndex].GetAnimationClip(m_CurrentClipIndex).m_Transform;
		break;
	}
}