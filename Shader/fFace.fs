#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 TSM_coord;

const int MaxLightNum=2;

unform sampler2D irrad1Tex;//ģ������ն�����
unform sampler2D irrad2Tex;
unform sampler2D irrad3Tex;
unform sampler2D irrad4Tex;
unform sampler2D irrad5Tex;
uniform sampler2D irrad6Tex;
unifrom vec3 gauss1w;//Ƥ�������RGBȨ��
unifrom vec3 gauss2w;
unifrom vec3 gauss3w;
unifrom vec3 gauss4w;
unifrom vec3 gauss5w;
uniform vec3 gauss6w;
uniform float mix;//����ɢ��仯��

uniform sampler2D TSMTex;//��͸����Ӱ����
uniform sample2D rhodTex;//����ǿ��ӳ������

uniform sample2D stretch32Tex;

//���㾵��BRDF��FRDF��Fresnel���亯��
float fresnelReflectance(vec3 H,vec3 V,float F0)
{
    float base=1.0-dot(V,H);
    float exponential=pow(base,5.0);
    return exponential+F0*(1.0-exponential);
}
//Ԥ����Beckmann����
float PHBeckmann(float ndoth,float m)
{
    float alpha=acos(ndoth);
    float ta=tan(alpha);
    float val=1.0/(m*m*pow(ndoth,4.0))*exp(-(ta*ta)/(m*m));
    return val;
}
//��Ⱦһ������Ļ����ķ�������һ��512*512���������Ԥ����
float KSTextureCompute()
{
    return 0.5*pow(PHBeckmann(tex.x,tex.y),0.1);
}

float KS_Skin_Specular(vec3 N,vec3 L,vec3 V,float m,float rho_s)
{
    float result=0.0;
    float ndotl=dot(N,L);
    if(ndotl>0.0)
    {
        vec3 h=L+V;
        vec3 H=normalize(h);
        float ndoth=dot(N,H);
        float PH=pow(2.0*texture(beckmannTex,vec2(ndoth,m)),10.0);
        float F=fresnelReflectance(H,V,0.028);
        float frSpec=max(PH*F/dot(h,h),0);
        result=ndotl*rho_s*frSpec;

	}

}


// ----------------------------------------------------------------------------
void main()
{		
   //�뿪�������ɢ��
   vec3 diffuseLight=0;
   //���ն��������
   vec4 irrad1tap=texture(irrad1Tex,TexCoords);
   vec4 irrad2tap=texture(irrad2Tex,TexCoords);
   vec4 irrad3tap=texture(irrad3Tex,TexCoords);
   vec4 irrad4tap=texture(irrad4Tex,TexCoords);
   vec4 irrad5tap=texture(irrad5Tex,TexCoords);
   vec4 irrad6tap=texture(irrad6Tex,TexCoords);

   diffuseLight+=gauss1w*irrad1tap.xyz;
   diffuseLight+=gauss2w*irrad2tap.xyz;
   diffuseLight+=gauss3w*irrad3tap.xyz;
   diffuseLight+=gauss4w*irrad4tap.xyz;
   diffuseLight+=gauss5w*irrad5tap.xyz;
   diffuseLight+=gauss6w*irrad6tap.xyz;

   //���½������������һ������ɫ
   vec3 normConst=gauss1w+gauss2w+gauss3w+gauss4w+gauss5w+gauss6w;
   diffuseLight /=normConst;//���¹�һ������ɫ�������

   //���޸Ĺ���TSM����ȫ��ɢ��
   vec3 TSMtap= texture(TSMTex,TSM_coord.xy/TSM_coord.w);
   //ͨ�������4��ƽ�����(��mmΪ��λ)
   vec4 thickness_mm=1.0*-(1.0/2.0)*log(vec4(irrad2tap.w,irrad3tap.w,irrad4tap.w,irrad5tap.w));
   //ʹ��������������������
   vec2 strectchTap=texture(stretch32Tex,TexCoords);
   //ƽ��
   float stretchaval=0.5*(strectchTap.x,strectchTap.y);
   //7-tap�������˹
   vec4 a_values=vec4(0.433,0.753,1.412,23.722);
   vec4 inv_a=-1.0/(2.0*a_values*a_values);
   vec4 fades=exp(thickness_mm*thickness_mm*inv_a);

   float textureScale=1024.0*0.1/stretchval;
   float blendFactor4=saturate(textureScale*length(v2f.c_texCoord.xy-TSMtap.yz)/(a_values.y*6.0));
   float blendFactor5=saturate(textureScale*length(v2f.c_texCoord.xy-TSMtap.yz)/(a_values.z*6.0));
   float blendFactor6=saturate(textureScale*length(v2f.c_texCoord.xy-TSMtap.yz)/(a_values.w*6.0));

   diffuseLight +=gauss4w/normConst*fades.y*blendFactor4*texture(irrad4Tex,TSMtap.yz).xyz;
   diffuseLight +=gauss5w/normConst*fades.z*blendFactor5*texture(irrad5Tex,TSMtap.yz).xyz;
   diffuseLight +=gauss6w/normConst*fades.w*blendFactor6*texture(irrad6Tex,TSMtap.yz).xyz;

   //��һ��diffuseColor��ɫͼ����Ƥ����ɫ
   diffuseLight*=pow(texture(diffuseColorTex,TexCoords),1.0-mix);

   //Ϊÿ�����ռ��㾵�淴��



}