#include "stdafx.h"
#include "AllocateHierarchy.h"


// ������ ���� : ���� �޸𸮸� �Ҵ��ؼ� �������� �����ϰ� ppNewFrame ���ڿ� ����
// ������ �������� �̸��� �𵨸� ���α׷����� ������ �������� �̸��� ���ڷ� �޾� ����
// ������ ��ü�� Ʈ�������� �ٹ̱� ���� �غ� �۾�
STDMETHODIMP AllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	FRAME_EX* pFrameEx = new FRAME_EX;
	
	// TODO : �̸��� �� �������ּ���. ���� ������.
	if (Name != NULL)
	{
		pFrameEx->Name = new char[strlen(Name) + 1];
		strcpy_s(pFrameEx->Name, (strlen(Name) + 1), Name);
	}
	else
	{
		pFrameEx->Name = NULL;
	}

	D3DXMatrixIdentity(&pFrameEx->TransformationMatrix);
	D3DXMatrixIdentity(&pFrameEx->CombinedTM);

	pFrameEx->pMeshContainer = NULL;
	pFrameEx->pFrameFirstChild = NULL;
	pFrameEx->pFrameSibling = NULL;

	*ppNewFrame = pFrameEx;
	
	

	return S_OK;
}


// �Ž� �����̳� ���� : ���� �޸𸮸� �Ҵ��ؼ��������� �����ϰ� ppNewMeshContainer ���ڿ� ����
// �Ž��� �̸�, ��Ƽ����, �ؽ�ó�� ����
// ������ �Ž� �����̳ʿ� pSkinInfo �� ����
// Skinning �� ���� �غ� �۾�
STDMETHODIMP AllocateHierarchy::CreateMeshContainer(
	THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	MESHCONTAINER_EX* pMeshContainerEx = NULL;
	LPD3DXMESH pMesh = NULL;

	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		assert(false && _T("D3DXMESHTYPE_MESH Ÿ�Ը� ��� ����"));
	}

	pMesh = pMeshData->pMesh;

	if (pMesh->GetFVF() == 0)
	{
		assert(false && _T("FVF Flag �� �������� ����"));
	}

	pMeshContainerEx = new MESHCONTAINER_EX;
	memset(pMeshContainerEx, 0, sizeof(MESHCONTAINER_EX));
	
	if (Name != NULL)
	{
		pMeshContainerEx->Name = new char[strlen(Name) + 1];
		strcpy_s(pMeshContainerEx->Name, (strlen(Name) + 1), Name);
	}
	
	// if no normals are in the mesh, add them
	if ((pMesh->GetFVF() & D3DFVF_NORMAL) == false)
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL, DX::GetDevice(), &pMesh);

		D3DXComputeNormals(pMesh, NULL);
	}

	SAFE_ADDREF(pMesh);
	pMeshContainerEx->pOrigMesh = pMesh;
	pMeshContainerEx->pOrigMesh->CloneMeshFVF(
		pMeshContainerEx->pOrigMesh->GetOptions(),
		pMeshContainerEx->pOrigMesh->GetFVF(),
		DX::GetDevice(),
		&pMeshContainerEx->pWorkMesh);

	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		MTLTEX* pMtlTex = new MTLTEX;
		pMtlTex->material = pMaterials[i].MatD3D;
		pMtlTex->pTexture = Assets::GetTexture(m_path + pMaterials[i].pTextureFilename);
		pMeshContainerEx->vecMtlTex.push_back(pMtlTex);
	}

	// pSkinInfo �� ������ ����
	if (pSkinInfo != NULL)
	{
		pMeshContainerEx->pSkinInfo = pSkinInfo;
		SAFE_ADDREF(pSkinInfo);

		// pSkinInfo->GetNumBones() : �� �Ž��� ������ �ִ� ��(������)�� ����
		DWORD numBones = pSkinInfo->GetNumBones();
		pMeshContainerEx->ppBoneMatrixPtrs = new D3DXMATRIX*[numBones];
		pMeshContainerEx->pBoneOffsetMatrices = new D3DXMATRIX[numBones];
		pMeshContainerEx->pFinalBoneMatrices = new D3DXMATRIX[numBones];

		for (DWORD i = 0; i < numBones; ++i)
		{
			pMeshContainerEx->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	*ppNewMeshContainer = pMeshContainerEx;
	
	return S_OK;
}


//�����ߴ� �������� ����
//������ ���� �Ҵ� ����� �κ��� ��� ����
STDMETHODIMP AllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}


//�����ߴ� �Ž� �����̳ʸ� ����
//������ ���� �Ҵ� ����� �κ��� ��� ����
STDMETHODIMP AllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pMeshContainerToFree;
	if (pMeshContainerEx == NULL)
		return E_FAIL;

	
	SAFE_RELEASE(pMeshContainerEx->pOrigMesh);
	SAFE_RELEASE(pMeshContainerEx->pWorkMesh);
	SAFE_RELEASE(pMeshContainerEx->pSkinInfo);

	for (auto p : pMeshContainerEx->vecMtlTex)
	{
		SAFE_DELETE(p);
	}

	SAFE_DELETE_ARRAY(pMeshContainerEx->Name);
	SAFE_DELETE_ARRAY(pMeshContainerEx->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pMeshContainerEx->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pMeshContainerEx->pFinalBoneMatrices);

	SAFE_DELETE(pMeshContainerToFree);

	return S_OK;
}