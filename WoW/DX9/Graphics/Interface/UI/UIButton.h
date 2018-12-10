#pragma once
#include "IUIObject.h"

class UIButton;
class UIText;

class IUIButtonDelegate
{
public:
	virtual void OnClick(UIButton* pSender) = 0;
};


class UIButton : public IUIObject
{
private:
	enum ButtonState
	{
		STATE_NORMAL,
		STATE_MOUSEOVER,
		STATE_SELECTED,
		STATE_COUNT
	};
	ButtonState	m_buttonState;
	
public:
	IUIButtonDelegate * m_pDelegate;
	LPDIRECT3DTEXTURE9	m_aTexture[STATE_COUNT];
	UIText*				m_pUIText;

	UIButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite);
	virtual ~UIButton();

	void Update() override;
	void Render() override;

	void SetTexture(LPCTSTR normal, LPCTSTR mouseOver, LPCTSTR selected);
	void SetText(LPD3DXFONT font, LPCTSTR text, D3DCOLOR color);
};

