#include "base.fx"

float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;

float4 gObjectColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

texture ShadowMapTex;
sampler2D ShadowSampler = sampler_state
{
    Texture = (ShadowMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
};

struct VS_INPUT 
{
	float4 Position : POSITION0;
};


struct PS_INPUT
{
	float4 Position : POSITION0;
    float4 ClipPosition : TEXCOORD1;
};

PS_INPUT ApplyShadow_VS_main(VS_INPUT Input)
{
    PS_INPUT Output;
   
	float4 worldPos = mul( Input.Position, gWorldMatrix);
	Output.Position = mul( worldPos, gViewMatrix);
	Output.Position = mul( Output.Position, gProjMatrix);
   
	Output.ClipPosition = mul( worldPos, gLightViewMatrix);
	Output.ClipPosition = mul( Output.ClipPosition, gLightProjMatrix);
   
	return( Output );
   
}


float4 ApplyShadow_PS_main(PS_INPUT Input) : COLOR0
{   
    float4 color = gObjectColor;

    Input.ClipPosition.xyz = Input.ClipPosition.xyz / Input.ClipPosition.w;

    float currentDepth = Input.ClipPosition.z;
    float2 shadowMapUV = Input.ClipPosition.xy;
	shadowMapUV.y = -shadowMapUV.y;
	shadowMapUV = shadowMapUV * 0.5 + 0.5;
   
	float shadowDepth = tex2D(ShadowSampler, shadowMapUV).r;
   
	if ( currentDepth > shadowDepth + 0.0000125f)
	{
        color *= 0.5f;
    }
   
    return color;
   
}




//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass ApplyShadow
   {
        VertexShader = compile vs_2_0 ApplyShadow_VS_main();
      PixelShader = compile ps_2_0 ApplyShadow_PS_main();
   }

}

