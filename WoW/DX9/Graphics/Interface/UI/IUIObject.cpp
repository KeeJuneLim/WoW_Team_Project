#include "stdafx.h"
#include "IUIObject.h"

IUIObject::IUIObject(LPD3DXSPRITE pSprite)
	: m_bDrawBorder(false)
	, m_pSprite(pSprite)
	, m_color(D3DXCOLOR(1, 1, 1, 1))
{
}

IUIObject::~IUIObject()
{
}

void IUIObject::Init()
{
}

void IUIObject::Update()
{
	UpdateCombinedPosition();

	for (auto p : children)
	{
		p->Update();
	}
}


void IUIObject::Render()
{
	if (m_bDrawBorder) DrawBorder();

	for (auto p : children)
	{
		p->Render();
	}
}


void IUIObject::UpdateCombinedPosition()
{
	m_combinedPos = position;

	if (parent)
	{
		m_combinedPos += *static_cast<IUIObject*>(parent)->GetCombinedPosition();
	}
}


void IUIObject::DrawBorder()
{
	RECT rect;
	GetBorderRect(&rect);
	
	vector<VERTEX_RHWC> vecVertex;
	vecVertex.reserve(5);
	D3DCOLOR c = Color::WHITE;
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(rect.left, rect.top, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(rect.right, rect.top, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(rect.right, rect.bottom, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(rect.left, rect.bottom, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(rect.left, rect.top, 0, 1), c));

	DX::GetDevice()->SetFVF(VERTEX_RHWC::FVF);
	DX::GetDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP, vecVertex.size() - 1, 
		&vecVertex[0], sizeof(VERTEX_RHWC));
}

void IUIObject::GetBorderRect(RECT *rect)
{
	D3DXMATRIXA16 mat;
	m_pSprite->GetTransform(&mat);
	int left = mat._41 + m_combinedPos.x * mat._11;
	int top = mat._42 + m_combinedPos.y * mat._22;
	int right = left + m_size.x * mat._11;
	int bottom = top + m_size.y * mat._22;

	SetRect(rect, left, top, right, bottom);
}


