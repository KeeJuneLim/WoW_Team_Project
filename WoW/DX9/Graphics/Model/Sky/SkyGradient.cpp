#include "stdafx.h"
#include "SkyGradient.h"

SkyGradient::SkyGradient()
{
	material = new Material();

	shaderType = ST_SkyGradient;
	Rendering::Get()->AddRenderingObject(this);
}

SkyGradient::~SkyGradient()
{
	SAFE_RELEASE(mesh);
	Rendering::Get()->RemoveRenderingObject(this);
}

void SkyGradient::Init()
{
	wstring filepath = ASSET_PATH + L"Models/X/Sphere/Sphere.x";
	D3DXCreateSphere(DX::GetDevice(), 6.0f, 10, 10, &mesh, NULL);
}

void SkyGradient::Update()
{
	
}

void SkyGradient::Render()
{
	
}

void SkyGradient::Render(IShader * pShader)
{
	position = *Camera::Get()->GetPosition();

	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();

	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	pShader->Render(this);

	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void SkyGradient::Draw()
{
	mesh->DrawSubset(0);
}
