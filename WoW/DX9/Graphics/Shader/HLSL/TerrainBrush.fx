
#include "Base.fx"

float3 gBrushCenter;
float gBrushRange;
float3 gBrushColor;
int gBrushShape;

texture DiffuseMapTex;
sampler2D DiffuseMapSampler = sampler_state
{
    Texture = (DiffuseMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};

texture DiffuseMap1Tex;
sampler2D DiffuseMap1Sampler = sampler_state
{
    Texture = (DiffuseMap1Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};

texture DiffuseMap2Tex;
sampler2D DiffuseMap2Sampler = sampler_state
{
    Texture = (DiffuseMap2Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};

texture DiffuseMap3Tex;
sampler2D DiffuseMap3Sampler = sampler_state
{
    Texture = (DiffuseMap3Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};

texture DiffuseMap4Tex;
sampler2D DiffuseMap4Sampler = sampler_state
{
    Texture = (DiffuseMap4Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};
texture AlphaMapTex;
sampler2D AlphaMapSampler = sampler_state
{
    Texture = (AlphaMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};

float3 ComputeBrushColor(float3 pos)
{
    if (gBrushShape == 0)
        return float3(0, 0, 0);

    if (gBrushShape == 1)
    {
        if ((pos.x >= (gBrushCenter.x - gBrushRange)) &&
            (pos.x <= (gBrushCenter.x + gBrushRange)) &&
            (pos.z >= (gBrushCenter.z - gBrushRange)) &&
            (pos.z <= (gBrushCenter.z + gBrushRange)))
        {
            return gBrushColor;
        }
    }

    if (gBrushShape == 2)
    {
        float dx = pos.x - gBrushCenter.x;
        float dz = pos.z - gBrushCenter.z;
		
        if (dx * dx + dz * dz <= gBrushRange * gBrushRange)
            return gBrushColor;
    }

    return float3(0, 0, 0);

}

struct VS_input
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float2 TexCoord1 : TEXCOORD1;
};

struct PS_input
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float2 TexCoord1 : TEXCOORD1;
    float3 Normal : TEXCOORD2;
    float3 ViewDir : TEXCOORD3;
    float4 WorldPos : TEXCOORD4;
};

PS_input VS(VS_input input)
{
    PS_input output;
    output.Position = mul(input.Position, gWorldMatrix);
    output.WorldPos = output.Position;
    output.ViewDir = normalize(output.Position.xyz - gWorldCamPos.xyz);
    
    output.Position = mul(output.Position, gViewMatrix);
    output.Position = mul(output.Position, gProjMatrix);
    
    output.Normal = normalize(mul(input.Normal, (float3x3) gWorldMatrix));
	
    output.TexCoord = input.TexCoord;
    output.TexCoord1 = input.TexCoord1;
	
    return output;
}


float4 PS(PS_input input) : COLOR
{
    float3 lightDir = normalize(gWorldLightDir);
    float3 normal = normalize(input.Normal);
    float3 diffuse = Diffuse(lightDir, normal);
    float3 viewDir = normalize(input.ViewDir);
    float3 specular = 0;

    float4 albedo;
    float4 grassColor;
    float4 slopeColor;
    float4 rockColor;
    float slope;
    float blendAmount;

    if (diffuse.x > 0)
    {
        specular = Specular(normal, lightDir, viewDir, gMtl_P) * gMtl_S.rgb;
    }
    diffuse = diffuse * gMtl_D.rgb;
    
    float3 ambient = gAmbientColor * gMtl_A.rgb;

    grassColor = tex2D(DiffuseMapSampler, input.TexCoord);
    slopeColor = tex2D(DiffuseMap1Sampler, input.TexCoord);
    rockColor = tex2D(DiffuseMap2Sampler, input.TexCoord);
	
    slope = 1.0f - normal.y;

    if (slope < 0.25f)
    {
        blendAmount = slope / 0.25f;
        albedo = lerp(grassColor, slopeColor, blendAmount);
    }
	
    if ((slope < 0.6f) && (slope >= 0.25f))
    {
        blendAmount = (slope - 0.25f) / (0.6f - 0.25f);
        albedo = lerp(slopeColor, rockColor, blendAmount);
    }

    if (slope >= 0.6f)
    {
        albedo = rockColor;
    }

    float4 alphaColor = tex2D(AlphaMapSampler, input.TexCoord1);
    float4 blendingColor = tex2D(DiffuseMap3Sampler, input.TexCoord1);
    float4 blending1Color = tex2D(DiffuseMap4Sampler, input.TexCoord1);
    
    albedo = lerp(albedo, blendingColor, alphaColor.r);
    albedo = lerp(albedo, blending1Color, alphaColor.g);
    
	float3 brushColor = ComputeBrushColor(input.WorldPos.xyz);

    return float4((diffuse + specular + ambient) * albedo.rgb * gLightColor + brushColor, 1);
}


technique tech
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }

}



