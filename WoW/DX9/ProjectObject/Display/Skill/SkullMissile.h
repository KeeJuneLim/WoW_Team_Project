#pragma once


class SkullMissile : public IDisplayObject
{
public:
	SkullMissile();
	SkullMissile(D3DXVECTOR3 pos, D3DXVECTOR3 dir);

	~SkullMissile();
	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	void Render(IShader* pShader);
	virtual void Draw() override;


private:

	LPD3DXMESH	 skull;

	LPD3DXEFFECT m_pCreateShadow;
	LPD3DXEFFECT m_pApplyShadow;
	LPDIRECT3DTEXTURE9 m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;


	D3DXVECTOR3 skillDir;
};