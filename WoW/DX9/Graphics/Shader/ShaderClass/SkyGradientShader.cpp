#include "stdafx.h"
#include "SkyGradientShader.h"

SkyGradientShader::SkyGradientShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"SkyGradient.fx");
}

SkyGradientShader::~SkyGradientShader()
{
	
}

void SkyGradientShader::PreRender()
{
	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());
	effect->SetFloatArray(hWorldLightDir, (float*)&Light::WorldLightDir, 3);
	effect->SetFloatArray(hLightColor, (float*)&Light::LightColor, 3);
	effect->SetFloatArray("gApexColor", (float*)&Light::ApexColor, 3);
	effect->SetFloatArray("gCenterColor", (float*)&Light::CenterColor, 3);
}

void SkyGradientShader::Render(IDisplayObject* pObj)
{
	effect->SetMatrix(hWorldMatrix, pObj->GetWorldMatrix());
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

void SkyGradientShader::PostRender()
{

}


