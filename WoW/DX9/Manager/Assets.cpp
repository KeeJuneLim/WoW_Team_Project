#include "stdafx.h"
#include "Assets.h"

map<CString, LPDIRECT3DTEXTURE9> Assets::m_pTextureList;
map<Assets::FontType, LPD3DXFONT> Assets::m_pFontList;
map<CString, D3DXIMAGE_INFO> Assets::m_imageInfoList;
map<CString, SkinnedMeshHLSL*> Assets::m_pSkinnedMeshList;

void Assets::Delete()
{
	for (auto p : m_pTextureList)
	{
		SAFE_RELEASE(p.second);
	}
	m_pTextureList.clear();

	for (auto p : m_pFontList)
	{
		SAFE_RELEASE(p.second);
	}
	m_pFontList.clear();

	for (auto p : m_pSkinnedMeshList)
	{
		p.second->Delete();
		SAFE_DELETE(p.second);
	}
	m_pFontList.clear();

}


LPDIRECT3DTEXTURE9 Assets::GetTexture(LPCTSTR filePath, D3DXIMAGE_INFO* info)
{
	if (lstrlen(filePath) == 0) return NULL;

	if (info == NULL)
	{
		if (m_pTextureList.find(filePath) == m_pTextureList.end())
		{
			if (FAILED(D3DXCreateTextureFromFile(DX::GetDevice(), filePath, &m_pTextureList[filePath])));
				//assert(false && "FAILED(D3DXCreateTextureFromFile)");
		}
	}
	else
	{
		if (m_pTextureList.find(filePath) == m_pTextureList.end())
		{
			if (FAILED(D3DXCreateTextureFromFileEx(DX::GetDevice(), filePath, 
				D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, &m_imageInfoList[filePath], NULL, &m_pTextureList[filePath])))
				assert(false && "FAILED(D3DXCreateTextureFromFileEx)");
		}
		*info = m_imageInfoList[filePath];
	}
	
	return m_pTextureList[filePath];
}


LPD3DXFONT Assets::GetFont(FontType type)
{
	if (m_pFontList.find(type) == m_pFontList.end())
	{
		if (type == FT_NORMAL)
		{
			if (FAILED(D3DXCreateFont(DX::GetDevice(), 24, 12, FW_NORMAL, 1, false, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("consolas"), &m_pFontList[type])))
				assert(false && "FAILED(D3DXCreateFont)");
		}
		else if (type == FT_QUEST)
		{
			AddFontResource(_T("../../_assets/fonts/umberto.ttf"));
			if (FAILED(D3DXCreateFont(DX::GetDevice(), 24, 12, FW_NORMAL, 1, false, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("umberto"), &m_pFontList[type])))
				assert(false && "FAILED(D3DXCreateFont)");
		}
		else if (type == FT_NORMAL2)
		{
			if (FAILED(D3DXCreateFont(DX::GetDevice(), 16, 8, FW_NORMAL, 1, false, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("consolas"), &m_pFontList[type])))
				assert(false && "FAILED(D3DXCreateFont)");
		}
	}

	return m_pFontList[type];
}


SkinnedMeshHLSL* Assets::GetSkinnedMesh(LPCTSTR path, LPCTSTR filename)
{
	CString fullPath(path);
	fullPath.Append(filename);

	if (m_pSkinnedMeshList.find(fullPath) == m_pSkinnedMeshList.end())
	{
		SkinnedMeshHLSL *pMesh = new SkinnedMeshHLSL();
		pMesh->Load(path, filename);
		m_pSkinnedMeshList[fullPath] = pMesh;
	}
	return m_pSkinnedMeshList[fullPath];
}

