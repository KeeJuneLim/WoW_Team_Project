#include "stdafx.h"
#include "Weapon.h"
#include "Graphics\Model\X\AllocateHierarchyHLSL.h"


Weapon::Weapon()
{
	Rendering::Get()->AddRenderingObject(this);
}

Weapon::~Weapon()
{
	SAFE_RELEASE(axe);

	Rendering::Get()->RemoveRenderingObject(this);
}

void Weapon::Init()
{
	//Camera::Get()->SetOffset(&D3DXVECTOR3(0, 10, -50));

	m_pCreateShadow = Shaders::Get()->GetEffect(SHADER_PATH + _T("SimpleCreateShadow.fx"));
	m_pApplyShadow = Shaders::Get()->GetEffect(SHADER_PATH + _T("SimpleApplyShadow.fx"));


	//모델 지정
	LPCTSTR xPath;

	xPath = _T("../../_Assets/Models/X/Player/Axe.X");
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &axe)))
		assert(false && "Loading x file is failed");



	const int SHADOWMAP_SIZE = 2048;
	D3DXCreateTexture(DX::GetDevice(), SHADOWMAP_SIZE, SHADOWMAP_SIZE, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget);

	DX::GetDevice()->CreateDepthStencilSurface(SHADOWMAP_SIZE, SHADOWMAP_SIZE,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &m_pShadowDepthStencil, NULL);

}

void Weapon::Update()
{
}

void Weapon::Update(IDisplayObject * frame, string name)
{
	FRAME_EX* tmp = (FRAME_EX*)D3DXFrameFind(dynamic_cast<SkinnedMeshHLSL*>(frame)->m_pRootFrame, name.c_str());
	this->SetWorldMatrix(tmp->CombinedTM);
}

void Weapon::Render()
{

	D3DXMATRIX matLightView;
	D3DXMATRIX matLightProj;

	D3DXVECTOR3 eye(Light::WorldLightPos);
	D3DXVECTOR3 at(eye + Light::WorldLightDir);
	D3DXVECTOR3 up(0, 1, 0);

	D3DXMatrixLookAtLH(&matLightView, &eye, &at, &up);
	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DX_PI / 4.0f, 1, 0.1f, 1000.0f);

	m_pCreateShadow->SetMatrix("gLightViewMatrix", &matLightView);
	m_pCreateShadow->SetMatrix("gLightProjMatrix", &matLightProj);

	LPDIRECT3DSURFACE9 pHWRenderTarget = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencil = NULL;
	DX::GetDevice()->GetRenderTarget(0, &pHWRenderTarget);
	DX::GetDevice()->GetDepthStencilSurface(&pHWDepthStencil);

	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		DX::GetDevice()->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
	}

	DX::GetDevice()->SetDepthStencilSurface(m_pShadowDepthStencil);
	DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1, 0);

	//매트릭스 업데이트
	//D3DXMATRIXA16 matS_disc, matT_disc, matS_torus, matT_torus;
	//float scale = 0.5f;
	//D3DXMatrixScaling(&matS_torus, scale, scale, scale);
	//D3DXMatrixTranslation(&matT_disc, 0, -0.5, 0);
	//D3DXMatrixTranslation(&matT_torus, 0, 3, 0);




	UINT numPasses = 0;
	m_pCreateShadow->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pCreateShadow->BeginPass(i);

		//worldMatrix = matS_torus * matT_torus;
		m_pCreateShadow->SetMatrix("gWorldMatrix", &worldMatrix);
		m_pCreateShadow->CommitChanges();
		axe->DrawSubset(0);

		m_pCreateShadow->EndPass();
	}

	m_pCreateShadow->End();

	DX::GetDevice()->SetRenderTarget(0, pHWRenderTarget);
	DX::GetDevice()->SetDepthStencilSurface(pHWDepthStencil);

	pHWRenderTarget->Release();
	pHWDepthStencil->Release();

	ImGui::Image(m_pShadowRenderTarget, ImVec2(100, 100));

	//그림자를 적용한 실제 렌더링
	m_pApplyShadow->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
	m_pApplyShadow->SetMatrix("gProjMatrix", Camera::Get()->GetProjectionMatrix());
	m_pApplyShadow->SetMatrix("gLightViewMatrix", &matLightView);
	m_pApplyShadow->SetMatrix("gLightProjMatrix", &matLightProj);
	m_pApplyShadow->SetTexture("ShadowMapTex", m_pShadowRenderTarget);

	numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);
	D3DXVECTOR4 color;

	for (int i = 0; i < numPasses; i++)
	{
		m_pApplyShadow->BeginPass(i);

		color = D3DXVECTOR4(0, 1, 0, 0);
		//worldMatrix = matS_torus * matT_torus;
		m_pApplyShadow->SetVector("gObjectColor", &color);
		m_pApplyShadow->SetMatrix("gWorldMatrix", &worldMatrix);
		m_pApplyShadow->CommitChanges();
		axe->DrawSubset(0);

		m_pApplyShadow->EndPass();
	}

	m_pApplyShadow->End();
}

void Weapon::SetWorldMatrix(D3DXMATRIX combinedTM)
{
	worldMatrix = combinedTM;
}
