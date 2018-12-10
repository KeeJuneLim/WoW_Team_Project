#pragma once

class Light
{
public:
	static D3DXVECTOR3 WorldLightDir;
	static D3DXVECTOR3 LightColor;
	static D3DXVECTOR3 CenterColor;
	static D3DXVECTOR3 ApexColor;
	static D3DXVECTOR3 WorldLightPos;
	static float LightDistance;

	static void Update();
};


