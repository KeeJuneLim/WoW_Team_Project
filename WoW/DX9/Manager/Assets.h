#pragma once

//프로젝트 전체의 리소스를 관리

class SkinnedMeshHLSL;

class Assets
{
public:
	enum FontType
	{
		FT_NORMAL,
		FT_QUEST,
		FT_NORMAL2
	};

	static void	Delete();

	/////Texture/////
private:
	static map<CString, LPDIRECT3DTEXTURE9>	m_pTextureList;
	static map<CString, D3DXIMAGE_INFO>	m_imageInfoList;
public:
	static LPDIRECT3DTEXTURE9 GetTexture(LPCTSTR filePath, D3DXIMAGE_INFO* info = NULL);
	/////Texture/////
	
	/////Font/////
private:
	static map<FontType, LPD3DXFONT> m_pFontList;
public:
	static LPD3DXFONT GetFont(FontType e);
	/////Font/////

	/////SkinnedMesh/////
private:
	static map<CString, SkinnedMeshHLSL*> m_pSkinnedMeshList;
public:
	static SkinnedMeshHLSL* GetSkinnedMesh(LPCTSTR path, LPCTSTR filename);
	/////SkinnedMesh/////
};

