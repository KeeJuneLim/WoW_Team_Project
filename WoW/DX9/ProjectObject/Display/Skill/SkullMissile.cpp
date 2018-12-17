#include "stdafx.h"
#include "SkullMissile.h"

SkullMissile::SkullMissile()
{
	shaderType = ST_Lighting;

	skillDir = Objects::GetPlayer()->forward;
	Rendering::Get()->AddRenderingObject(this);

}

SkullMissile::SkullMissile(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	boundary = new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1.3f);
	//boundary = new BoundingBox(D3DXVECTOR3(-1.2, -1.2, -1.2), D3DXVECTOR3(1.2, 1.2, 1.2));
	boundary->InitGeometry();
	position = pos + dir + D3DXVECTOR3(0, 1, 0);
	scale = D3DXVECTOR3(.2, .2, .2);
	material = new Material();

	skillDir = dir;
	shaderType = ST_Shadow;

	rotation = *Objects::GetPlayer()->GetRotation() + D3DXVECTOR3(0, D3DX_PI, 0);

	Rendering::Get()->AddRenderingObject(this);
	Rendering::Get()->AddShadowCastObject(this);
}

SkullMissile::~SkullMissile()
{
	SAFE_RELEASE(skull);

	Rendering::Get()->RemoveRenderingObject(this);
	Rendering::Get()->RemoveShadowCastObject(this);

}

void SkullMissile::Init()
{

	LPCTSTR xPath;

	xPath = _T("../../_Assets/Models/X/Skill/Skull.X");
	if (FAILED(D3DXLoadMeshFromX(xPath, D3DXMESH_MANAGED, DX::GetDevice(), NULL, NULL, NULL, NULL, &skull)))
		assert(false && "Loading x file is failed");

	material = new Material();

	wstring filepath = ASSET_PATH + L"Models/X/Skill/Skull.png";
	material->SetTexture(TT_DiffuseMap, Assets::GetTexture(filepath.c_str()));
	

}

void SkullMissile::Update()
{
	IDisplayObject::UpdateLocalMatrix();
	IDisplayObject::UpdateWorldMatrix();
	boundary->UpdateTransform(worldMatrix);

	for (auto p : children)
	{
		SAFE_UPDATE(p);
	}


	position += skillDir * 1 * Time::Get()->GetDeltaTime();
}

void SkullMissile::Render()
{

}

void SkullMissile::Render(IShader * pShader)
{
	if (Frustum::Get()->IsSphereInsideFrustum(&position, boundary->cullingRadius) == false) return;

	if (boundary->isPicked)
		material->Diffuse = D3DXCOLOR(0, 1, 0, 1);
	else
		material->Diffuse = D3DXCOLOR(1, 1, 1, 1);


	pShader->Render(this);

	//SAFE_RENDER(boundary);

	for (auto p : children)
	{
		SAFE_RENDER(p);
	}

}


void SkullMissile::Draw()
{
	//DrawPrimitive();
	skull->DrawSubset(0);

}
