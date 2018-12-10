#include "base.fx"

float gTime;
float gBumpiness = float(0.25f);
float gUVSpeed = float(0.02f);
float4x4 rotMat;

struct VS_input 
{
    float4 Position : POSITION;
    float3 Normal: NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PS_input 
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 ViewDir: TEXCOORD2;
    float3 Normal : TEXCOORD4;
};

PS_input VS(VS_input input)
{
    PS_input Output;
    Output.Position = mul(input.Position, gWorldMatrix);
	
    float3 viewDir = normalize(Output.Position.xyz - gWorldCamPos.xyz);
    Output.ViewDir = viewDir;
    
    Output.Position = mul(Output.Position, gViewMatrix);
    Output.Position = mul(Output.Position, gProjMatrix);
    
    float3 worldNormal = mul(input.Normal, (float3x3) gWorldMatrix);
    worldNormal = normalize(worldNormal);
    Output.Normal = worldNormal;
	
    Output.TexCoord = input.TexCoord;

    return Output;
}


texture DiffuseMapTex;
sampler2D DiffuseMapSampler = sampler_state
{
    Texture = (DiffuseMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
};
texture DiffuseMap1Tex;
sampler2D DiffuseMap1Sampler = sampler_state
{
    Texture = (DiffuseMap1Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
};



float4 PS(PS_input input) : COLOR
{
    //float2 uv = input.TexCoord;
    //uv = uv * 2 - 1.0f;
    //uv = mul(uv, (float2x2) rotMat).xy;
    //uv = uv * 0.5 + 0.5f;
    
    
    float3 normal = normalize(input.Normal);
    float2 perturbUV = float2(input.TexCoord.x + gTime * gUVSpeed, input.TexCoord.y);
    //perturbUV += uv;
    float3 perturb = tex2D(DiffuseMap1Sampler, perturbUV).rgb;
    float3 albedo = tex2D(DiffuseMapSampler, input.TexCoord + gBumpiness * ((perturb.xy) * 2.0f - 1.0f)).
    rgb;
    
    float3 lightDir = normalize(gWorldLightDir);
    float3 diffuse = Diffuse(lightDir, normal);
    float3 viewDir = normalize(input.ViewDir);
    float3 specular = 0;

    if (diffuse.x > 0)
    {
        specular = Specular(normal, lightDir, viewDir, gMtl_P) * gMtl_S.rgb;
    }
    diffuse = diffuse * gMtl_D.rgb;
    
    float3 ambient = gAmbientColor * gMtl_A.rgb;

    return float4((diffuse + specular + ambient) * albedo * gLightColor, 0.8f);
}

technique UVAnimation
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader = compile ps_2_0 PS();
   }

}

