#include<string>
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CMaterial.h"
#include <assimp\scene.h>
#include "CSkinMesh.h"
#include "CSkeleton.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include "CDefaultMat.h"
#include "CSkeletonMaps.h"
#include "CAnimationController.h"

using namespace std;

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

/**
 * @brief 模型
 * 读取模型网格、模型骨骼、模型骨骼映射表;动画控制，材质管理
 */
class CModel
{

private:
	string m_sName;//! 模型名
	CSkeleton m_Skeleton;//!骨骼
	CSkinMesh m_SkinMesh;//!网格
	CSkeletonMaps m_SkeletonMaps;//!骨骼映射
	CAnimationController* m_CAnimationController;//!动画控制器
	glm::mat4 m_Transform;//!模型变换，旋转、缩放、平移
	glm::mat4 m_Projection;//!屏幕坐标转换矩阵
	glm::mat4 m_View;//!裁剪坐标转换器

	bool m_bShowSkeleton; //!是否绘制骨骼

    vector<CMaterial> m_MeshMaterials;//!模型所采用的材质
    CMaterial m_SkeletonMaterial; //!骨骼所采用的材质
   
public:

	/**
	 * @brief 模型信息
	 * @param path    模型所在的路径
	 *
	 */
	CModel(string path);
	CModel();
	~CModel();
	/**
     * @brief 绘制
     * @param windowIndex    窗口索引
     *
     */
	void Draw(int windowIndex);
	/**
     * @brief 绑定模型绘制数据
     * @param windowIndex    窗口索引
     *
     */
	void BindModelBufferData(int windowIndex) { m_SkinMesh.BindMeshesBufferData(windowIndex); m_Skeleton.BindSkeleteBufferData(windowIndex);}
	/**
	 * @brief 设置是否绘制骨骼
	 * @param bShow    是否绘制数据
	 *
	 */
	void SetShowSkeletonState(bool bShow) { m_bShowSkeleton = bShow; }
	/**
     * @brief 获取骨骼数据
     *
     * @return 当前模型的骨骼数据
     *     -<em>CSkeleton</em> 帧数据
     */
	const CSkeleton& GetSkeletion()const{ return m_Skeleton; };
	/**
	 * @brief 设置模型屏幕坐标转换矩阵
	 * @param mat    转换矩阵
	 *
	 */
	void SetProjection(glm::mat4 mat) { m_Projection = mat; };
	/**
	 * @brief 设置模型屏幕坐标转换矩阵
	 * @param mat    转换矩阵
	 *
	 */
	void SetView(glm::mat4 mat) { m_View = mat; };
	/**
	 * @brief 设置模型平移。旋转，缩放
	 * @param m_Translate     平移坐标
	 * @param m_Translate     旋转坐标
	  * @param m_Translate    缩放坐标
	 */
	void TransformModel(glm::vec3 m_Translate, glm::vec3 m_Rotation, glm::vec3 m_Scale);
	/**
	 * @brief 将材质应用到指定网格上
	 * @param index    网格索引
	 * @param CMaterial    材质
	 *
	 */
	void SetMeshMaterials(int index, CMaterial mat);
	/**
	 * @brief 得到动画控制器
	 *
	 * @return 当前返回该模型的动画控制器
	 *     -<em>CAnimationController</em> 动画控制器
	 */
    CAnimationController* GetAnimationController() { return m_CAnimationController; }

private:
	

};

