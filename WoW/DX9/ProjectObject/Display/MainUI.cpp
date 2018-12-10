#include "stdafx.h"
#include "MainUI.h"
#include "Graphics/Interface/UI/IUIObject.h"

static const wstring BTN_NAME_CLOSE = L"X";
static const wstring BTN_NAME_GRID = L"Grid";
static const wstring BTN_NAME_OBJ = L"OBJ";
static const wstring BTN_NAME_TERRAIN = L"Terrain";
static const wstring BTN_NAME_TEST = L"Test";
static const wstring BTN_NAME_SPACE = L"Space";

MainUI::MainUI()
{
	m_enableMenu = true;
}
MainUI::~MainUI()
{
	SAFE_RELEASE(m_pSprite);
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pMenu);
	SAFE_DELETE(m_pCloseBtn);
}

void MainUI::Init()
{
	D3DXCreateSprite(DX::GetDevice(), &m_pSprite);

	UIImage* pImage = NULL;
	UIButton* pButton = NULL;
	UIText* pText = NULL;
	D3DCOLOR textColor = 0xff338888;
	
	RECT rect;
	GetClientRect(DX::GetInfo().hWnd, &rect);

	pImage = new UIImage(m_pSprite);
	pImage->SetTexture((ASSET_PATH + L"Textures/ui/main_bg.png").c_str());
	m_pBG = pImage;

	//close btn
	pButton = CreateButton(this, m_pSprite, NULL, BTN_NAME_CLOSE,
		D3DXVECTOR3(rect.right - 170, rect.top + 10, 0),
		ASSET_PATH + L"Textures/ui/main_btn_normal.png",
		ASSET_PATH + L"Textures/ui/main_btn_over.png",
		ASSET_PATH + L"Textures/ui/main_btn_select.png");
	pButton->SetText(Assets::GetFont(Assets::FT_NORMAL), BTN_NAME_CLOSE.c_str(), textColor);
	m_pCloseBtn = pButton;
	
	//menu bg
	pImage = new UIImage(m_pSprite);
	pImage->SetTexture((ASSET_PATH + L"Textures/ui/main_panel.png").c_str());
	//啊款单
	pImage->SetPosition(&D3DXVECTOR3(
		(rect.right - (*pImage->GetSize()).x ) / 2,
		(rect.bottom - (*pImage->GetSize()).y) / 2, 0));
	//哭率
	pImage->SetPosition(&D3DXVECTOR3(
		(rect.right * 0.05f),
		(rect.bottom - (*pImage->GetSize()).y) / 2, 0));
	//坷弗率
	pImage->SetPosition(&D3DXVECTOR3(
		(rect.right - (*pImage->GetSize()).x) - rect.right * 0.1f,
		(rect.bottom - (*pImage->GetSize()).y) / 2, 0));

	m_pMenu = pImage;
	
	
	pText = new UIText(Assets::GetFont(Assets::FT_NORMAL), m_pSprite);
	m_pMenu->AddChild(pText);
	pText->m_text = _T("SCENE");
	pText->SetSize(&D3DXVECTOR2(200, 60));
	pText->SetPosition(&D3DXVECTOR3(((*m_pMenu->GetSize()).x - (*pText->GetSize()).x) / 2, 30, 0));
	textColor = 0xff88dddd;
	pText->SetColor(&textColor);

	float btnY = 30;
	float btnOffsetY = 70;
	float btnLeft = m_pMenu->GetSize()->x / 2.0f - 80;
	
	textColor = 0xff338888;

	pButton = CreateButton(this, m_pSprite, m_pMenu, BTN_NAME_GRID,
		D3DXVECTOR3(btnLeft, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/main_btn_normal.png",
		ASSET_PATH + L"Textures/ui/main_btn_over.png",
		ASSET_PATH + L"Textures/ui/main_btn_select.png");
	pButton->SetText(Assets::GetFont(Assets::FT_NORMAL), BTN_NAME_GRID.c_str(), textColor);

	
	pButton = CreateButton(this, m_pSprite, m_pMenu, BTN_NAME_OBJ,
		D3DXVECTOR3(btnLeft, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/main_btn_normal.png",
		ASSET_PATH + L"Textures/ui/main_btn_over.png",
		ASSET_PATH + L"Textures/ui/main_btn_select.png");
	pButton->SetText(Assets::GetFont(Assets::FT_NORMAL), BTN_NAME_OBJ.c_str(), textColor);

	
	pButton = CreateButton(this, m_pSprite, m_pMenu, BTN_NAME_TERRAIN,
		D3DXVECTOR3(btnLeft, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/main_btn_normal.png",
		ASSET_PATH + L"Textures/ui/main_btn_over.png",
		ASSET_PATH + L"Textures/ui/main_btn_select.png");
	pButton->SetText(Assets::GetFont(Assets::FT_NORMAL), BTN_NAME_TERRAIN.c_str(), textColor);


	pButton = CreateButton(this, m_pSprite, m_pMenu, BTN_NAME_TEST,
		D3DXVECTOR3(btnLeft, btnY += btnOffsetY, 0),
		ASSET_PATH + L"Textures/ui/main_btn_normal.png",
		ASSET_PATH + L"Textures/ui/main_btn_over.png",
		ASSET_PATH + L"Textures/ui/main_btn_select.png");
	pButton->SetText(Assets::GetFont(Assets::FT_NORMAL), BTN_NAME_TEST.c_str(), textColor);
	
}

void MainUI::Update()
{
	if ( m_enableMenu) SAFE_UPDATE(m_pMenu);
	SAFE_UPDATE(m_pCloseBtn);
}

void MainUI::Render()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	float scale = 1;
	
	if (m_enableMenu)
	{
		scale = DX::GetInfo().screenWidth / (float)m_pBG->GetSize()->x;
		D3DXMatrixScaling(&worldMatrix, scale, scale, 1);
		m_pSprite->SetTransform(&worldMatrix);
		SAFE_RENDER(m_pBG);

		D3DXMatrixIdentity(&worldMatrix);
		m_pSprite->SetTransform(&worldMatrix);
		if (m_enableMenu) SAFE_RENDER(m_pMenu);
	}
	
	D3DXMatrixIdentity(&worldMatrix);
	m_pSprite->SetTransform(&worldMatrix);
	SAFE_RENDER(m_pCloseBtn);

	m_pSprite->End();
}

void MainUI::OnClick(UIButton * pSender)
{
	if (pSender->GetName() == (BTN_NAME_CLOSE))
	{
		Scenes::Get()->DeleteCurrentScene();
		m_enableMenu = true;
		Camera::Get()->Reset();
	}
	else if (pSender->GetName() == BTN_NAME_GRID)
	{
		m_enableMenu = false;
		Scenes::Get()->SetCurrentScene(SCENE_GRID);
	}
	else if (pSender->GetName() == BTN_NAME_OBJ)
	{
		m_enableMenu = false;
		Scenes::Get()->SetCurrentScene(SCENE_OBJ);
	}
	else if (pSender->GetName() == BTN_NAME_TERRAIN)
	{
		m_enableMenu = false;
		Scenes::Get()->SetCurrentScene(SCENE_TERRAIN);
	}
	else if (pSender->GetName() == BTN_NAME_TEST)
	{
		m_enableMenu = false;
		Scenes::Get()->SetCurrentScene(SCENE_TEST);
	}
	
}

UIButton * MainUI::CreateButton(IUIButtonDelegate * pDelegate, LPD3DXSPRITE pSprite,
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

