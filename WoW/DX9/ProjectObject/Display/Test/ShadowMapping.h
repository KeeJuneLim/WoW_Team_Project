#pragma once
#include "stdafx.h"

class ShadowMapping : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pTorus;
	LPD3DXMESH	 m_pDisc;
	LPD3DXEFFECT m_pCreateShadow;
	LPD3DXEFFECT m_pApplyShadow;
	LPDIRECT3DTEXTURE9 m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;
	

public:
	ShadowMapping();
	~ShadowMapping();

	void Init();
	void Update();
	void Render();
	void Draw() {};
};

