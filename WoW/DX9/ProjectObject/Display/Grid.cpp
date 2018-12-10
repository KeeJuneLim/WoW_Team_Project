#include "stdAfx.h"
#include "Grid.h"

Grid::Grid()
{
	D3DXMatrixIdentity(&localMatrix);
	D3DXMatrixIdentity(&worldMatrix);
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
}

Grid::~Grid()
{

}

void Grid::Init()
{
	float interval = 1.0f;
	int numHalfLine = 15;
	float halfLength = interval * numHalfLine;
	D3DCOLOR color;
	float tmpInterval;
	for (int i = 1; i <= numHalfLine; ++i)
	{
		tmpInterval = interval * i;
		color = (i % 5 == 0) ? D3DCOLOR_XRGB(255, 255, 255) : D3DCOLOR_XRGB(128, 128, 128);
		vertices.push_back(VertexType(D3DXVECTOR3(-halfLength, 0.0f, tmpInterval), color));
		vertices.push_back(VertexType(D3DXVECTOR3(halfLength, 0.0f, tmpInterval), color));
		vertices.push_back(VertexType(D3DXVECTOR3(-halfLength, 0.0f, -tmpInterval), color));
		vertices.push_back(VertexType(D3DXVECTOR3(halfLength, 0.0f, -tmpInterval), color));
		vertices.push_back(VertexType(D3DXVECTOR3(tmpInterval, 0.0f, -halfLength), color));
		vertices.push_back(VertexType(D3DXVECTOR3(tmpInterval, 0.0f, halfLength), color));
		vertices.push_back(VertexType(D3DXVECTOR3(-tmpInterval, 0.0f, -halfLength), color));
		vertices.push_back(VertexType(D3DXVECTOR3(-tmpInterval, 0.0f, halfLength), color));
	}
	color = D3DCOLOR_XRGB(255, 0, 0);
	vertices.push_back(VertexType(D3DXVECTOR3(-halfLength, 0.0f, 0.0f), color));
	vertices.push_back(VertexType(D3DXVECTOR3(halfLength, 0.0f, 0.0f), color));

	color = D3DCOLOR_XRGB(0, 255, 0);
	vertices.push_back(VertexType(D3DXVECTOR3(0.0f, -halfLength, 0.0f), color));
	vertices.push_back(VertexType(D3DXVECTOR3(0.0f, halfLength, 0.0f), color));

	color = D3DCOLOR_XRGB(0, 0, 255);
	vertices.push_back(VertexType(D3DXVECTOR3(0.0f, 0.0f, -halfLength), color));
	vertices.push_back(VertexType(D3DXVECTOR3(0.0f, 0.0f, halfLength), color));
}

void Grid::Update()
{
	D3DXMATRIXA16 matT, matR, matS;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&matR, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&matT, position.x, position.y, position.z);
	worldMatrix = matS * matR * matT;
}

void Grid::Render()
{
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &worldMatrix);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->SetFVF(VertexType::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 
		vertices.size() / 2, &vertices[0], sizeof(VertexType));
}
