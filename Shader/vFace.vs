#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

const int MAX_BONES=210;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

int anim=0;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int animation;
uniform mat4 gBones[MAX_BONES];

void main()
{
    mat4 BoneTransform=mat4(1.0f);
    anim=animation;

    if(anim!=0)
    {
        BoneTransform = gBones[BoneIDs[0]] * Weights[0];
        BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
        BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
        BoneTransform     += gBones[BoneIDs[3]] * Weights[3];
    }
   
    vec4 PosL    = BoneTransform * vec4(aPos, 1.0);

    TexCoords = aTexCoords;
    WorldPos = vec3(model * PosL);
    Normal = mat3(model) * aNormal;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}