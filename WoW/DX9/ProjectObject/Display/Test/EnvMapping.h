#pragma once
#include "Graphics/Interface/UI/IUIObject.h"

class Grid;
class SphereParticle;

class EnvMapping : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;

	const UINT TEX_SIZE = 256;
	LPDIRECT3DCUBETEXTURE9 g_pCubeTex;
	IDirect3DSurface9* g_pCubeDepth;

	LPD3DXSPRITE g_pSprite;
	UIImage* g_pImageUI[6];
	UIText* g_pTextUI[6];
	LPDIRECT3DTEXTURE9 g_pImageUITex[6];

	Grid *grid;
	SphereParticle *particle;
	
public:
	EnvMapping();
	~EnvMapping();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Draw() override {};

	void RenderCubeMap();
	void RenderShader();
	void RenderUI();
};