#include "stdafx.h"
#include "NormalMapping.h"

NormalMapping::NormalMapping()
{
	Rendering::Get()->AddRenderingObject(this);

	scale = D3DXVECTOR3(0.05, 0.05, 0.05);
}

NormalMapping::~NormalMapping()
{
	Rendering::Get()->RemoveRenderingObject(this);

	SAFE_RELEASE(m_pMesh);
}

void NormalMapping::Init()
{
	//쉐이더 지정
	m_pEffect = Shaders::Get()->GetEffect((SHADER_PATH + L"NormalMapping.fx").c_str());
	
	//모델 지정
	wstring path = (ASSET_PATH + L"shader/teapotwithtangent.x");
	
	D3DXLoadMeshFromX(path.c_str(), D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &m_pMesh);
	assert(m_pMesh && "Loading x file is failed");

	//텍스처 지정
	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/fieldStone/Fieldstone_DM.tga").c_str());
	m_pEffect->SetTexture("DiffuseMapTex", pTex);
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/fieldStone/Fieldstone_SM.tga").c_str());
	m_pEffect->SetTexture("SpecularMapTex", pTex);
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/fieldStone/Fieldstone_NM.tga").c_str());
	m_pEffect->SetTexture("NormalMapTex", pTex);
	
}

void NormalMapping::Update()
{
	if (Camera::Get()->GetCameraMode() != CameraMode::CM_Free) IDisplayObject::UpdateInput();
	IDisplayObject::ApplyInput();
	position = targetPosition;
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();
}

void NormalMapping::Render()
{
	m_pEffect->SetFloatArray("gWorldLightDir", (float*)&Light::WorldLightDir, 3);
	m_pEffect->SetFloatArray("gLightColor", (float*)&Light::LightColor, 3);
	m_pEffect->SetVector("gWorldCamPos", &D3DXVECTOR4(*Camera::Get()->GetPosition(), 1));

	//매트릭스 업데이트
	D3DXMATRIX matVP;
	m_pEffect->SetMatrix("gWorldMatrix", &worldMatrix);
	matVP = *Camera::Get()->GetVPMatrix();
	m_pEffect->SetMatrix("gWVPMatrix", &(worldMatrix * matVP));

	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);
		for ( int j = 0; j < 50; j++)
			m_pMesh->DrawSubset(j);
		m_pEffect->EndPass();
	}

	m_pEffect->End();
}
