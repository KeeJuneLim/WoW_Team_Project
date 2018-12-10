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

	// IShader��(��) ���� ��ӵ�
	virtual void PreRender() override;
	virtual void Render(IDisplayObject* pObj) override;
	virtual void PostRender() override;
};

