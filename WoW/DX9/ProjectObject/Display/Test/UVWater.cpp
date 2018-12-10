#include "stdafx.h"
#include "UVWater.h"


UVWater::UVWater()
{
	Rendering::Get()->AddRenderingObject(this);

	position = D3DXVECTOR3(50, 10, 50);
	scale = D3DXVECTOR3(50, 50, 50);
	rotation.x = D3DX_PI * 0.5f;
}


UVWater::~UVWater()
{
	Rendering::Get()->RemoveRenderingObject(this);
}


void UVWater::Init()
{
	m_pEffect = Shaders::Get()->GetEffect((SHADER_PATH + L"UVWater.fx").c_str());
	
	D3DXVECTOR3 normal(0, 0, -1);
	for (int i = 0; i < g_vecQuadIndex.size(); i++)
	{
		m_vertices.push_back(VertexType(
			g_vecQuadVertex[g_vecQuadIndex[i]],
			normal,
			g_vecQuadUV[g_vecQuadIndex[i]])
		);
	}
	
	LPDIRECT3DTEXTURE9 pTex = NULL;
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/sun.png").c_str());
	m_pEffect->SetTexture("DiffuseMapTex", pTex);
	pTex = Assets::GetTexture((ASSET_PATH + L"shader/water-nm.jpg").c_str());
	m_pEffect->SetTexture("DiffuseMap1Tex", pTex);
}

void UVWater::Update()
{
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();

}

void UVWater::Render()
{
	

}

void UVWater::Render(IShader * pShader)
{
	m_pEffect->SetFloatArray("gWorldLightDir", (float*)&Light::WorldLightDir, 3);
	m_pEffect->SetFloatArray("gLightColor", (float*)&Light::LightColor, 3);
	m_pEffect->SetVector("gWorldCamPos", &D3DXVECTOR4(*Camera::Get()->GetPosition(), 1));

	m_pEffect->SetMatrix("gWorldMatrix", &worldMatrix);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjectionMatrix());

	m_pEffect->SetFloat("gTime", GetTickCount() / 1000.0);

	static float angle = 0;
	angle += 0.02f;
	D3DXMATRIX mat, matS;
	D3DXMatrixRotationZ(&mat, angle);

	m_pEffect->SetMatrix("rotMat", &(mat));

	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DX::GetDevice()->SetFVF(VertexType::FVF);

	m_pEffect->Begin(0, 0);
	m_pEffect->BeginPass(0);

	m_pEffect->CommitChanges();
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vertices[0], sizeof(VertexType));

	m_pEffect->EndPass();
	m_pEffect->End();

	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
