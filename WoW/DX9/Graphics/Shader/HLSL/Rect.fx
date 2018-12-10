#include "Base.fx"

int _brushShape = 1;
float3 _brushCenter;
float _brushRange;
float3 _brushColor;

texture DiffuseMapTex;
sampler2D DiffuseMapSampler = sampler_state
{
    Texture = (DiffuseMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    ADDRESSU = WRAP;
    ADDRESSV = WRAP;
};



struct VS_INPUT 
{
    float4 Position : POSITION;
    float4 ProjPos : TEXCOORD1;
};

struct PS_INPUT
{
    float4 Position : POSITION;
    float4 ProjPos : TEXCOORD1;
};

float3 ComputeBrushColor(float3 pos)
{
    if (_brushShape == 0)
        return float3(0, 0, 0);

    if (_brushShape == 1)
    {
        if ((pos.x >= (_brushCenter.x - _brushRange)) &&
            (pos.x <= (_brushCenter.x + _brushRange)) &&
            (pos.z >= (_brushCenter.z - _brushRange)) &&
            (pos.z <= (_brushCenter.z + _brushRange)))
        {
            return _brushColor;
        }
    }

    if (_brushShape == 2)
    {
        float dx = pos.x - _brushCenter.x;
        float dz = pos.z - _brushCenter.z;
		
        if (dx * dx + dz * dz <= _brushRange * _brushRange)
            return _brushColor;
    }

    return float3(0, 0, 0);

}

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT Output;
    Output.Position.xyz = input.Position.xyz / 256.0f;
    Output.Position.w = 1.0f;
    Output.ProjPos = input.ProjPos;
	
	return Output;
}



float4 PS(PS_INPUT input) : COLOR
{   
    float3 center = _brushCenter / 257.0f * 2.0f - 1.0f;
    float4 color = 0;
    input.ProjPos.xyz = input.ProjPos.xyz / input.ProjPos.w;
    if (abs(center.x - input.ProjPos.x) > 0.05f)
        color = float4(1, 0, 0, 1);
    else 
        color = float4(0, 1, 0, 1);
    color.b = center.x;
    return color;

}

//--------------------------------------------------------------//
// Technique Section for Lighting
//--------------------------------------------------------------//
technique Lighting
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader = compile ps_2_0 PS();
   }

}

