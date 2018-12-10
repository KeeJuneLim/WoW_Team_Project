#include "base.fx"

float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;

texture ShadowMapTex;
sampler2D ShadowMapSampler = sampler_state
{
    Texture = (ShadowMapTex);
	MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
	
};

texture DiffuseMapTex;
sampler2D DiffuseMapSampler = sampler_state
{
    Texture = (DiffuseMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
};

struct VS_INPUT 
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
    float4 LightProjPos : TEXCOORD3;
};


PS_INPUT ApplyShadow_VS_main(VS_INPUT input)
{
    PS_INPUT output;
   
	float4 worldPos = mul( input.Position, gWorldMatrix);
	output.Position = mul( worldPos, gViewMatrix);
	output.Position = mul( output.Position, gProjMatrix);
   
    output.ViewDir = normalize(worldPos.xyz - gWorldCamPos.xyz);

	output.LightProjPos = mul( worldPos, gLightViewMatrix);
	output.LightProjPos = mul( output.LightProjPos, gLightProjMatrix);
   
    output.Normal = normalize(mul(input.Normal, (float3x3) gWorldMatrix));

    output.TexCoord = input.TexCoord;

	return output;
   
}



float4 ApplyShadow_PS_main(PS_INPUT input) : COLOR0
{   
    float3 lightDir = normalize(gWorldLightDir);
    float3 normal = normalize(input.Normal);
    float3 diffuse = Diffuse(lightDir, normal);
    float3 viewDir = normalize(input.ViewDir);
    float3 specular = 0;

    if (diffuse.x > 0)
    {
        specular = Specular(normal, lightDir, viewDir, gMtl_P) * gMtl_S.rgb;
    }
    diffuse = diffuse * gMtl_D.rgb;
    
    float3 ambient = gAmbientColor * gMtl_A.rgb;
    float3 albedo = tex2D(DiffuseMapSampler, input.TexCoord).rgb;
   
    
    //input.LightProjPos.xyz = input.LightProjPos.xyz / input.LightProjPos.w;

    float currentDepth = input.LightProjPos.z / input.LightProjPos.w;
    float2 shadowMapUV = input.LightProjPos.xy / input.LightProjPos.w;
	shadowMapUV.y = -shadowMapUV.y;
	shadowMapUV = shadowMapUV * 0.5f + 0.5f;
   
	float shadowDepth = tex2D(ShadowMapSampler, shadowMapUV).r;
   
	if ( currentDepth > shadowDepth + 0.0000125f)
	{
		diffuse *= 0.5f;
    }
	
    return float4((diffuse + specular + ambient) * albedo * gLightColor, 1);
   
}



technique ShadowMapping
{
    pass ApplyShadow
    {
        VertexShader = compile vs_2_0 ApplyShadow_VS_main();
        PixelShader = compile ps_2_0 ApplyShadow_PS_main();
    }
}

