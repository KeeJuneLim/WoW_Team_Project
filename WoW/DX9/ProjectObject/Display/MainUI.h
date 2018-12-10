#pragma once
#include "Graphics/Interface/UI/IUIObject.h"

class MainUI : public IDisplayObject, public IUIButtonDelegate
{
private:
	LPD3DXSPRITE m_pSprite;
	UIButton * m_pCloseBtn;
	UIImage* m_pMenu;
	UIImage* m_pBG;

	bool m_enableMenu;
public:

	MainUI();
	~MainUI();
	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	void Draw() {};
	// IUIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;

	UIButton* CreateButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite,
		IDisplayObject* pParent, wstring name, D3DXVECTOR3 &pos,
		wstring normal, wstring over, wstring select);

};
