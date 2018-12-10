#pragma once

struct FRAME_EX : public D3DXFRAME
{
	
	D3DXMATRIXA16 CombinedTM;	//root ���� �ڽ� ������ ������ ������ �������
};

struct MESHCONTAINER_EX : public D3DXMESHCONTAINER
{
	vector<MTLTEX*>	vecMtlTex;
	LPD3DXMESH		pOrigMesh;				// �������� ���� ���� �Ž�(Update ���� �������� ������ Anim ����)
	LPD3DXMESH		pWorkMesh;				// Anim �� ������ ���� ������ �Ž�
	D3DXMATRIX**	ppBoneMatrixPtrs;		// �� �Ž��� ������ �ִ� �����ӵ��� �����Ʈ���� �������� �迭
	D3DXMATRIX*		pBoneOffsetMatrices;	// ���� �޽��� �� �������� ���ý����̽��� ������ ��Ʈ���� �迭
	D3DXMATRIX*		pFinalBoneMatrices;		// �� ���� ���� �����Ʈ���� �迭
};

class AllocateHierarchy : public ID3DXAllocateHierarchy
{
protected:
	CString m_path;

public:
	AllocateHierarchy() {}
	AllocateHierarchy(CString path) { m_path = path; }
	~AllocateHierarchy() {}


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

