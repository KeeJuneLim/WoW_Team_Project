#include "stdafx.h"
#include "TerrainEditorShader.h"

TerrainEditorShader::TerrainEditorShader()
{
	effect = Shaders::Get()->GetEffect(SHADER_PATH + L"TerrainBrush.fx");
}


TerrainEditorShader::~TerrainEditorShader()
{
	
}

void TerrainEditorShader::PreRender()
{
	effect->SetMatrix(hViewMatrix, Camera::Get()->GetViewMatrix());
	effect->SetMatrix(hProjMatrix, Camera::Get()->GetProjectionMatrix());

	effect->SetFloatArray(hWorldLightDir, (float*)&Light::WorldLightDir, 3);
	effect->SetFloatArray(hLightColor, (float*)&Light::LightColor, 3);
	effect->SetVector(hWorldCamPos, &D3DXVECTOR4(*Camera::Get()->GetPosition(), 1));
}


void TerrainEditorShader::Render(IDisplayObject* pObj)
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


void TerrainEditorShader::PostRender()
{

}
