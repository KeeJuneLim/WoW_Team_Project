#include "stdafx.h"
#include "Sky.h"

Sky::Sky()
{
	mesh = NULL;

	material = new Material();

	shaderType = ST_Sky;
	Rendering::Get()->AddRenderingObject(this);
}

Sky::~Sky()
{
	Rendering::Get()->RemoveRenderingObject(this);
	SAFE_RELEASE(mesh);
}

void Sky::Init()
{
	wstring filepath = ASSET_PATH + L"Models/X/Sphere/Sphere.x";
	D3DXLoadMeshFromX(filepath.c_str(), 0, DX::GetDevice(), 0, 0, 0, 0, &mesh);
}

void Sky::Update()
{
	
}

void Sky::Render()
{
	DX::GetDevice()->SetVertexShader(NULL);
	DX::GetDevice()->SetPixelShader(NULL);

	position = *Camera::Get()->GetPosition();
	UpdateLocalMatrix();
	UpdateWorldMatrix();

	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	DX::GetDevice()->SetTransform(D3DTS_WORLD, &worldMatrix);
	DX::GetDevice()->SetTexture(0, material->GetTexture(TT_DiffuseMap));
	Draw();

	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Sky::Draw()
{
	mesh->DrawSubset(0);
}
