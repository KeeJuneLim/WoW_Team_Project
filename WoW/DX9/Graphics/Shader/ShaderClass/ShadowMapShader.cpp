#include "stdafx.h"
#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"ShadowMap.fx");

	const int SHADOWMAP_SIZE = 2048;
	D3DXCreateTexture(DX::GetDevice(), SHADOWMAP_SIZE, SHADOWMAP_SIZE, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &ShaderData::shadow.shadowMap);

	ShaderData::shadow.shadowMap->GetSurfaceLevel(0, &pShadowSurface);

	DX::GetDevice()->CreateDepthStencilSurface(SHADOWMAP_SIZE, SHADOWMAP_SIZE,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &m_pShadowDepthStencil, NULL);
}


ShadowMapShader::~ShadowMapShader()
{
	SAFE_RELEASE(pShadowSurface);
	SAFE_RELEASE(ShaderData::shadow.shadowMap);
	SAFE_RELEASE(m_pShadowDepthStencil);
}

void ShadowMapShader::PreRender()
{
	D3DXMATRIX matLightView;
	D3DXMATRIX matLightProj;
	
	
	D3DXVECTOR3 eye(Light::WorldLightPos);
	D3DXVECTOR3 at(eye + Light::WorldLightDir);
	D3DXVECTOR3 up(0, 1, 0);

	D3DXMatrixLookAtLH(&matLightView, &eye, &at, &up);
	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DX_PI / 4.0f, 1, 0.1f, 500);

	effect->SetMatrix(hLightViewMatrix, &matLightView);
	effect->SetMatrix(hLightProjMatrix, &matLightProj);


	DX::GetDevice()->GetRenderTarget(0, &pHWRenderTarget);
	DX::GetDevice()->GetDepthStencilSurface(&pHWDepthStencil);
	
	DX::GetDevice()->SetRenderTarget(0, pShadowSurface);
	DX::GetDevice()->SetDepthStencilSurface(m_pShadowDepthStencil);
	DX::GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1, 0);
	
}


void ShadowMapShader::Render(IDisplayObject* pObj)
{
	effect->SetMatrix(hWorldMatrix, pObj->GetWorldMatrix());

	UINT numPasses = 0;
	effect->Begin(&numPasses, NULL);
	
	for (int i = 0; i < numPasses; i++)
	{
		effect->BeginPass(i);

		pObj->Draw();

		effect->EndPass();
	}
	
	effect->End();
}


void ShadowMapShader::PostRender()
{
	DX::GetDevice()->SetRenderTarget(0, pHWRenderTarget);
	DX::GetDevice()->SetDepthStencilSurface(pHWDepthStencil);
	
	SAFE_RELEASE(pHWRenderTarget);
	SAFE_RELEASE(pHWDepthStencil);
	
	
}
