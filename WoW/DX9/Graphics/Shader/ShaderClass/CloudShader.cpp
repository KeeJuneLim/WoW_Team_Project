#include "stdafx.h"
#include "CloudShader.h"

CloudShader::CloudShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Cloud.fx");
}


CloudShader::~CloudShader()
{
	
}

void CloudShader::PreRender()
{
	effect->SetFloat("gTranslation", Time::Get()->GetRunningTime() * 0.005f);
	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());
}


void CloudShader::Render(IDisplayObject* pObj)
{
	effect->SetMatrix(hWorldMatrix, pObj->GetWorldMatrix());
	effect->SetTexture(hDiffuseMapTex, pObj->GetMaterial()->GetDiffuseMapTex());
	effect->SetTexture(hDiffuseMap1Tex, pObj->GetMaterial()->GetDiffuseMapTex(1));
	SetMaterial(pObj->GetMaterial());
	effect->CommitChanges();

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


void CloudShader::PostRender()
{

}
