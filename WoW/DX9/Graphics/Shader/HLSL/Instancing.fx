
#include "Base.fx"

struct VS_INPUT
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
    row_major matrix InstanceWorld : TEXCOORD1;
    float3 InstanceColor : TEXCOORD5;
};

struct PS_INPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 InstanceColor : TEXCOORD5;
};

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


PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    output.Position = mul(input.Position, input.InstanceWorld);
    output.Position = mul(output.Position, gViewMatrix);
    output.Position = mul(output.Position, gProjMatrix);
    
    output.TexCoord = input.TexCoord;
    
    output.InstanceColor = input.InstanceColor;

    return output;
}

float4 PS(PS_INPUT input) : COLOR0
{
    float4 textureColor;
    float4 color;

    textureColor = tex2D(DiffuseMapSampler, input.TexCoord);
    color = textureColor * float4(input.InstanceColor, 1.0f);
	
    color = saturate(color);

    return color;
}


technique THW_Instancing
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}


