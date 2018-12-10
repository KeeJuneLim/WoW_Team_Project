#include "stdAfx.h"
#include "UIText.h"

UIText::UIText(LPD3DXFONT pFont, LPD3DXSPRITE pSprite)
	: IUIObject(pSprite)
	, m_pFont(pFont)
	, m_drawTextFormat(DT_CENTER | DT_VCENTER)
{
}

UIText::~UIText()
{
}

void UIText::Render()
{
	RECT rect;
	SetRect(&rect, m_combinedPos.x, m_combinedPos.y, 
		m_combinedPos.x + m_size.x, m_combinedPos.y + m_size.y);
	
	m_pFont->DrawText(m_pSprite, m_text, lstrlen(m_text), &rect, m_drawTextFormat, m_color);
	
	IUIObject::Render();
}
