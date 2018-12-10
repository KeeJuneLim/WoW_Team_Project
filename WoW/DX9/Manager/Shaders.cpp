#include "stdafx.h"
#include "Shaders.h"
#include "Graphics/Shader/ShaderClass/ColorShader.h"
#include "Graphics/Shader/ShaderClass/TextureShader.h"
#include "Graphics/Shader/ShaderClass/LightingShader.h"
#include "Graphics/Shader/ShaderClass/SkyGradientShader.h"
#include "Graphics/Shader/ShaderClass/TerrainEditorShader.h"
#include "Graphics/Shader/ShaderClass/ToonShader.h"
#include "Graphics/Shader/ShaderClass/BoundaryShader.h"
#include "Graphics/Shader/ShaderClass/HidingShader.h"
#include "Graphics/Shader/ShaderClass/CloudShader.h"
#include "Graphics/Shader/ShaderClass/ShadowMapShader.h"
#include "Graphics/Shader/ShaderClass/ShadowShader.h"

Shaders* Shaders::instance = NULL;

Shaders::Shaders()
{
	
}

Shaders::~Shaders()
{
	for (auto p : shaderList)
		SAFE_DELETE(p);

	for (auto p : m_pEffects)
		SAFE_RELEASE(p.second);
}

void Shaders::AddShader(IShader * pShader, ShaderType type)
{
	pShader->SetRenderQueue(type);
	shaderList[type] = pShader;
}

void Shaders::Init()
{
	shaderList.resize(ShaderType::ST_COUNT);

	AddShader(new SkyGradientShader, ShaderType::ST_SkyGradient);
	AddShader(new TerrainEditorShader, ShaderType::ST_TerrainEditor);
	AddShader(new ColorShader, ShaderType::ST_Color);
	AddShader(new TextureShader, ShaderType::ST_Texture);
	AddShader(new LightingShader, ShaderType::ST_Lighting);
	AddShader(new ToonShader, ShaderType::ST_Toon);
	AddShader(new BoundaryShader, ShaderType::ST_Boundary);
	AddShader(new HidingShader, ShaderType::ST_Hiding);
	AddShader(new CloudShader, ShaderType::ST_Cloud);
	AddShader(new ShadowMapShader, ShaderType::ST_ShadowMap);
	AddShader(new ShadowShader, ShaderType::ST_Shadow);
}

LPD3DXEFFECT Shaders::GetEffect(wstring filePath)
{
	if (m_pEffects.find(filePath) == m_pEffects.end())
	{
		m_pEffects[filePath] = LoadEffect(filePath);
	}
	return m_pEffects[filePath];
}

LPD3DXEFFECT Shaders::LoadEffect(wstring fullPath)
{
	LPD3DXEFFECT pEffect = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD flags = D3DXSHADER_DEBUG;
	
	D3DXCreateEffectFromFile(DX::GetDevice(), fullPath.c_str(),
		NULL, NULL, flags, NULL, &pEffect, &pError);
		
	
	if (!pEffect && pError)
	{
		int size = pError->GetBufferSize();
		LPVOID errorMsg = pError->GetBufferPointer();
		
		if (errorMsg)
		{
			char* str = new char[size];
			sprintf_s(str, size, (const char*)errorMsg, size);
			
			MessageBoxA(NULL, str, "LoadShader", 0);
			//OutputDebugStringA((str));
			delete[] str;
		}
	}
	
	return pEffect;
}
