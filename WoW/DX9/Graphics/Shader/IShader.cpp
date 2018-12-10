#include "stdafx.h"
#include "Graphics/Model/Terrain/TerrainEditor.h"

void IShader::SetMatrix(MatrixType type, D3DXMATRIX * matrix)
{
	switch (type)
	{
	case MT_World:
		effect->SetMatrix(hWorldMatrix, matrix);
		break;
	case MT_View:
		effect->SetMatrix(hViewMatrix, matrix);
		break;
	case MT_Proj:
		effect->SetMatrix(hProjMatrix, matrix);
		break;
	case MT_WVP:
		effect->SetMatrix(hWVPMatrix, matrix);
		break;
	}
}

void IShader::SetMaterial(Material* material)
{
	effect->SetVector(hMtl_D, (D3DXVECTOR4*)&material->Diffuse);
	effect->SetVector(hMtl_A, (D3DXVECTOR4*)&material->Ambient);
	effect->SetVector(hMtl_S, (D3DXVECTOR4*)&material->Specular);
	effect->SetFloat(hMtl_P, material->Power);
}

void IShader::SetTexture(TextureType index, IDirect3DTexture9 * pTexture)
{
	switch (index)
	{
	case TextureType::TT_DiffuseMap:
		effect->SetTexture(hDiffuseMapTex, pTexture);
		break;
	case TextureType::TT_DiffuseMap1:
		effect->SetTexture(hDiffuseMap1Tex, pTexture);
		break;
	case TextureType::TT_DiffuseMap2:
		effect->SetTexture(hDiffuseMap2Tex, pTexture);
		break;
	case TextureType::TT_DiffuseMap3:
		effect->SetTexture(hDiffuseMap3Tex, pTexture);
		break;
	case TextureType::TT_DiffuseMap4:
		effect->SetTexture(hDiffuseMap4Tex, pTexture);
		break;
	case TextureType::TT_NormalMap:
		effect->SetTexture(hNormalMapTex, pTexture);
		break;
	case TextureType::TT_SpecularMap:
		effect->SetTexture(hSpecularMapTex, pTexture);
		break;
	case TextureType::TT_AlphaMap:
		effect->SetTexture(hAlphaMapTex, pTexture);
		break;
	}
}

void IShader::SetTerrainEditor(TerrainEditor * pEditor)
{
	effect->SetFloatArray("gBrushCenter", (float*)&pEditor->center, 3);
	effect->SetFloatArray("gBrushColor", (float*)&pEditor->color, 3);
	effect->SetFloat("gBrushRange", pEditor->range);
	effect->SetFloat("gBrushShape", pEditor->shape);
}
