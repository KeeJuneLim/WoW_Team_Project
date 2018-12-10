#include "stdafx.h"
#include "RectObject.h"

RectObject::RectObject()
{
	D3DXMatrixIdentity(&localMatrix);
	D3DXMatrixIdentity(&worldMatrix);
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	useBuffer = true;
}

RectObject::~RectObject()
{
	SAFE_RELEASE(vb);
	SAFE_RELEASE(ib);
}

void RectObject::Init()
{
	vertices.push_back(VertexType(D3DXVECTOR3(-1, -1, 0), Color::BLUE));
	vertices.push_back(VertexType(D3DXVECTOR3(-1,  1, 0), Color::YELLOW));
	vertices.push_back(VertexType(D3DXVECTOR3( 1,  1, 0), Color::WHITE));
	vertices.push_back(VertexType(D3DXVECTOR3( 1, -1, 0), Color::RED));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	
	DXUtil::CreateVertexBuffer(vb, (LPVOID)&vertices[0], VertexType::FVF,
		vertices.size(), sizeof(VertexType));
	DXUtil::CreateIndexBuffer(ib, (LPVOID)&indices[0], indices.size(), sizeof(indices[0]));
}

void RectObject::Update()
{
	D3DXMATRIXA16 matT, matR, matS;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&matR, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);
	worldMatrix = matS * matR * matT;
}

void RectObject::Render()
{
	ImGui::Begin("RectObject");
	ImGui::SliderFloat3("Position", (float*)&position, -10, 10);
	ImGui::SliderFloat3("Rotation", (float*)&rotation, -D3DX_PI, D3DX_PI);
	ImGui::SliderFloat3("Scaling", (float*)&scale, 0.1f, 10);
	ImGui::End();

	DX::GetDevice()->SetTransform(D3DTS_WORLD, &worldMatrix);
	DX::GetDevice()->SetFVF(VertexType::FVF);
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	if (useBuffer)
	{
		DX::GetDevice()->SetStreamSource(0, vb, 0, sizeof(VertexType));
		DX::GetDevice()->SetIndices(ib);
		DX::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0, vertices.size(), 0, indices.size() / 3);
	}
	else
	{
		DX::GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
			0, vertices.size(), indices.size() / 3,
			&indices[0], D3DFMT_INDEX32, &vertices[0], sizeof(VertexType));
	}
}
