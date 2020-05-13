#include "PBRShader.h"

bool PBRShader::Init()
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



    return true;





}
