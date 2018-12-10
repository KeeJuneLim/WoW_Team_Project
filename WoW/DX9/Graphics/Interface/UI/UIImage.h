#pragma once
#include "IUIObject.h"

class UIImage : public IUIObject
{
public:
	LPDIRECT3DTEXTURE9 m_pTex;
	
	UIImage(LPD3DXSPRITE pSprite);
	~UIImage();

	void Render() override;
	void SetTexture(LPCTSTR filename);
	void SetTexture(LPDIRECT3DTEXTURE9 pTex, UINT width, UINT height = 0);
};

