
#include "Base.fx"


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
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
	output.Position = mul(input.Position, gWorldMatrix);
	
    output.ViewDir = normalize(output.Position.xyz - gWorldCamPos.xyz);
    
	output.Position = mul(output.Position, gViewMatrix);
	output.Position = mul(output.Position, gProjMatrix);
    
    output.Normal = normalize(mul(input.Normal, (float3x3) gWorldMatrix));
	
	output.TexCoord = input.TexCoord;

	return output;
}



float4 PS(PS_INPUT input) : COLOR
{   
    float3 lightDir = normalize(gWorldLightDir);
    float3 normal = normalize(input.Normal);
    float3 diffuse = Diffuse(lightDir, normal);
    float3 viewDir = normalize(input.ViewDir);
    float3 specular = 0;

    if (diffuse.x > 0)
    {
        specular = Specular(normal, lightDir, viewDir, gMtl_P) * gMtl_S.rgb;
    }
    diffuse = diffuse * gMtl_D.rgb;
    
    float3 ambient = gAmbientColor * gMtl_A.rgb;
    float3 albedo = tex2D(DiffuseMapSampler, input.TexCoord).rgb;
   
    float4 finalColor = float4((diffuse + specular + ambient) * albedo * gLightColor, 1);

	return finalColor;
}


technique Lighting
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader = compile ps_2_0 PS();
   }

}

