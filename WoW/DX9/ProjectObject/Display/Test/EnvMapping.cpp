#include "stdafx.h"
#include "EnvMapping.h"
#include "ProjectObject/Display/Grid.h"
#include "Graphics/Interface/UI/IUIObject.h"
#include "ProjectObject/Display/Particle/SphereParticle.h"

EnvMapping::EnvMapping()
{
	Rendering::Get()->AddRenderingObject(this);

	position.y = 1;
	scale = D3DXVECTOR3(0.05, 0.05, 0.05);
}

EnvMapping::~EnvMapping()
{
	Rendering::Get()->RemoveRenderingObject(this);

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(g_pCubeTex);
	SAFE_RELEASE(g_pCubeDepth);
	
	SAFE_RELEASE(g_pSprite);
	
	for (auto p : g_pImageUI)
		SAFE_DELETE(p);
	for (auto p : g_pTextUI)
		SAFE_DELETE(p);
	for (auto p : g_pImageUITex)
		SAFE_RELEASE(p);

	SAFE_DELETE(grid);
	SAFE_DELETE(particle);
}

void EnvMapping::Init()
{	
	//쉐이더 지정
	m_pEffect = Shaders::Get()->GetEffect(SHADER_PATH + _T("EnvMapping.fx"));

	//모델 지정
	wstring path = (ASSET_PATH + L"shader/teapotwithtangent.x");

	D3DXLoadMeshFromX(path.c_str(), D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &m_pMesh);
	assert(m_pMesh && "Loading x file is failed");

	//텍스처 지정
	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/fieldStone/Fieldstone_DM.tga").c_str());
	m_pEffect->SetTexture("DiffuseMapTex", pTex);
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/fieldStone/Fieldstone_SM.tga").c_str());
	m_pEffect->SetTexture("SpecularMapTex", pTex);
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/fieldStone/Fieldstone_NM.tga").c_str());
	m_pEffect->SetTexture("NormalMapTex", pTex);
	
	//D3DXCreateCubeTextureFromFile(DX::GetDevice(), (ASSET_PATH + L"shader/snow_env.dds").c_str(), &g_pCubeTex);
	
	/**/
	D3DXCreateCubeTexture(DX::GetDevice(), TEX_SIZE, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pCubeTex);
	
	D3DXCreateSprite(DX::GetDevice(), &g_pSprite);
	
	LPCTSTR faceName[6] = {
		_T("POSITIVE_X"),
		_T("NEGATIVE_X"),
		_T("POSITIVE_Y"),
		_T("NEGATIVE_Y"),
		_T("POSITIVE_Z"),
		_T("NEGATIVE_Z")
	};
	
	for (int i = 0; i < 6; i++)
	{
		D3DXCreateTexture(DX::GetDevice(), TEX_SIZE, TEX_SIZE, 1,
			0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pImageUITex[i]);

		g_pImageUI[i] = new UIImage(g_pSprite);
		g_pImageUI[i]->SetPosition(&D3DXVECTOR3(i * TEX_SIZE, 0, 0));
		g_pImageUI[i]->SetTexture(g_pImageUITex[i], TEX_SIZE);
		g_pImageUI[i]->m_bDrawBorder = true;

		g_pTextUI[i] = new UIText(Assets::GetFont(Assets::FT_NORMAL), g_pSprite);
		g_pTextUI[i]->SetSize(&D3DXVECTOR2(TEX_SIZE, 60));
		g_pTextUI[i]->SetPosition(&D3DXVECTOR3(i * TEX_SIZE, 0, 0));
		g_pTextUI[i]->m_text = faceName[i];
	}

	DX::GetDevice()->CreateDepthStencilSurface(TEX_SIZE, TEX_SIZE,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &g_pCubeDepth, NULL);
	
	grid = new Grid(); grid->Init();
	particle = new SphereParticle(); particle->Init();
}

void EnvMapping::Update()
{
	if ( Camera::Get()->GetCameraMode() != CameraMode::CM_Free) IDisplayObject::UpdateInput();
	IDisplayObject::ApplyInput();
	position = targetPosition;
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();
	
	SAFE_UPDATE(grid);
	SAFE_UPDATE(particle);

	for (auto p : g_pImageUI)
		p->Update();
	for (auto p : g_pTextUI)
		p->Update();
}

void EnvMapping::Render()
{
	RenderCubeMap();
	SAFE_RENDER(grid);
	SAFE_RENDER(particle);
	
	RenderShader();
	RenderUI();
}

void EnvMapping::RenderCubeMap()
{
	
	// The projection matrix has a FOV of 90 degrees and asp ratio of 1
	/*
	D3DXMATRIXA16 mProj;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.5f, 1.0f, 0.1f, 100.0f);
	DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &mProj);
	*/

	LPDIRECT3DSURFACE9 pRTOri = NULL;
	DX::GetDevice()->GetRenderTarget(0, &pRTOri);
	LPDIRECT3DSURFACE9 pDSOri = NULL;
	DX::GetDevice()->GetDepthStencilSurface(&pDSOri);
	DX::GetDevice()->SetDepthStencilSurface(g_pCubeDepth);
	
	D3DXMATRIXA16 matViewCube, matViewOri;
	D3DXVECTOR3 camPos, camForward, camUp;

	camPos = position;
	
	D3DXVECTOR3 lookat[6] =
	{
		D3DXVECTOR3(1, 0, 0),
		D3DXVECTOR3(-1, 0, 0),
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, -1, 0),
		D3DXVECTOR3(0, 0, 1),
		D3DXVECTOR3(0, 0, -1)
	};
	D3DXVECTOR3 up[6] =
	{
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, 0, -1),
		D3DXVECTOR3(0, 0, 1),
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, 1, 0)
	};
	
	DX::GetDevice()->GetTransform(D3DTS_VIEW, &matViewOri);

	for (int nFace = 0; nFace < 6; ++nFace)
	{
		LPDIRECT3DSURFACE9 pSurf;
		g_pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)nFace, 0, &pSurf);
		DX::GetDevice()->SetRenderTarget(0, pSurf);
		DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1, 0);


		D3DXMATRIXA16 mViewDir(*Camera::Get()->GetViewMatrix());
		//D3DXVec3TransformNormal(&camForward, &lookat[nFace], &mViewDir);
		camForward = lookat[nFace] + camPos;
		camUp = up[nFace] + camPos;
		D3DXMatrixLookAtLH(&matViewCube, &camPos, &camForward, &up[nFace]);
		DX::GetDevice()->SetTransform(D3DTS_VIEW, &matViewCube);

		SAFE_RENDER(grid);
		SAFE_RENDER(particle);
		
		// Render ImageUI
		LPDIRECT3DSURFACE9 dest = NULL;
		g_pImageUITex[nFace]->GetSurfaceLevel(0, &dest);
		D3DXLoadSurfaceFromSurface(dest, NULL, NULL, pSurf, NULL, NULL, D3DX_DEFAULT, 0);
		g_pImageUI[nFace]->SetTexture(g_pImageUITex[nFace], TEX_SIZE);

		SAFE_RELEASE(dest);
		SAFE_RELEASE(pSurf);
	}
	
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &matViewOri);

	// Restore depth-stencil buffer and render target
	DX::GetDevice()->SetDepthStencilSurface(pDSOri);
	SAFE_RELEASE(pDSOri);
	DX::GetDevice()->SetRenderTarget(0, pRTOri);
	SAFE_RELEASE(pRTOri);

}

void EnvMapping::RenderShader()
{
	
	m_pEffect->SetFloatArray("gWorldLightDir", (float*)&Light::WorldLightDir, 3);
	m_pEffect->SetFloatArray("gLightColor", (float*)&Light::LightColor, 3);
	m_pEffect->SetVector("gWorldCamPos", &D3DXVECTOR4(*Camera::Get()->GetPosition(), 1));

	//매트릭스 업데이트
	D3DXMATRIX matVP;
	m_pEffect->SetMatrix("gWorldMatrix", &worldMatrix);
	matVP = *Camera::Get()->GetVPMatrix();
	m_pEffect->SetMatrix("gWVPMatrix", &(worldMatrix * matVP));


	m_pEffect->Begin(0, NULL);

	m_pEffect->BeginPass(0);
	m_pEffect->SetTexture("EnvMapTex", g_pCubeTex);
	m_pEffect->CommitChanges();
	m_pMesh->DrawSubset(0);
	m_pEffect->EndPass();

	m_pEffect->End();
}


void EnvMapping::RenderUI()
{
	for (auto p : g_pImageUITex)
		ImGui::Image(p, ImVec2(100, 70));
	

	D3DXMATRIXA16 matS, matT;
	float sprScale = 0.7f;
	D3DXMatrixScaling(&matS, sprScale, sprScale, sprScale);
	float offsetY = DX::GetInfo().screenHeight - TEX_SIZE * sprScale;
	D3DXMatrixTranslation(&matT, 0, offsetY, 0);
	g_pSprite->SetTransform(&(matS * matT));

	g_pSprite->Begin(0);
	
	for (auto p : g_pImageUI)
		p->Render();
	
	g_pSprite->End();

	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	for (auto p : g_pTextUI)
		p->Render();

	g_pSprite->End();
}
