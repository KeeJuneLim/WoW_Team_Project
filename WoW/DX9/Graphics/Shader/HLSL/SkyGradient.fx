
#include "Base.fx"

float3 gApexColor = float3(0.1f, 0.1f, 0.6f);
float3 gCenterColor = float3(0.3f, 0.3f, 1.0f);
float gRadius = 6.0f;

struct VS_INPUT 
{
    float4 mPosition : POSITION;
};

struct PS_INPUT
{
    float4 mPosition : POSITION;
    float4 localPos : TEXCOORD1;
};

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPosition = Input.mPosition;
    Output.mPosition = mul(Output.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjMatrix);
    
    Output.localPos = Input.mPosition;
    
   return Output;
}

float4 PS(PS_INPUT Input) : COLOR
{   
    float4 lightDist = distance(normalize(-gWorldLightDir) * gRadius, Input.localPos.xyz);
    float4 lightIntensity = 1.0f / (1.0f + lightDist) * float4(gLightColor, 1);
    
    float height = saturate(Input.localPos.y / gRadius);
    float4 lerpColor = float4(lerp(gCenterColor, gApexColor, height), 1);
    return lerpColor + lightIntensity;
}


technique Shader
{
   pass Pass_0
   {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
   }

}

