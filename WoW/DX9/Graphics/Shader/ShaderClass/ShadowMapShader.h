#pragma once

class ShadowMapShader : public IShader
{	
private:
	//LPDIRECT3DTEXTURE9 m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;

	LPDIRECT3DSURFACE9 pHWRenderTarget = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencil = NULL;

public:
	ShadowMapShader();
	~ShadowMapShader();

	// IShader을(를) 통해 상속됨
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

