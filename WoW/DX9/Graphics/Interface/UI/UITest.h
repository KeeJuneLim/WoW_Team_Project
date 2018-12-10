#pragma once
#include "stdafx.h"

class UITest : public IDisplayObject
{
	/*
	vector<VERTEX_PT> vertices;
	vertices.push_back(VERTEX_PT(D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(0, 1)));
	vertices.push_back(VERTEX_PT(D3DXVECTOR3(0, 1, 0), D3DXVECTOR2(0, 0)));
	vertices.push_back(VERTEX_PT(D3DXVECTOR3(1, 1, 0), D3DXVECTOR2(1, 0)));

	wstring filepath = ASSET_PATH + L"textures/girl.png";
	IDirect3DTexture9* tex = Assets::GetTexture(filepath.c_str());

	//DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	RECT rect;
	GetClientRect(DX::GetInfo().hWnd, &rect);
	static float angleY = 0;
	angleY += 0.02f;
	D3DXMATRIX rotY, matS;
	D3DXMatrixScaling(&matS, 120, 120, 120);
	D3DXMatrixRotationZ(&rotY, angleY);
	for (auto &p : vertices)
		D3DXVec3TransformCoord(&p.p, &p.p, &(matS * rotY));
	D3DXMATRIX matProj, matWorld;
	D3DXMatrixOrthoLH(&matProj, rect.right, rect.bottom, 0.0f, 1.0f);

	D3DXMatrixIdentity(&matWorld);
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTexture(0, tex);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	DX::GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);
	DX::GetDevice()->SetTransform(D3DTS_VIEW, &matWorld);
	DX::GetDevice()->SetFVF(VERTEX_PT::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		vertices.size() / 3, &vertices[0], sizeof(VERTEX_PT));

	DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, true);
	*/
};