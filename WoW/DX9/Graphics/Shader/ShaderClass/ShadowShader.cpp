#include "stdafx.h"
#include "ShadowShader.h"

ShadowShader::ShadowShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Shadow.fx");
}


ShadowShader::~ShadowShader()
{
	
}

void ShadowShader::PreRender()
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

	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());
	
	effect->SetFloatArray(hWorldLightDir, (float*)&Light::WorldLightDir, 3);
	effect->SetFloatArray(hLightColor, (float*)&Light::LightColor, 3);
	effect->SetVector(hWorldCamPos, &D3DXVECTOR4(*Camera::Get()->GetPosition(), 1));

	effect->SetTexture("ShadowMapTex", ShaderData::shadow.shadowMap);
}


void ShadowShader::Render(IDisplayObject* pObj)
{
	SetMatrix(MatrixType::MT_World, pObj->GetWorldMatrix());
	SetTexture(TextureType::TT_DiffuseMap, pObj->GetMaterial()->GetDiffuseMapTex());
	SetMaterial(pObj->GetMaterial());
	Commit();

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


void ShadowShader::PostRender()
{

}
