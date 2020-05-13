#include "BoneShader.h"
#include <cassert>

bool BoneShader::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\default.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "I:\\SignLanguageOpenGL\\SignLanguageOpenGL\\Shader\\default.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
  

    m_WorldMatrixLocation = GetUniformLocation("World");
    m_WVPLocation = GetUniformLocation("gWVP");

   // m_SkeleteTransformLocation = GetUniformLocation("SkeleteTransform");

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_SkeleteTransformLocation); i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
       SNPRINTF(Name, sizeof(Name), "SkeleteTransform[%d]", i);
       m_SkeleteTransformLocation[i] = GetUniformLocation(Name);
    }

    return true;

  


   
}

void BoneShader::SetBoneTransform(unsigned int Index, const Matrix4f& Transform)
{
    //Transform.Print();
    glUniformMatrix4fv(m_SkeleteTransformLocation[Index], 1, GL_TRUE, (const GLfloat*)Transform);
}

void BoneShader::SetWorldMatrix(const Matrix4f& World)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)World);
}

void BoneShader::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP);
}

void BoneShader::SetTest(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_Test, 1, GL_TRUE, (const GLfloat*)WVP);
}