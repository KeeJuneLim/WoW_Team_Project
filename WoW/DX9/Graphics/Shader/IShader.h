#pragma once
#include "stdafx.h"

class IDisplayObject;
class TerrainEditor;
class Boundary;

enum MatrixType
{
	MT_World, MT_View, MT_Proj, MT_WVP
};

class IShader
{
protected:
	VARIABLE(LPD3DXEFFECT, effect, Effect);
	VARIABLE(UINT, renderQueue, RenderQueue);
public:
	virtual ~IShader() {};
	
	virtual void PreRender() = 0;
	virtual void Render(IDisplayObject* pObj) = 0;
	virtual void Render(Boundary* pObj) {}
	virtual void PostRender() = 0;
	virtual void SetMatrix(MatrixType type, D3DXMATRIX* matrix);
	virtual void SetMaterial(Material* material);
	virtual void SetTexture(TextureType index, IDirect3DTexture9* pTexture);
	virtual void Commit() { effect->CommitChanges(); }
	virtual void SetTerrainEditor(TerrainEditor* pEditor);
protected:
	const D3DXHANDLE hWorldMatrix = "gWorldMatrix";
	const D3DXHANDLE hViewMatrix = "gViewMatrix";
	const D3DXHANDLE hProjMatrix = "gProjMatrix";
	const D3DXHANDLE hVPMatrix = "gVPMatrix";
	const D3DXHANDLE hWVPMatrix = "gWVPMatrix";

	const D3DXHANDLE hWorldLightPos = "gWorldLightPos";
	const D3DXHANDLE hWorldLightDir = "gWorldLightDir";
	const D3DXHANDLE hLightColor = "gLightColor";
	const D3DXHANDLE hWorldCamPos = "gWorldCamPos";
	
	const D3DXHANDLE hLightViewMatrix = "gLightViewMatrix";
	const D3DXHANDLE hLightProjMatrix = "gLightProjMatrix";

	const D3DXHANDLE hMtl_D = "gMtl_D";
	const D3DXHANDLE hMtl_A = "gMtl_A";
	const D3DXHANDLE hMtl_S = "gMtl_S";
	const D3DXHANDLE hMtl_P = "gMtl_P";

	const D3DXHANDLE hDiffuseMapTex = "DiffuseMapTex";
	const D3DXHANDLE hDiffuseMap1Tex = "DiffuseMap1Tex";
	const D3DXHANDLE hDiffuseMap2Tex = "DiffuseMap2Tex";
	const D3DXHANDLE hDiffuseMap3Tex = "DiffuseMap3Tex";
	const D3DXHANDLE hDiffuseMap4Tex = "DiffuseMap4Tex";
	const D3DXHANDLE hSpecularMapTex = "SpecularMapTex";
	const D3DXHANDLE hNormalMapTex = "NormalMapTex";
	const D3DXHANDLE hAlphaMapTex = "AlphaMapTex";
	

};
