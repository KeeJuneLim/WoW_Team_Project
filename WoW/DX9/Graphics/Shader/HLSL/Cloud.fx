
#include "base.fx"

float gTranslation = 0.0f;
float gPerturbScale = 0.3f;

texture DiffuseMapTex;
sampler2D DiffuseMapSampler = sampler_state
{
    Texture = (DiffuseMapTex);
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MIPFILTER = LINEAR;
};
texture DiffuseMap1Tex;
sampler2D DiffuseMap1Sampler = sampler_state
{
    Texture = (DiffuseMap1Tex);
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MIPFILTER = LINEAR;
};

struct VS_INPUT 
{
   float4 Position : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
   float4 Position : POSITION;
   float2 TexCoord : TEXCOORD0;
};



PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
   
    Output.Position = mul(Input.Position, gWorldMatrix);
    Output.Position = mul(Output.Position, gViewMatrix);
    Output.Position = mul(Output.Position, gProjMatrix);
   
    Output.TexCoord = Input.TexCoord;
    return Output;
}

float4 PS(PS_INPUT input) : COLOR
{
    input.TexCoord.x = input.TexCoord.x + gTranslation;
    float2 perturb = tex2D(DiffuseMap1Sampler, input.TexCoord).xy * gPerturbScale;
    float4 cloudColor = tex2D(DiffuseMapSampler, input.TexCoord + gTranslation + perturb);
	cloudColor = cloudColor * 0.8f;

    return cloudColor;
}


technique tech
{
	pass Pass_0
	{
		
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
	
}

