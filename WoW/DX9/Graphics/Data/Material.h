#pragma once
#include "stdafx.h"

enum TextureType {
	TT_DiffuseMap = 0, TT_DiffuseMap1, TT_DiffuseMap2, TT_DiffuseMap3, TT_DiffuseMap4,
	TT_NormalMap, TT_SpecularMap, TT_AlphaMap, TT_Count
};


class Material : public D3DMATERIAL9
{
public:
	wstring name;
	UINT id;
	vector<IDirect3DTexture9*> textures;
	
	Material()
	{
		Diffuse = D3DXCOLOR(1, 1, 1, 1);
		Ambient = D3DXCOLOR(1, 1, 1, 1);
		Specular = D3DXCOLOR(1, 1, 1, 1);
		Emissive = D3DXCOLOR(1, 1, 1, 1);
		Power = 8.0f;
		textures.resize(TextureType::TT_Count);
	}
	
	IDirect3DTexture9* GetDiffuseMapTex(int index = 0) { return textures[TT_DiffuseMap + index]; }
	void SetDiffuseMapTex(IDirect3DTexture9* tex, int index = 0) { textures[TT_DiffuseMap + index] = tex; }
	IDirect3DTexture9* GetNormalMapTex() { return textures[TT_NormalMap]; }
	void SetNormalMapTex(IDirect3DTexture9* tex) { textures[TT_NormalMap] = tex; }
	IDirect3DTexture9* GetSpecularMapTex() { return textures[TT_SpecularMap]; }
	void SetSpecularMapTex(IDirect3DTexture9* tex) { textures[TT_SpecularMap] = tex; }
	void SetAlphaMapTex(IDirect3DTexture9* tex) { textures[TT_SpecularMap] = tex; }
	void SetTexture(int index, IDirect3DTexture9* tex) { textures[index] = tex; }
	IDirect3DTexture9* GetTexture(int index) { return textures[index]; }
	void SetMaterial(const D3DMATERIAL9 &material)
	{
		Diffuse = material.Diffuse;
		Ambient = material.Ambient;
		Specular = material.Specular;
		Emissive = material.Emissive;
		Power = material.Power;
	}
};


