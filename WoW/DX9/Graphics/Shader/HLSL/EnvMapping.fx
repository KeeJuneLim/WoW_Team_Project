
#include "base.fx"

float4x4 gWVPMatrix;

struct VS_INPUT 
{
	float4 mPosition : POSITION;
	float2 mUV : TEXCOORD0;
	float3 mNormal : NORMAL;
	float3 mBinormal : BINORMAL;
	float3 mTangent : TANGENT;
};

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float2 mUV: TEXCOORD0;
	float3 mViewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

PS_INPUT VS(VS_INPUT Input)
{
	PS_INPUT Output;
	Output.mPosition = mul(Input.mPosition, gWVPMatrix);

	Output.mUV = Input.mUV;
   
	float4 worldPos = mul(Input.mPosition, gWorldMatrix);

    Output.mViewDir = normalize(worldPos.xyz - gWorldCamPos.xyz);
    
    // T B N on World Space
    Output.T = normalize(mul(Input.mTangent, (float3x3) gWorldMatrix));
    Output.B = normalize(mul(Input.mBinormal, (float3x3) gWorldMatrix));
    Output.N = normalize(mul(Input.mNormal, (float3x3) gWorldMatrix));
   
	return Output;
}

texture DiffuseMapTex;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMapTex);
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture SpecularMapTex;
sampler2D SpecularSampler = sampler_state
{
   Texture = (SpecularMapTex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
texture NormalMapTex;
sampler2D NormalSampler = sampler_state
{
   Texture = (NormalMapTex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
texture EnvMapTex;
samplerCUBE EnvSampler = sampler_state
{
   Texture = (EnvMapTex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};


float4 PS(PS_INPUT Input) : COLOR
{   
	float3 tangentNormal = tex2D(NormalSampler, Input.mUV).xyz;
	tangentNormal.z += 6.0f;
	tangentNormal = normalize(tangentNormal * 2 - 1);
	tangentNormal = float3(0, 0, 1);
   
	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
	//TBN = transpose(TBN);
   
	//float3 worldNormal = mul(TBN, tangentNormal);
	float3 worldNormal = mul(tangentNormal, TBN);
	
    float3 lightDir = normalize(gWorldLightDir);
    float3 diffuse = Diffuse(worldNormal, lightDir);
	float3 specular = 0;
	float3 viewDir = normalize(Input.mViewDir);
   
	if ( diffuse.x > 0 )
	{
        specular = Specular(worldNormal, lightDir, viewDir, gMtl_P) * gMtl_S.rgb;
        
		float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
		specular *= specularIntensity.rgb * gLightColor;
	}
   
	float3 ambient = gAmbientColor * gMtl_A.rgb;
	float3 albedo = tex2D(DiffuseSampler, Input.mUV).rgb;

	float3 viewReflect = reflect(viewDir, worldNormal);
	float3 env = texCUBE(EnvSampler, viewReflect).rgb;

    float4 finalColor = float4((diffuse + specular + ambient) * albedo * gLightColor + env, 1);
    //float4 finalColor = float4(env, 1);

    return finalColor;
}


technique Tech
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }

}

