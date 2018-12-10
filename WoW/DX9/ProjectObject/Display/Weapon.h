#pragma once


class Weapon : IDisplayObject
{
private:
	LPD3DXMESH	 axe;

	LPD3DXEFFECT m_pCreateShadow;
	LPD3DXEFFECT m_pApplyShadow;
	LPDIRECT3DTEXTURE9 m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;


public:
	Weapon();
	~Weapon();

	void Init();
	void Update();
	void Update(IDisplayObject* frame, string name);
	void Render();
	void Draw() {};

	void SetWorldMatrix(D3DXMATRIX combinedTM);
};