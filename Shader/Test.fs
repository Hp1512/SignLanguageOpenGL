#version 330 core
out vec4 FragColor;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform sampler2D albedoMap;

void main()
{  
     FragColor=vec4(texture(albedoMap, TexCoords).xyz,1.0);   
}