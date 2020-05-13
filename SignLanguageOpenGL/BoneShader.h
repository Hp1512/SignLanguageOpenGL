
#include "technique.h"
#include "ogldev_math_3d.h"

class BoneShader : public Technique
{
public:
	virtual bool Init();
	void SetBoneTransform(unsigned int Index, const Matrix4f& Transform);
	void SetWorldMatrix(const Matrix4f& World);
	void BoneShader::SetWVP(const Matrix4f& WVP);
	void SetTest(const Matrix4f& WVP);

private:
	GLuint m_SkeleteTransformLocation[210];
	GLuint m_WorldMatrixLocation;
	GLuint m_WVPLocation;
	GLuint m_Test;
};

