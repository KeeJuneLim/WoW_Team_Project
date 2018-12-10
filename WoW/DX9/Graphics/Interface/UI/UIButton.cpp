#include "stdAfx.h"
#include "UIButton.h"
#include "UIText.h"

UIButton::UIButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite)
	: IUIObject(pSprite)
	, m_pDelegate(pDelegate)
	, m_buttonState(STATE_NORMAL)
	, m_pUIText(NULL)
{
}


UIButton::~UIButton()
{
  
}


void UIButton::Update()
{
	IUIObject::Update();
	
	RECT rect;
	GetBorderRect(&rect);

	POINT mousePoint;
	GetCursorPos(&mousePoint);
	ScreenToClient(DX::GetInfo().hWnd, &mousePoint);

	if (PtInRect(&rect, mousePoint))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_buttonState == STATE_MOUSEOVER)
			{
				m_buttonState = STATE_SELECTED;
			}
		}
		else
		{
			if (m_buttonState == STATE_SELECTED)
			{
				if (m_pDelegate)
					m_pDelegate->OnClick(this);
			}
			m_buttonState = STATE_MOUSEOVER;
		}
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{

		}
		else
		{
			m_buttonState = STATE_NORMAL;
		}
	}
}

void UIButton::Render()
{
	if (m_aTexture[m_buttonState]) 
	{
		RECT rc;
		SetRect(&rc, 0, 0, m_size.x, m_size.y);
		
		m_pSprite->Draw(m_aTexture[m_buttonState], &rc, &m_pivot, &m_combinedPos, m_color);
	}

	IUIObject::Render();
}

void UIButton::SetTexture(LPCTSTR normal, LPCTSTR mouseOver, LPCTSTR selected)
{
	D3DXIMAGE_INFO imageInfo;
	m_aTexture[STATE_NORMAL] = Assets::GetTexture(normal, &imageInfo);
	m_aTexture[STATE_MOUSEOVER] = Assets::GetTexture(mouseOver, &imageInfo);
	m_aTexture[STATE_SELECTED] = Assets::GetTexture(selected, &imageInfo);
	
	m_size.x = imageInfo.Width;
	m_size.y = imageInfo.Height;
}

void UIButton::SetText(LPD3DXFONT font, LPCTSTR text, D3DCOLOR color)
{
	if (m_pUIText == NULL)
	{
		m_pUIText = new UIText(font, m_pSprite);
		this->AddChild(m_pUIText);
		m_pUIText->SetSize(&m_size);
	}
	m_pUIText->m_pFont = font;
	m_pUIText->m_text = text;
	m_pUIText->SetColor(&color);
}
