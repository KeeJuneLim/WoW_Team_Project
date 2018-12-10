#include "stdafx.h"
#include "HidingShader.h"

HidingShader::HidingShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"Hiding.fx");

	D3DSURFACE_DESC bbDesc;
	DX::GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bbSurf);
	bbSurf->GetDesc(&bbDesc);

	D3DXCreateTexture(DX::GetDevice(), bbDesc.Width, bbDesc.Height, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &tex);

	tex->GetSurfaceLevel(0, &texSurf);
	
	effect->SetTexture("BackbufferTex", tex);
	
	wstring filepath = ASSET_PATH + L"shader/noisemap.jpg";
	effect->SetTexture("NoiseMapTex", Assets::GetTexture(filepath.c_str()));
}


HidingShader::~HidingShader()
{
	bbSurf->Release();
	texSurf->Release();
	tex->Release();
}

void HidingShader::PreRender()
{
	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());

	effect->SetFloatArray(hWorldLightDir, (float*)&Light::WorldLightDir, 3);
	effect->SetFloatArray(hLightColor, (float*)&Light::LightColor, 3);
	effect->SetVector(hWorldCamPos, &D3DXVECTOR4(*Camera::Get()->GetPosition(), 1));

	D3DXLoadSurfaceFromSurface(texSurf, NULL, NULL, bbSurf, NULL, NULL, D3DX_DEFAULT, 0);
	
	
}


void HidingShader::Render(IDisplayObject* pObj)
{
	UINT numPasses = 0;
	effect->Begin(&numPasses, NULL);
	
	effect->SetBool("gOnChange", ShaderData::hiding.onChange);
	effect->SetFloat("gHidingFactor", ShaderData::hiding.hidingFactor);
	effect->SetFloat("gHidingBumpness", ShaderData::hiding.hidingBumpness);
	
	for (int i = 0; i < numPasses; i++)
	{
		effect->BeginPass(i);

		pObj->Draw();

		effect->EndPass();
	}

	effect->End();
}


void HidingShader::PostRender()
{

}
