#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;                                               
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

 
const int MAX_BONES = 210;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 bones[MAX_BONES];

void main()
{   
    mat4 BoneTransform = bones[BoneIDs[0]] * Weights[0];
    BoneTransform     += bones[BoneIDs[1]] * Weights[1];
    BoneTransform     += bones[BoneIDs[2]] * Weights[2];
    BoneTransform     += bones[BoneIDs[3]] * Weights[3];

    vec4 PosL    = BoneTransform * vec4(Position, 1.0f); 

    TexCoords = TexCoords;
    WorldPos = vec3(model * PosL);
    Normal = mat3(model) * Normal;   

    gl_Position  = projection * view * model *PosL;

}
