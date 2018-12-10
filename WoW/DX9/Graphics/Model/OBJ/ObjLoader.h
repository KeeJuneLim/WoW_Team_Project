#pragma once

class ObjLoader
{
private:
	CString					m_filePath;
	map<CString, Material*>	m_mapMtlTex;
	vector<VERTEX_PNT>		vecPNT;

public:
	ObjLoader();
	~ObjLoader();

	LPD3DXMESH	LoadMesh(LPCTSTR filePath, LPCTSTR fileName, D3DXMATRIX* pMat, OUT vector<Material*> &vecMtlTex);
	void		LoadSurface(LPCTSTR fullPath, D3DXMATRIXA16 * pMat, OUT vector<D3DXVECTOR3> &vecVertex);
	void		LoadMtlLib(LPCTSTR fullPath);
	void		CreateSurface(OUT vector<D3DXVECTOR3> &vecVertex);
};

