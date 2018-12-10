#include "stdafx.h"
#include "TextureShader.h"

TextureShader::TextureShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Texture.fx");
}


TextureShader::~TextureShader()
{
	
}

void TextureShader::PreRender()
{
	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());
}

void TextureShader::Render(IDisplayObject* pObj)
{
	UINT numPasses = 0;
	effect->Begin(&numPasses, NULL);
	static float cos = 0;
	cos += 0.02f;
	effect->SetFloat("gConst", cosf(cos));

	for (int i = 0; i < numPasses; i++)
	{
		effect->BeginPass(i);
		
		pObj->Draw();

		effect->EndPass();
	}

	effect->End();
}

void TextureShader::PostRender()
{

}


