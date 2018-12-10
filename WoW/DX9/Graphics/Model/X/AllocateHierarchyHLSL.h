#pragma once

struct FRAME_EX : public D3DXFRAME
{
	
	D3DXMATRIXA16 CombinedTM;	//root 부터 자신 프레임 까지의 누적된 행렬정보
};

struct MESHCONTAINER_EX : public D3DXMESHCONTAINER
{
	
	vector<Material*>	vecMtlTex;
	LPD3DXMESH		pOrigMesh;				// 수정하지 않은 원본 매쉬(Update 마다 원본값을 참조해 Anim 연산)
	D3DXMATRIX**	ppBoneMatrixPtrs;		// 이 매쉬에 영향을 주는 프레임들의 월드매트릭스 포인터의 배열
	D3DXMATRIX*		pBoneOffsetMatrices;	// 원본 메쉬를 각 프레임의 로컬스페이스로 보내는 매트릭스 배열
	
	// SkinMesh info            
	DWORD NumPaletteEntries;
	DWORD NumInfl;
	DWORD NumAttributeGroups;
	LPD3DXBUFFER pBoneCombinationBuf;

};



class AllocateHierarchyHLSL : public ID3DXAllocateHierarchy
{
protected:
	CString m_path;
	
public:
	AllocateHierarchyHLSL() {};
	AllocateHierarchyHLSL(CString path):NumBoneMatricesMax(0) { m_path = path; }
	~AllocateHierarchyHLSL() {}

	void GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, MESHCONTAINER_EX* pMeshContainer);
	
	VARIABLE(UINT, NumBoneMatricesMax, NumBoneMatricesMax);

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;

	
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;


	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

