#include "stdafx.h"
#include "PlayerUI.h"
#include "Graphics/Interface/UI/IUIObject.h"

static const wstring BTN_NAME_CLOSE = L"X";
static const wstring BTN_NAME_GRID = L"Grid";
static const wstring BTN_NAME_OBJ = L"OBJ";
static const wstring BTN_NAME_TERRAIN = L"Terrain";
static const wstring BTN_NAME_TEST = L"Test";
static const wstring BTN_NAME_SPACE = L"Space";

PlayerUI::PlayerUI()
{
	m_enableMenu = true;
}
PlayerUI::~PlayerUI()
{
	SAFE_RELEASE(m_pSprite);
	SAFE_DELETE(m_pMenu);
	SAFE_DELETE(m_pMenuBG);
}

void PlayerUI::Init()
{
	D3DXCreateSprite(DX::GetDevice(), &m_pSprite);

	UIImage* pImage = NULL;
	UIButton* pButton = NULL;
	UIText* pText = NULL;
	D3DCOLOR textColor = 0xff338888;
	
	RECT rect;
	GetClientRect(DX::GetInfo().hWnd, &rect);

	//menu bg
	pImage = new UIImage(m_pSprite);
	pImage->SetTexture((ASSET_PATH + L"Textures/ui/PlayerMainBG_Ui.png").c_str());
	//啊款单
	pImage->SetPosition(&D3DXVECTOR3(
		(rect.right - (*pImage->GetSize()).x ) / 2,
		(rect.bottom - (*pImage->GetSize()).y) , 0));
	////哭率
	//pImage->SetPosition(&D3DXVECTOR3(
	//	(rect.right * 0.05f),
	//	(rect.bottom - (*pImage->GetSize()).y) / 2, 0));
	////坷弗率
	//pImage->SetPosition(&D3DXVECTOR3(
	//	(rect.right - (*pImage->GetSize()).x) - rect.right * 0.1f,
	//	(rect.bottom - (*pImage->GetSize()).y) / 2, 0));

	m_pMenuBG = pImage;
	
	//menu
	pImage = new UIImage(m_pSprite);
	pImage->SetTexture((ASSET_PATH + L"Textures/ui/PlayerMain_Ui.png").c_str());
	//啊款单
	pImage->SetPosition(&D3DXVECTOR3(
		(rect.right - (*pImage->GetSize()).x) / 2,
		(rect.bottom - (*pImage->GetSize()).y) , 0));

	m_pMenu = pImage;
	
	pText = new UIText(Assets::GetFont(Assets::FT_NORMAL), m_pSprite);
	m_pMenu->AddChild(pText);
	//pText->m_text = _T("SCENE");
	pText->SetSize(&D3DXVECTOR2(200, 60));
	pText->SetPosition(&D3DXVECTOR3(((*m_pMenu->GetSize()).x - (*pText->GetSize()).x) / 2, 30, 0));
	textColor = 0xff88dddd;
	pText->SetColor(&textColor);

	float btnY = 83;
	float btnOffsetY = 0;
	float btnLeft = 844;
	float btnoffsetX = 71;

	textColor = 0xff338888;

	pButton = CreateButton(this, m_pSprite, m_pMenuBG, BTN_NAME_GRID,
		D3DXVECTOR3(btnLeft, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/NULL.png",
		ASSET_PATH + L"Textures/ui/NULL.png",
		ASSET_PATH + L"Textures/ui/NULL.png");

	pButton = CreateButton(this, m_pSprite, m_pMenuBG, BTN_NAME_GRID,
		D3DXVECTOR3(btnLeft += btnoffsetX, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/NULL.png",
		ASSET_PATH + L"Textures/ui/NULL.png",
		ASSET_PATH + L"Textures/ui/NULL.png");

	pButton = CreateButton(this, m_pSprite, m_pMenuBG, BTN_NAME_GRID,
		D3DXVECTOR3(btnLeft += btnoffsetX, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/NULL.png",
		ASSET_PATH + L"Textures/ui/NULL.png",
		ASSET_PATH + L"Textures/ui/NULL.png");
	//pButton->SetText(Assets::GetFont(Assets::FT_NORMAL), BTN_NAME_GRID.c_str(), textColor);
}

void PlayerUI::Update()
{
	if (m_enableMenu) { SAFE_UPDATE(m_pMenuBG);SAFE_UPDATE(m_pMenu);}
}

void PlayerUI::Render()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	float scale = 1;
	
	if (m_enableMenu)
	{
		D3DXMatrixIdentity(&worldMatrix);
		m_pSprite->SetTransform(&worldMatrix);
		if (m_enableMenu) { SAFE_RENDER(m_pMenuBG); SAFE_RENDER(m_pMenu);  }
	}
	
	D3DXMatrixIdentity(&worldMatrix);
	m_pSprite->SetTransform(&worldMatrix);

	m_pSprite->End();
}

void PlayerUI::OnClick(UIButton * pSender)
{


//	else if (pSender->GetName() == BTN_NAME_TEST)
//	{
//		m_enableMenu = false;
//		Scenes::Get()->SetCurrentScene(SCENE_TEST);
//	}
	
}

UIButton * PlayerUI::CreateButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite,
	IDisplayObject * pParent, wstring name, D3DXVECTOR3 &pos,
	wstring normal, wstring over, wstring select)
{
	UIButton* pButton = new UIButton(pDelegate, pSprite);
	if(pParent) pParent->AddChild(pButton);
	pButton->SetName(name);
	pButton->SetTexture(
		normal.c_str(),
		over.c_str(),
		select.c_str());
	pButton->SetPosition(&pos);
	
	return pButton;
}

