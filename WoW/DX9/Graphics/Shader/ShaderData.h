#pragma once

struct HidingData;
struct ShadowData;

class ShaderData
{
public:
	static HidingData hiding;
	static ShadowData shadow;
};

struct HidingData
{
	bool onChange;
	float hidingFactor;
	float hidingBumpness;
};

struct ShadowData
{
	LPDIRECT3DTEXTURE9 shadowMap;
};