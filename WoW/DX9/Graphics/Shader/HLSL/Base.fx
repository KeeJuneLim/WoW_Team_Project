

float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjMatrix;

float3 gWorldLightDir = float3(1.0f, -1.0f, 1.0f);
float4 gWorldCamPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
float3 gLightColor = float3(1.0f, 1.0f, 1.0f);
float3 gAmbientColor = float3(0.3f, 0.3f, 0.3f);

float4 gMtl_D = float4(1.0, 1.0, 1.0, 1.0);
float4 gMtl_A = float4(1.0, 1.0, 1.0, 1.0);
float4 gMtl_S = float4(1.0, 1.0, 1.0, 1.0);
float gMtl_P = float(8.0f);

float3 Diffuse(float3 normal, float3 lightDir)
{
    return saturate(dot(normal, -lightDir));
}

float3 Specular(float3 normal, float3 lightDir, float3 viewDir, float power)
{
	float3 reflection = reflect(lightDir, normal);
	float3 specular = saturate(dot(reflection, -viewDir));
    specular = pow(specular, power);
    
	return specular;
}