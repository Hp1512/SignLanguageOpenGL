#pragma once
#include<vector>
#include "CAnimation.h"
#include "CSkeletonMaps.h"
using namespace std;

/**
 * @brief �������ŵ�״̬
 */
enum EAnimationState
{
	Start,
	Stop,
	Pause,
	ReStart,
	None
};
/**
 * @brief �������ƣ����ƶ�������ͣ����������ʼ
 * ͨ���Զ������е����ݵĶ�ȡ�������ﵽ�Զ����Ŀ���
 */
class CAnimationController
{
private:
	//! ����ѭ��
	bool m_bLoop;
	//! ��������
	vector<CAnimation> m_Animations;
	//! ���Ŷ����ٶ�
	int m_Speed;
	//! ��������״̬
	EAnimationState m_AnimationState;
	//! ��������
	CSkeleton m_Skeleton;
	//! ����ӳ��
	map<string, string> m_BoneMaps;
	//! Ŀǰ��������
	vector<glm::mat4> m_CurrentTransform;
	//!��ʼ��������
	vector<glm::mat4> m_InitTransform;
	//!Ŀǰ��������
	int m_CurrentAnimIndex = 0;
	//!Ŀǰ����Ƭ������
	int m_CurrentClipIndex = 0;
public:
	/**
     * @brief ��ȡ��ǰ��������
     * @param index    ֡����
     *
     * @return ����˵��
     *     -<em>vector<glm::mat4></em> ��ǰ��������
     */
	const vector<glm::mat4> GetCurrrntTransform()const { return m_CurrentTransform; }
	/**
	 * @brief ���ö��������ٶ�
	 * @param speed    ��ǰ�ٶ�
	 *
	 */
	void SetAnimationSpeed(int speed) { m_Speed = speed; }
	/**
	 * @brief ��Ӷ���
	 * @param speed    ��ǰ��������·��
	 *
	 */
	void AddAnimations(string animationPath);
	/**
	 * @brief ��ǰ�����Ƿ�ѭ��
	 * @param speed    ��ǰ����·��
	 *
	 */
	void SetLoop(bool bLoop) { m_bLoop = bLoop; };
	/**
	 * @brief �������
	 *
	 */
	void ClearAnimations();
	/**
	 * @brief ֹͣ���Ŷ���
	 *
	 */
	void StopAnimations();
	/**
	 * @brief ��ʼ���Ŷ���
	 *
	 */
	void StartAnimations();
	/**
	 * @brief ���¿�ʼ���Ŷ���
	 *
	 */
	void ReStartAnimations();
	/**
	 * @brief ֹͣ���Ŷ���
	 *
	 */
	void PauseAnimations();
	/**
	 * @brief ���¶���״̬
	 *
	 */
	void UpdateAnimationState();

	CAnimationController();
    /**
	 * @brief ����������
	 *
	 */
	CAnimationController(CSkeleton skelete, CSkeletonMaps maps);
	~CAnimationController();

private:
	
	void __UpdateAnimationIndex();


};

