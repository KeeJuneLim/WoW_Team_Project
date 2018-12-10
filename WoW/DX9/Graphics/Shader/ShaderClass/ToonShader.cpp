#include "stdafx.h"
#include "ToonShader.h"

ToonShader::ToonShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Toon.fx");
}


ToonShader::~ToonShader()
{
	
}

void ToonShader::PreRender()
{
	effect->SetFloatArray(hWorldLightDir, (float*)&Light::WorldLightDir, 3);
	effect->SetFloatArray(hLightColor, (float*)&Light::LightColor, 3);
}


void ToonShader::Render(IDisplayObject* pObj)
{
	D3DXMATRIX wvpMatrix =
		*pObj->GetWorldMatrix() *
		*Camera::Get()->GetViewMatrix() *
		*Camera::Get()->GetProjectionMatrix();

	SetMatrix(MT_WVP, &wvpMatrix);

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


void ToonShader::PostRender()
{

}
