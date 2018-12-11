#pragma once
#include "Graphics/Interface/UI/IUIObject.h"

class PlayerUI : public IDisplayObject, public IUIButtonDelegate
{
private:
	LPD3DXSPRITE m_pSprite;
	UIImage* m_pMenu;
	UIImage* m_pMenuBG;

	bool m_enableMenu;
public:

	PlayerUI();
	~PlayerUI();
	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	void Draw() {};
	// IUIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;

	UIButton* CreateButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite,
		IDisplayObject* pParent, wstring name, D3DXVECTOR3 &pos,
		wstring normal, wstring over, wstring select);

};
