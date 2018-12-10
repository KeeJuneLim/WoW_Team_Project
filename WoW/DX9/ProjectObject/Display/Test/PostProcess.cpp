#include "stdafx.h"
#include "PostProcess.h"
#include "ProjectObject/Display/Grid.h"

static LPD3DXSPRITE g_pSprite;
static UIImage* g_pMenuUI;
static UIButton* g_pButtonUI[4];


static LPD3DXMESH g_pMesh;
static Grid* g_pGrid;
static LPD3DXMESH g_pTeapot;
static const int TEAPOT_DIM = 5;
static const LPCTSTR g_buttonName[] =
{
	_T("Inverse"),
	_T("GBlurH"),
	_T("GBlurV"),
	_T("Monochrome"),
	_T("BloomH"),
	_T("BloomV"),
	_T("BrightPass"),
	_T("ToneMap"),
	_T("EdgeDetect")
};


const D3DVERTEXELEMENT9 PostProcess::ScreenVtx::Decl[] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
	D3DDECL_END()
};

static const LPCTSTR g_fxFile[] =
{
	_T("PP_ColorInverse.fx"),
	_T("PP_ColorGBlurH.fx"),
	_T("PP_ColorGBlurV.fx"),
	_T("PP_ColorMonochrome.fx"),
	_T("PP_ColorBloomH.fx"),
	_T("PP_ColorBloomV.fx"),
	_T("PP_ColorBrightPass.fx"),
	_T("PP_ColorToneMap.fx"),
	_T("PP_ColorEdgeDetect.fx")
	/*
	_T("PP_ColorDownFilter4.fx"),
	_T("PP_ColorUpFilter4.fx"),
	_T("PP_ColorCombine.fx"),
	_T("PP_ColorCombine4.fx"),
	_T("PP_NormalEdgeDetect.fx"),
	_T("PP_DofCombine.fx"),
	_T("PP_NormalMap.fx"),
	_T("PP_PositionMap.fx")
	*/
};

PostProcess::~PostProcess()
{
	SAFE_RELEASE(m_pScreenVB);
	SAFE_RELEASE(m_pScreenVtxDecl);
	SAFE_RELEASE(m_pColorTex);
	SAFE_RELEASE(m_pColorSurf);
	SAFE_RELEASE(m_pNormalTex);
	SAFE_RELEASE(m_pNormalSurf);
	SAFE_RELEASE(m_pPositionTex);
	SAFE_RELEASE(m_pPositionSurf);
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthSurf);
	
	m_rtChain.Cleanup();

	SAFE_RELEASE(g_pMesh);
	SAFE_RELEASE(g_pTeapot);

	SAFE_RELEASE(g_pSprite);
	SAFE_DELETE(g_pMenuUI);

}

void PostProcess::Init()
{
	m_pEffect = Shaders::Get()->GetEffect((SHADER_PATH + _T("postprocess/") + _T("Scene.fx")).c_str());

	
	ScreenVtx m_vertices[4] = 
	{
		{ -0.5, -0.5, 1, 1, 0, 0},
		{ DX::GetInfo().screenWidth - 0.5, -0.5, 1, 1, 1, 0},
		{ -0.5, DX::GetInfo().screenHeight - 0.5, 1, 1, 0, 1},
		{ DX::GetInfo().screenWidth - 0.5, DX::GetInfo().screenHeight - 0.5, 1, 1, 1, 1}
	};
	
	DX::GetDevice()->CreateVertexDeclaration(ScreenVtx::Decl, &m_pScreenVtxDecl);
	
	DXUtil::CreateVertexBuffer(m_pScreenVB, (LPVOID*)&m_vertices[0], 0, 4, sizeof(ScreenVtx));
	
	IDirect3DTexture9* pRt[2];
	D3DXCreateTexture(DX::GetDevice(), DX::GetInfo().screenWidth, DX::GetInfo().screenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRt[0]);
	D3DXCreateTexture(DX::GetDevice(), DX::GetInfo().screenWidth, DX::GetInfo().screenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRt[1]);
	m_rtChain.Init(pRt);
	
	D3DXCreateTexture(DX::GetDevice(), DX::GetInfo().screenWidth, DX::GetInfo().screenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pColorTex);
	m_pColorTex->GetSurfaceLevel(0, &m_pColorSurf);

	D3DXCreateTexture(DX::GetDevice(), DX::GetInfo().screenWidth, DX::GetInfo().screenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pNormalTex);
	m_pNormalTex->GetSurfaceLevel(0, &m_pNormalSurf);

	D3DXCreateTexture(DX::GetDevice(), DX::GetInfo().screenWidth, DX::GetInfo().screenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pPositionTex);
	m_pPositionTex->GetSurfaceLevel(0, &m_pPositionSurf);

	D3DXCreateTexture(DX::GetDevice(), DX::GetInfo().screenWidth, DX::GetInfo().screenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pDepthTex);
	m_pDepthTex->GetSurfaceLevel(0, &m_pDepthSurf);
	
	int num = sizeof(g_buttonName) / sizeof(g_buttonName[0]);

	for (int i = 0; i < num; i++)
	{
		LPD3DXEFFECT pEffect = Shaders::Get()->GetEffect((SHADER_PATH + _T("postprocess/") + g_fxFile[i]).c_str());
		pEffect->SetFloat("gWidth", DX::GetInfo().screenWidth);
		pEffect->SetFloat("gHeight", DX::GetInfo().screenHeight);
		pEffect->SetTexture("g_txSceneDepth", m_pDepthTex);
		m_pEffectList.push_back(pEffect);
	}

	m_pLightingEffect = Shaders::Get()->GetEffect((SHADER_PATH + L"Lighting.fx").c_str());

	InitScene();
	InitUI();
}

void PostProcess::Update()
{
	g_pMenuUI->Update();
	
}

void PostProcess::Render()
{
	LPDIRECT3DSURFACE9 pRTOri = NULL;
	DX::GetDevice()->GetRenderTarget(0, &pRTOri);
	
	RenderSceneToQuad();
	
	PerformPP();
	
	DX::GetDevice()->SetRenderTarget(0, pRTOri);
	SAFE_RELEASE(pRTOri);
	DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1, 0);

	RenderFinalQuad();
	RenderUI();
}

void PostProcess::InitScene()
{
	LPCTSTR xPath = (ASSET_PATH + L"Models/zealot/zealot.x").c_str();
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &g_pMesh)))
		assert(false && "Loading x file is failed");

	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = Assets::GetTexture((ASSET_PATH + L"Models/zealot/Zealot_Diffuse.bmp").c_str());
	m_pLightingEffect->SetTexture("DiffuseMap_Tex", pTex);

	xPath = _T("../../_assets/shader/sphere.x"); 
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &g_pTeapot)))
		assert(false && "Loading x file is failed");

	g_pGrid = new Grid(); g_pGrid->Init();

	
}

void PostProcess::InitUI()
{
	D3DXCreateSprite(DX::GetDevice(), &g_pSprite);

	g_pMenuUI = new UIImage(g_pSprite);
	g_pMenuUI->SetTexture((ASSET_PATH + L"Textures/ui/main_panel2.png").c_str());
	g_pMenuUI->SetPosition(&D3DXVECTOR3(0, 0, 0));

	D3DCOLOR textColor(0xff338888);
	float btnY = 0;
	
	int num = sizeof(g_buttonName) / sizeof(g_buttonName[0]);

	for (int i = 0; i < num; i++)
	{
		UIButton* pButton = new UIButton(this, g_pSprite);
		pButton->SetTexture(
			ASSET_PATH + _T("Textures/ui/main_btn2_normal.png"),
			ASSET_PATH + _T("Textures/ui/main_btn2_normal.png"),
			ASSET_PATH + _T("Textures/ui/main_btn2_normal.png"));

		btnY = i * (pButton->GetSize()->y + 4) + 20;

		pButton->SetPosition(&D3DXVECTOR3(g_pMenuUI->GetSize()->x / 2 - pButton->GetSize()->x - 10, btnY, 0));
		pButton->SetText(Assets::GetFont(Assets::FontType_NORMAL2), g_buttonName[i], textColor);
		pButton->SetName(g_buttonName[i]);
		pButton->SetTag(0);
		g_pMenuUI->AddChild(pButton);

		pButton = new UIButton(this, g_pSprite);
		pButton->SetTexture(
			ASSET_PATH + _T("Textures/ui/main_btn2_normal.png"),
			ASSET_PATH + _T("Textures/ui/main_btn2_normal.png"),
			ASSET_PATH + _T("Textures/ui/main_btn2_normal.png"));
		pButton->SetPosition(&D3DXVECTOR3(g_pMenuUI->GetSize()->x / 2 + 10, btnY, 0));
		pButton->SetText(Assets::GetFont(Assets::FontType_NORMAL2), g_buttonName[i], textColor);
		pButton->SetName(g_buttonName[i]);
		pButton->SetTag(1);
		g_pMenuUI->AddChild(pButton);
	}

	
}

void PostProcess::RenderSceneToQuad()
{
	DX::GetDevice()->SetRenderTarget(0, m_pColorSurf);
	DX::GetDevice()->SetRenderTarget(1, m_pNormalSurf);
	DX::GetDevice()->SetRenderTarget(2, m_pPositionSurf);
	DX::GetDevice()->SetRenderTarget(3, m_pDepthSurf);

	DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1, 0);

	SAFE_RENDER(g_pGrid);
	
	float scale = 10.0f;
	D3DXMatrixScaling(&worldMatrix, scale, scale, scale);
	D3DXVECTOR3 lightDir(1, -1, 1);
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVECTOR4 camPos(*Camera::Get()->GetPosition(), 1);
	
	m_pEffect->SetTechnique("RenderScene");


	D3DXMATRIXA16 matWV, matV;
	matV = *Camera::Get()->GetViewMatrix();
	matWV = worldMatrix * matV;

	D3DXMatrixScaling(&worldMatrix, scale, scale, scale);
	m_pEffect->SetMatrix("g_mWorldView", &matWV);
	m_pEffect->SetMatrix("g_mProj", Camera::Get()->GetProjectionMatrix());
	
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVec3TransformNormal(&lightDir, &lightDir, &matV);
	m_pEffect->SetFloatArray("g_vLightDirView", (float*)&lightDir, 3);
	

	m_pEffect->Begin(0, NULL);
	m_pEffect->BeginPass(0);

	m_pEffect->SetTexture("g_txScene", Assets::GetTexture(ASSET_PATH + _T("Models/zealot/Zealot_Diffuse.bmp")));
	m_pEffect->CommitChanges();

	g_pMesh->DrawSubset(0);

	//m_pEffect->SetTexture("g_txScene", Assets::GetTexture(_T("../../_assets/shader/Fieldstone/Fieldstone_DM.tga")));
	m_pEffect->SetTexture("g_txScene", Assets::GetTexture(ASSET_PATH + _T("Models/zealot/Zealot_Diffuse.bmp")));
	
	D3DXMATRIXA16 matT, matS;
	float offset = 20;
	scale = 0.2f;
	for ( int nY = 0; nY < TEAPOT_DIM; nY++)
		for (int nZ = 0; nZ < TEAPOT_DIM; nZ++)
			for (int nX = 0; nX < TEAPOT_DIM; nX++)
			{
				D3DXMatrixScaling(&matS, scale, scale, scale);
				D3DXMatrixTranslation(&matT, (nX - TEAPOT_DIM / 2) * offset, (nY - TEAPOT_DIM / 2) * offset, (nZ - TEAPOT_DIM / 2) * offset);
				m_worldMatrix = matS * matT;
				matWV = m_worldMatrix * matV;
				m_pEffect->SetMatrix("g_mWorldView", &matWV);
				
				m_pEffect->CommitChanges();

				g_pTeapot->DrawSubset(0);
			}
				

	m_pEffect->EndPass();
	m_pEffect->End();

	DX::GetDevice()->SetRenderTarget(1, NULL);
	DX::GetDevice()->SetRenderTarget(2, NULL);
	DX::GetDevice()->SetRenderTarget(3, NULL);
	
}

void PostProcess::PerformPP()
{
	m_rtChain.rtIndex = 0;
	m_rtChain.bFirst = true;

	if (m_pPPList.size() == 0) return;

	for (auto p : m_pPPList)
	{
		PerformSinglePP(p);
	}
}

void PostProcess::PerformSinglePP(LPD3DXEFFECT pEffect)
{
	LPDIRECT3DSURFACE9 pSurf;
	m_rtChain.GetTarget()->GetSurfaceLevel(0, &pSurf);
	DX::GetDevice()->SetRenderTarget(0, pSurf);
	SAFE_RELEASE(pSurf);
	DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1, 0);

	DX::GetDevice()->SetVertexDeclaration(m_pScreenVtxDecl);
	pEffect->Begin(0, NULL);
	pEffect->BeginPass(0);

	if (m_rtChain.bFirst)
	{
		m_rtChain.bFirst = false;
		pEffect->SetTexture("g_txSrcColor", m_pColorTex);
	}
	else
	{
		pEffect->SetTexture("g_txSrcColor", m_rtChain.GetSource());
	}
	
	pEffect->CommitChanges();
	
	DX::GetDevice()->SetStreamSource(0, m_pScreenVB, 0, sizeof(ScreenVtx));
	DX::GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	m_rtChain.Flip();
}

void PostProcess::RenderFinalQuad()
{
	DX::GetDevice()->SetVertexDeclaration(m_pScreenVtxDecl);
	m_pEffect->SetTechnique("RenderNoLight");

	if(m_rtChain.bFirst)
		m_pEffect->SetTexture("g_txScene", m_pColorTex);
	else
		m_pEffect->SetTexture("g_txScene", m_rtChain.GetSource());
	
	
	m_pEffect->Begin(0, NULL);
	m_pEffect->BeginPass(0);
	m_pEffect->CommitChanges();
	DX::GetDevice()->SetStreamSource(0, m_pScreenVB, 0, sizeof(ScreenVtx));
	DX::GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	m_pEffect->EndPass();
	m_pEffect->End();
}

void PostProcess::RenderUI()
{
	/**/
	D3DXMATRIXA16 mat;
	D3DXMatrixTranslation(&mat, 10, (DX::GetInfo().screenHeight - g_pMenuUI->GetSize()->y) / 2, 0);
	g_pSprite->SetTransform(&mat);
	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	g_pMenuUI->Render();


	g_pSprite->End();

	
	
}

void PostProcess::OnClick(UIButton * pSender)
{
	for (int i = 0; i < m_pEffectList.size(); i++)
	{
		if (pSender->GetName() == g_buttonName[i])
		{
			if (pSender->GetTag() == 0) m_pPPList.push_back(m_pEffectList[i]);
			else m_pPPList.remove(m_pEffectList[i]);
		}
	}
}
