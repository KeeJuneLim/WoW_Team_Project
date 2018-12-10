#pragma once

class HidingShader : public IShader
{	
private:
	LPDIRECT3DTEXTURE9 tex = NULL;
	LPDIRECT3DSURFACE9 texSurf = NULL;
	LPDIRECT3DSURFACE9 bbSurf = NULL;
public:
	HidingShader();
	~HidingShader();

	// IShader을(를) 통해 상속됨
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

