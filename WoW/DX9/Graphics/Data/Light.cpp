#include "stdafx.h"
#include "Light.h"

D3DXVECTOR3 Light::WorldLightDir = D3DXVECTOR3(1, -1, 1);
D3DXVECTOR3 Light::LightColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
D3DXVECTOR3 Light::CenterColor = D3DXVECTOR3(0.3f, 0.3f, 1.0f);
D3DXVECTOR3 Light::ApexColor = D3DXVECTOR3(0.1f, 0.1f, 0.6f);
D3DXVECTOR3 Light::WorldLightPos = D3DXVECTOR3(0, 0, 0);
float Light::LightDistance = 55.0f;

void Light::Update()
{
	D3DXVec3Normalize(&WorldLightPos, &Light::WorldLightDir);
	WorldLightPos = -WorldLightPos * LightDistance;

	Lerp(ApexColor.x, 0.8f, 0.1f, max(0, -WorldLightDir.y));
	Lerp(ApexColor.y, 0.6f, 0.1f, max(0, -WorldLightDir.y));
	Lerp(ApexColor.z, 0.2f, 0.6f, max(0, -WorldLightDir.y));

	Lerp(CenterColor.x, 0.6f, 0.3f, max(0, -WorldLightDir.y));
	Lerp(CenterColor.y, 0.4f, 0.3f, max(0, -WorldLightDir.y));
	Lerp(CenterColor.z, 0.4f, 1.0f, max(0, -WorldLightDir.y));

	Lerp(LightColor.x, 0.8f, 1.0f, max(0, -WorldLightDir.y));
	Lerp(LightColor.y, 0.8f, 1.0f, max(0, -WorldLightDir.y));
	Lerp(LightColor.z, 0.4f, 1.0f, max(0, -WorldLightDir.y));

	if (GUI::Get()->showLight)
	{
		ImGui::Begin(GUI::LightStr);
		ImGui::SliderFloat3("Direction", (float*)&Light::WorldLightDir, -1.0f, 1.0f);
		ImGui::SliderFloat("Distance", (float*)&Light::LightDistance, 0.0f, 100.0f);
		ImGui::SliderFloat3("Color", (float*)&Light::LightColor, 0.0f, 1.0f);
		ImGui::SliderFloat3("ApexColor", (float*)&Light::ApexColor, 0.0f, 1.0f);
		ImGui::SliderFloat3("CenterColor", (float*)&Light::CenterColor, 0.0f, 1.0f);
		ImGui::End();
	}
	
}