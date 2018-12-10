
#include "base.fx"

float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;

struct VS_Input 
{
	float4 Position : POSITION;
};

struct PS_Input
{
	float4 Position : POSITION;
	float4 LightProjPos : TEXCOORD1;
};

PS_Input CreateShadow_VS_main(VS_Input input)
{
    PS_Input output;
    
	output.Position = mul(input.Position, gWorldMatrix);
    output.Position = mul(output.Position, gLightViewMatrix);
    output.Position = mul(output.Position, gLightProjMatrix);
   
    output.LightProjPos = output.Position;
   
    return output;
   
}

float4 CreateShadow_PS_main(PS_Input input) : COLOR0
{   
    float depth = input.LightProjPos.z / input.LightProjPos.w;
    //float depth = input.LightProjPos.z / 1000.0f;
    
	return( float4( depth.xxx, 1.0f ) );
   
}



technique ShadowMapping
{
    pass Pass_0
    {
      VertexShader = compile vs_2_0 CreateShadow_VS_main();
      PixelShader = compile ps_2_0 CreateShadow_PS_main();
   }
}

