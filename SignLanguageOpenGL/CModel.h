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
 * @brief ģ��
 * ��ȡģ������ģ�͹�����ģ�͹���ӳ���;�������ƣ����ʹ���
 */
class CModel
{

private:
	string m_sName;//! ģ����
	CSkeleton m_Skeleton;//!����
	CSkinMesh m_SkinMesh;//!����
	CSkeletonMaps m_SkeletonMaps;//!����ӳ��
	CAnimationController* m_CAnimationController;//!����������
	glm::mat4 m_Transform;//!ģ�ͱ任����ת�����š�ƽ��
	glm::mat4 m_Projection;//!��Ļ����ת������
	glm::mat4 m_View;//!�ü�����ת����

	bool m_bShowSkeleton; //!�Ƿ���ƹ���

    vector<CMaterial> m_MeshMaterials;//!ģ�������õĲ���
    CMaterial m_SkeletonMaterial; //!���������õĲ���
   
public:

	/**
	 * @brief ģ����Ϣ
	 * @param path    ģ�����ڵ�·��
	 *
	 */
	CModel(string path);
	CModel();
	~CModel();
	/**
     * @brief ����
     * @param windowIndex    ��������
     *
     */
	void Draw(int windowIndex);
	/**
     * @brief ��ģ�ͻ�������
     * @param windowIndex    ��������
     *
     */
	void BindModelBufferData(int windowIndex) { m_SkinMesh.BindMeshesBufferData(windowIndex); m_Skeleton.BindSkeleteBufferData(windowIndex);}
	/**
	 * @brief �����Ƿ���ƹ���
	 * @param bShow    �Ƿ��������
	 *
	 */
	void SetShowSkeletonState(bool bShow) { m_bShowSkeleton = bShow; }
	/**
     * @brief ��ȡ��������
     *
     * @return ��ǰģ�͵Ĺ�������
     *     -<em>CSkeleton</em> ֡����
     */
	const CSkeleton& GetSkeletion()const{ return m_Skeleton; };
	/**
	 * @brief ����ģ����Ļ����ת������
	 * @param mat    ת������
	 *
	 */
	void SetProjection(glm::mat4 mat) { m_Projection = mat; };
	/**
	 * @brief ����ģ����Ļ����ת������
	 * @param mat    ת������
	 *
	 */
	void SetView(glm::mat4 mat) { m_View = mat; };
	/**
	 * @brief ����ģ��ƽ�ơ���ת������
	 * @param m_Translate     ƽ������
	 * @param m_Translate     ��ת����
	  * @param m_Translate    ��������
	 */
	void TransformModel(glm::vec3 m_Translate, glm::vec3 m_Rotation, glm::vec3 m_Scale);
	/**
	 * @brief ������Ӧ�õ�ָ��������
	 * @param index    ��������
	 * @param CMaterial    ����
	 *
	 */
	void SetMeshMaterials(int index, CMaterial mat);
	/**
	 * @brief �õ�����������
	 *
	 * @return ��ǰ���ظ�ģ�͵Ķ���������
	 *     -<em>CAnimationController</em> ����������
	 */
    CAnimationController* GetAnimationController() { return m_CAnimationController; }

private:
	

};

