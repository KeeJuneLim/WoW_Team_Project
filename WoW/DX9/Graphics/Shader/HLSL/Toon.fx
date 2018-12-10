#include "Base.fx"

float4x4 gWVPMatrix;

float4 gSurfaceColor = float4(0.5f, 1.00f, 0.5f, 1.0f);
float4 gOutlineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
float gThickness = 0.015f;


struct VS_INPUT 
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
};


PS_INPUT VS_OUTLINE(VS_INPUT input)
{
    PS_INPUT output;
    
    output.Position = float4(input.Position.xyz + (normalize(input.Normal) * gThickness), 1);
    output.Position = mul(output.Position, gWVPMatrix);
    
    output.Normal = 0;
    output.TexCoord = 0;

    return output;
}

float4 PS_OUTLINE() : COLOR
{
    return gOutlineColor;
}

texture DiffuseMapTex;
sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMapTex);
};


PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
   
    output.Position = mul(input.Position, gWVPMatrix);
   
    output.Normal = normalize(mul(normalize(input.Normal), (float3x3) gWorldMatrix));
    
    output.TexCoord = input.TexCoord;

    return output;
}

float4 PS(PS_INPUT input) : COLOR
{
    float3 diffuse = Diffuse(normalize(input.Normal), gWorldLightDir);
    diffuse = ceil(diffuse * 5) / 5.0f;
   
    float3 albedo = tex2D(DiffuseSampler, input.TexCoord).rgb;
    
    float3 color = (diffuse.rgb + gAmbientColor) * albedo.rgb * gLightColor;
    
	return float4(color, 1.0f);
}




//--------------------------------------------------------------//
// Technique Section for ToonShader
//--------------------------------------------------------------//
technique ToonShader
{
    pass Pass_0
    {
        CULLMODE = CW;
        VertexShader = compile vs_2_0 VS_OUTLINE();
        PixelShader = compile ps_2_0 PS_OUTLINE();
    }
    pass Pass_1
    {
        CULLMODE = CCW;
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }

}

