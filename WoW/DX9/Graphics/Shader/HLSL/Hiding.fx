
#include "Base.fx"

float gOnChange = false;
float gHidingFactor = 1.0f;
float gHidingBumpness = 0.6f;

texture NoiseMapTex;
sampler2D NoiseMapSampler = sampler_state
{
    Texture = (NoiseMapTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = CLAMP;
    //ADDRESSV = WRAP;
};

texture BackbufferTex;
sampler2D BackbufferSampler = sampler_state
{
    Texture = (BackbufferTex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    ADDRESSU = CLAMP;
    ADDRESSV = CLAMP;
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

struct VS_input 
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PS_input
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
    float4 ProjPosition : TEXCOORD3;
};

PS_input VS(VS_input input)
{
    PS_input output;
    
	output.Position = mul(input.Position, gWorldMatrix);

	float3 viewDir = normalize(output.Position.xyz - gWorldCamPos.xyz);
	output.ViewDir = viewDir;
    
    output.Position = mul(output.Position, gViewMatrix);
	output.Position = mul(output.Position, gProjMatrix);
    output.ProjPosition = output.Position;
    
	float3 worldNormal = mul(input.Normal, (float3x3) gWorldMatrix);
    worldNormal = normalize(worldNormal);
    output.Normal = worldNormal;
	
	output.TexCoord = input.TexCoord;

	return output;
}



float4 PS(PS_input input) : COLOR
{   
    float3 origColor = float3(1, 1, 1);
    float3 transColor;
    float3 albedo;

    float3 noiseMap = tex2D(NoiseMapSampler, input.TexCoord);
    noiseMap = noiseMap * 2.0f - 1.0f;

    if (gOnChange)
    {
        float3 lightDir = normalize(gWorldLightDir);
        float3 normal = normalize(input.Normal);
        float3 diffuse = Diffuse(lightDir, normal);
        float3 viewDir = normalize(input.ViewDir);
        float3 specular = 0;
        float3 reflection = 0;

        if (diffuse.x > 0)
        {
            specular = Specular(normal, lightDir, viewDir, gMtl_P) * gMtl_S.rgb;
        }

        diffuse = diffuse * gMtl_D.rgb;
    
        float3 ambient = gAmbientColor * gMtl_A.rgb;

        albedo = tex2D(DiffuseMapSampler,
		input.TexCoord + (noiseMap.xy) * gHidingBumpness * gHidingFactor).rgb;

        origColor = (diffuse + specular + ambient) * albedo * gLightColor;
        
    }
    
    float2 tex = (input.ProjPosition.xy) / input.ProjPosition.w;
    tex.y = -tex.y;
    tex = tex + noiseMap.xy * gHidingBumpness;
    tex = tex * 0.5f + 0.5f;
    
    transColor = tex2D(BackbufferSampler, tex).rgb;

    float4 finalColor = float4(lerp(origColor, transColor, gHidingFactor), 1);
    
    return finalColor;
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

