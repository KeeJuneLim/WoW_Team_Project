#include "base.fx"

float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;


struct VS_INPUT 
{
	float4 Position : POSITION0;
};


struct PS_INPUT
{
	float4 Position : POSITION0;
	float4 ClipPosition : TEXCOORD1;
   
};

PS_INPUT CreateShadow_VS_main(VS_INPUT Input)
{
    PS_INPUT Output;
    
	Output.Position = mul( Input.Position, gWorldMatrix );
	Output.Position = mul( Output.Position, gLightViewMatrix);
	Output.Position = mul( Output.Position, gLightProjMatrix);
   
	Output.ClipPosition = Output.Position;
   
	return( Output );
   
}


float4 CreateShadow_PS_main(PS_INPUT Input) : COLOR0
{   
	float depth = Input.ClipPosition.z / Input.ClipPosition.w;
    
	return( float4( depth.xxx, 1.0f ) );
   
}




technique ShadowMapping
{
   pass CreateShadow
   {
      VertexShader = compile vs_2_0 CreateShadow_VS_main();
      PixelShader = compile ps_2_0 CreateShadow_PS_main();
   }

}

