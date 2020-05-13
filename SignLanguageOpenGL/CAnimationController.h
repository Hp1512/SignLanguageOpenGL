#pragma once
#include<vector>
#include "CAnimation.h"
#include "CSkeletonMaps.h"
using namespace std;

/**
 * @brief 动画播放的状态
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
 * @brief 动画控制，控制动画的暂停，结束，开始
 * 通过对动画序列的数据的读取操作，达到对动画的控制
 */
class CAnimationController
{
private:
	//! 动画循环
	bool m_bLoop;
	//! 动画数据
	vector<CAnimation> m_Animations;
	//! 播放动画速度
	int m_Speed;
	//! 动画播放状态
	EAnimationState m_AnimationState;
	//! 动画骨骼
	CSkeleton m_Skeleton;
	//! 骨骼映射
	map<string, string> m_BoneMaps;
	//! 目前骨骼数据
	vector<glm::mat4> m_CurrentTransform;
	//!初始骨骼数据
	vector<glm::mat4> m_InitTransform;
	//!目前动画索引
	int m_CurrentAnimIndex = 0;
	//!目前动画片段索引
	int m_CurrentClipIndex = 0;
public:
	/**
     * @brief 获取当前骨骼数据
     * @param index    帧索引
     *
     * @return 返回说明
     *     -<em>vector<glm::mat4></em> 当前骨骼数据
     */
	const vector<glm::mat4> GetCurrrntTransform()const { return m_CurrentTransform; }
	/**
	 * @brief 设置动画播放速度
	 * @param speed    当前速度
	 *
	 */
	void SetAnimationSpeed(int speed) { m_Speed = speed; }
	/**
	 * @brief 添加动画
	 * @param speed    当前动画数据路径
	 *
	 */
	void AddAnimations(string animationPath);
	/**
	 * @brief 当前动画是否循环
	 * @param speed    当前动画路径
	 *
	 */
	void SetLoop(bool bLoop) { m_bLoop = bLoop; };
	/**
	 * @brief 清除动画
	 *
	 */
	void ClearAnimations();
	/**
	 * @brief 停止播放动画
	 *
	 */
	void StopAnimations();
	/**
	 * @brief 开始播放动画
	 *
	 */
	void StartAnimations();
	/**
	 * @brief 重新开始播放动画
	 *
	 */
	void ReStartAnimations();
	/**
	 * @brief 停止播放动画
	 *
	 */
	void PauseAnimations();
	/**
	 * @brief 更新动画状态
	 *
	 */
	void UpdateAnimationState();

	CAnimationController();
    /**
	 * @brief 动画控制器
	 *
	 */
	CAnimationController(CSkeleton skelete, CSkeletonMaps maps);
	~CAnimationController();

private:
	
	void __UpdateAnimationIndex();


};

