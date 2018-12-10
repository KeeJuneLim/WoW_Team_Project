#include "stdafx.h"
#include "AllocateHierarchyHLSL.h"


// 프레임 생성 : 새로 메모리를 할당해서 프레임을 생성하고 ppNewFrame 인자에 연결
// 생성한 프레임의 이름은 모델링 프로그램에서 정해준 프레임의 이름을 인자로 받아 지정
// 프레임 전체를 트리구조로 꾸미기 위한 준비 작업
STDMETHODIMP AllocateHierarchyHLSL::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	FRAME_EX* pFrameEx = new FRAME_EX;
	
	
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


// 매쉬 컨테이너 생성 : 새로 메모리를 할당해서프레임을 생성하고 ppNewMeshContainer 인자에 연결
// 매쉬의 이름, 머티리얼, 텍스처를 지정
// 생성한 매쉬 컨테이너에 pSkinInfo 를 연결
// Skinning 을 위한 준비 작업
STDMETHODIMP AllocateHierarchyHLSL::CreateMeshContainer(
	THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{

	LPD3DXMESH pMesh = NULL;
	MESHCONTAINER_EX* pMeshContainerEx = NULL;
	
	pMesh = pMeshData->pMesh;

	assert(pMeshData->Type == D3DXMESHTYPE_MESH && _T("D3DXMESHTYPE_MESH 타입만 사용 가능"));
	assert(pMesh->GetFVF() > 0 && _T("FVF Flag 가 지정되지 않음"));

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

	UINT NumFaces = pMesh->GetNumFaces();
	pMeshContainerEx->pAdjacency = new DWORD[NumFaces * 3];
	memcpy(pMeshContainerEx->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	
	pMeshContainerEx->NumMaterials = max(1, NumMaterials);

	// if materials provided, copy them
	if (NumMaterials > 0)
	{
		for (int i = 0; i < NumMaterials; i++)
		{
			Material* pMtlTex = new Material;
			pMtlTex->SetMaterial(pMaterials[i].MatD3D);
			pMtlTex->SetDiffuseMapTex(Assets::GetTexture(m_path + pMaterials[i].pTextureFilename));
			pMeshContainerEx->vecMtlTex.push_back(pMtlTex);
		}
	}
	else // if no materials provided, use a default one
	{
		Material* pMtlTex = new Material;
		pMtlTex = (Material*)&DXUtil::WHITE_MTRL;
		pMeshContainerEx->vecMtlTex.push_back(pMtlTex);
	}

	pMeshContainerEx->MeshData.pMesh = pMesh;
	pMeshContainerEx->MeshData.Type = D3DXMESHTYPE_MESH;
	pMesh->AddRef();

	pMeshContainerEx->pOrigMesh = pMesh;
	
	if (pSkinInfo != NULL)
	{
		pMeshContainerEx->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		// pSkinInfo->GetNumBones() : 이 매쉬에 영향을 주는 본(프레임)의 갯수
		DWORD numBones = pSkinInfo->GetNumBones();
		pMeshContainerEx->ppBoneMatrixPtrs = new D3DXMATRIX*[numBones];
		pMeshContainerEx->pBoneOffsetMatrices = new D3DXMATRIX[numBones];
		
		for (DWORD i = 0; i < numBones; ++i)
		{
			pMeshContainerEx->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}

		GenerateSkinnedMesh(DX::GetDevice(), pMeshContainerEx);
	}

	*ppNewMeshContainer = pMeshContainerEx;
	
	return S_OK;
}

void AllocateHierarchyHLSL::GenerateSkinnedMesh(IDirect3DDevice9 * pd3dDevice, MESHCONTAINER_EX * pMeshContainer)
{
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	// Get palette size
	// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
	// (96 - 9) /3 i.e. Maximum constant count - used constants 
	UINT MaxMatrices = 26;
	pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;
	
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
		pMeshContainer->pOrigMesh,
		Flags,
		pMeshContainer->NumPaletteEntries,
		pMeshContainer->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh);

	pMeshContainer->pOrigMesh = pMeshContainer->MeshData.pMesh;

	// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK)
		| D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		LPD3DXMESH pMesh = NULL;
		pMeshContainer->MeshData.pMesh->CloneMeshFVF(
			pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pd3dDevice, &pMesh);
		
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->pOrigMesh = pMesh;
		pMesh = NULL;
	}

	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
	
	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff)
	{
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
	
	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	
	if (NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
	{
		NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
	}
}


//생성했던 프레임을 해제
//생성시 동적 할당 해줬던 부분을 모두 해제
STDMETHODIMP AllocateHierarchyHLSL::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}


//생성했던 매쉬 컨테이너를 해제
//생성시 동적 할당 해줬던 부분을 모두 해제
STDMETHODIMP AllocateHierarchyHLSL::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pMeshContainerToFree;
	if (pMeshContainerEx == NULL)
		return E_FAIL;

	SAFE_RELEASE(pMeshContainerEx->pOrigMesh);
	SAFE_RELEASE(pMeshContainerEx->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainerEx->pSkinInfo);

	for (auto p : pMeshContainerEx->vecMtlTex)
	{
		SAFE_DELETE(p);
	}
	
	SAFE_DELETE_ARRAY(pMeshContainerEx->Name);
	SAFE_DELETE_ARRAY(pMeshContainerEx->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainerEx->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pMeshContainerEx->pBoneOffsetMatrices);
	
	SAFE_DELETE(pMeshContainerToFree);

	return S_OK;
}