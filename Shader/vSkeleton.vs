#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                                                                            
layout (location = 1) in vec2 SkeleteID;

 
const int MAX_BONES = 210;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 bones[MAX_BONES];

void main()
{       
    vec4 PosL    = bones[int(SkeleteID.x)] * vec4(Position, 1.0f); 


    gl_Position  = projection * view * model * PosL;

}
