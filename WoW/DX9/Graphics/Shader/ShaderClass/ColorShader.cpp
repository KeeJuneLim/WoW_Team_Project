#include "stdafx.h"
#include "ColorShader.h"

ColorShader::ColorShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Color.fx");
}


ColorShader::~ColorShader()
{
	
}

void ColorShader::PreRender()
{
	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());
}

void ColorShader::Render(IDisplayObject* pObj)
{
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

void ColorShader::PostRender()
{

}


