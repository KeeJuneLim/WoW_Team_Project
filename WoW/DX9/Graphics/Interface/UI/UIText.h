#pragma once
#include "IUIObject.h"

class UIText : public IUIObject
{
public:
	LPD3DXFONT	m_pFont;
	LPCTSTR		m_text;
	DWORD		m_drawTextFormat;
	
	UIText(LPD3DXFONT pFont, LPD3DXSPRITE pSprite);
	~UIText();

	void Render() override;
};

