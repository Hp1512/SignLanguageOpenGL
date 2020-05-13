#pragma once
#include<glad/glad.h>;
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

/**
 * @brief ����
 * ���ʻ��࣬���ò�����ز���
 */
class CMaterial
{
private:
	unsigned int m_ID;//! ��ɫ��id
	glm::mat4 m_Projection;//! ������ɫ������Ҫ����Ļת������
	glm::mat4 m_View;//! ������ɫ������Ҫ�Ĳü�ת������
	glm::mat4 m_Model; // !������ɫ������Ҫ��ģ��ת������
public:
	/**
	 * @brief ����
	 * @param vertexPath    ������ɫ��·��
	 *@param fragmentPath   Ƭ����ɫ��·��
	 *@param geometryPath   ������ɫ��·��
	 */
	CMaterial(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	CMaterial();
	~CMaterial();
	/**
	 * @brief ����ʹ��
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void Use();
	/**
	 * @brief ���ò���ֵ
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void SetBool(const std::string& name, bool value) const;
	/**
     * @brief ��������ֵ
     *@param name    ����
	 *@param value    ���õ�ֵ
     */
	void SetInt(const std::string& name, int value) const;
	/**
     * @brief ���ø���ֵ
     *@param name    ����
	 *@param value    ���õ�ֵ
     */
	void SetFloat(const std::string& name, float value) const;
	/**
	 * @brief ������ά����
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void SetVec4(const std::string& name, glm::vec4 value)const;
	/**
	* @brief ������ά����
	*@param name    ����
	*@param value    ���õ�ֵ
	*/
	void SetMat4(const std::string& name, glm::mat4 value)const;
	/**
     * @brief ������ά����
     *@param name    ����
     *@param value    ���õ�ֵ
     */
	void SetVec3(const std::string& name, glm::vec3 value)const;
	/**
	 * @brief ������Ļ����ת������
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void SetProjection(glm::mat4 value);
	/**
	 * @brief ���òü�����ת������
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void SetView(glm::mat4 value);
	/**
	 * @brief ����ģ������ת������
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void SetModel(glm::mat4 value);
	/**
	 * @brief ���ù�������
	 *@param name    ����
	 *@param value    ���õ�ֵ
	 */
	void SetBones(std::vector<glm::mat4> value);

};

