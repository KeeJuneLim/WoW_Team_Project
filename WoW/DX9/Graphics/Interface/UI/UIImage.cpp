#include "stdAfx.h"
#include "UIImage.h"


UIImage::UIImage(LPD3DXSPRITE pSprite)
	: IUIObject(pSprite)
	, m_pTex(NULL)
{
}

UIImage::~UIImage(void)
{
}

void UIImage::Render()
{
	if (m_pTex)
	{
		RECT rect;
		SetRect(&rect, 0, 0, m_size.x, m_size.y);
		m_pSprite->Draw(m_pTex, &rect, &m_pivot, &m_combinedPos, m_color);
	}

	IUIObject::Render();
}

void UIImage::SetTexture(LPCTSTR filename)
{
	D3DXIMAGE_INFO imageInfo;
	m_pTex = Assets::GetTexture(filename, &imageInfo);

	m_size.x = imageInfo.Width;
	m_size.y = imageInfo.Height;
}


void UIImage::SetTexture(LPDIRECT3DTEXTURE9 pTex, UINT width, UINT height)
{
	m_pTex = pTex;

	m_size.x = width;
	if ( height == 0)
		m_size.y = width;
	else
		m_size.y = height;
	
}