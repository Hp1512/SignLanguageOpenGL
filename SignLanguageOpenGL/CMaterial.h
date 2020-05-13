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
 * @brief 材质
 * 材质基类，设置材质相关参数
 */
class CMaterial
{
private:
	unsigned int m_ID;//! 着色器id
	glm::mat4 m_Projection;//! 顶点着色器所需要的屏幕转换矩阵
	glm::mat4 m_View;//! 顶点着色器所需要的裁剪转换矩阵
	glm::mat4 m_Model; // !顶点着色器所需要的模型转换矩阵
public:
	/**
	 * @brief 材质
	 * @param vertexPath    顶点着色器路径
	 *@param fragmentPath   片段着色器路径
	 *@param geometryPath   几何着色器路径
	 */
	CMaterial(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	CMaterial();
	~CMaterial();
	/**
	 * @brief 材质使用
	 *@param name    名字
	 *@param value    设置的值
	 */
	void Use();
	/**
	 * @brief 设置布尔值
	 *@param name    名字
	 *@param value    设置的值
	 */
	void SetBool(const std::string& name, bool value) const;
	/**
     * @brief 设置整数值
     *@param name    名字
	 *@param value    设置的值
     */
	void SetInt(const std::string& name, int value) const;
	/**
     * @brief 设置浮点值
     *@param name    名字
	 *@param value    设置的值
     */
	void SetFloat(const std::string& name, float value) const;
	/**
	 * @brief 设置四维向量
	 *@param name    名字
	 *@param value    设置的值
	 */
	void SetVec4(const std::string& name, glm::vec4 value)const;
	/**
	* @brief 设置四维矩阵
	*@param name    名字
	*@param value    设置的值
	*/
	void SetMat4(const std::string& name, glm::mat4 value)const;
	/**
     * @brief 设置三维向量
     *@param name    名字
     *@param value    设置的值
     */
	void SetVec3(const std::string& name, glm::vec3 value)const;
	/**
	 * @brief 设置屏幕坐标转换矩阵
	 *@param name    名字
	 *@param value    设置的值
	 */
	void SetProjection(glm::mat4 value);
	/**
	 * @brief 设置裁剪坐标转换矩阵
	 *@param name    名字
	 *@param value    设置的值
	 */
	void SetView(glm::mat4 value);
	/**
	 * @brief 设置模型坐标转换矩阵
	 *@param name    名字
	 *@param value    设置的值
	 */
	void SetModel(glm::mat4 value);
	/**
	 * @brief 设置骨骼数据
	 *@param name    名字
	 *@param value    设置的值
	 */
	void SetBones(std::vector<glm::mat4> value);

};

