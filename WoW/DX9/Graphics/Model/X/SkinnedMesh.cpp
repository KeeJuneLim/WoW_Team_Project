#include "stdafx.h"
#include "SkinnedMesh.h"
#include "AllocateHierarchy.h"

SkinnedMesh::SkinnedMesh()
{
	m_scale = Vector3::Create(0.1f);
	m_baseRotation.y = D3DX_PI;

	m_pRootFrame = NULL;
	m_pAC = NULL;
	m_fBlendTime = 0.3f;
	m_fPassedBlendTime = 0.0f;
	m_animIndex = 0;
	m_bWireFrame = false;
	m_bDrawFrame = true;
	m_bDrawSkeleton = false;

	m_pSubRootFrame = NULL;
	m_pSubAC = NULL;
	m_pConnetFrame = NULL;
}

SkinnedMesh::~SkinnedMesh()
{
	SAFE_RELEASE(m_pSphereMesh);

	AllocateHierarchy alloc;
	if (m_pRootFrame)
		D3DXFrameDestroy(m_pRootFrame, &alloc);
	if ( m_pSubRootFrame)
		D3DXFrameDestroy(m_pSubRootFrame, &alloc);
	Objects::RemoveFromTagList(TAG_PLAYER, this);
	Shaders::Get()->RemoveList(this, m_renderMode);
}

void SkinnedMesh::Init()
{
	Objects::AddToTagList(TAG_PLAYER, this);
	m_renderMode = RenderMode::RenderMode_ShadowMapping;
	Shaders::Get()->AddList(this, m_renderMode);
	SetAsCameraTarget();


	D3DXCreateSphere(DX::GetDevice(), 0.01f, 10, 10, &m_pSphereMesh, NULL);
	
	Load(m_pRootFrame, m_pAC, ASSET_PATH + _T("Models/x/zealot/"), _T("zealot.X"));
	//Load(m_pRootFrame, m_pAC, ASSET_PATH + _T("Models/marine/"), _T("marine.X"));
	//Load(m_pRootFrame, m_pAC, ASSET_PATH + _T("Models/SiegeTank/"), _T("SiegeTank.X"));
	//Load(m_pRootFrame, m_pAC, ASSET_PATH + _T("Models/SiegeTank/"), _T("SiegeTank_SiegeMode.X"));
	//Load(m_pRootFrame, m_pAC, ASSET_PATH + _T("Models/SiegeTank/"), _T("SiegeTank_Transform.X"));
	//Load(m_pRootFrame, m_pAC, ASSET_PATH + _T("Models/x/player/"), _T("player.X"));

	//SeparateFrame(m_pRootFrame, "UpperChest", m_pSubAC, m_pAC);
	//SeparateFrame(m_pRootFrame, "Bone_Turret_Base", m_pSubAC, m_pAC);
	SeparateFrame(m_pRootFrame, "BoneSpine02", m_pSubAC, m_pAC);
	
	//if (m_pSubAC) SetAnimationIndex(m_pSubAC, 2, true);
	
	/*
	/////Add Anim Set/////
	LPD3DXFRAME	pTmpRoot = NULL;
	LPD3DXANIMATIONCONTROLLER pTmpAC = NULL;
	Load(pTmpRoot, pTmpAC, ASSET_PATH + _T("Models/marine/"), _T("Marine_Death.X"));
	AddAnimSet(m_pAC, pTmpAC, 0);
	
	AllocateHierarchy alloc;
	D3DXFrameDestroy(pTmpRoot, &alloc);
	/////Add Anim Set/////
	*/
}

void SkinnedMesh::Load(LPD3DXFRAME &pRootFrame, LPD3DXANIMATIONCONTROLLER &pAC, LPCTSTR path, LPCTSTR filename)
{
	AllocateHierarchy alloc(path);

	CString fullPath(path);
	fullPath.Append(filename);

	D3DXLoadMeshHierarchyFromX(fullPath, D3DXMESH_MANAGED, DX::GetDevice(),
		&alloc, NULL, &pRootFrame, &pAC);

	SetupBoneMatrixPointers(pRootFrame);
}

void SkinnedMesh::SeparateFrame(LPD3DXFRAME pFrame, LPCSTR name, 
	LPD3DXANIMATIONCONTROLLER &pAC_Dest, LPD3DXANIMATIONCONTROLLER &pAC_Src)
{
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (CompareStr(pFrame->pFrameFirstChild->Name, name))
		{
			m_pConnetFrame = pFrame;
			m_pSubRootFrame = pFrame->pFrameFirstChild;
			pFrame->pFrameFirstChild = NULL;
		}
	}
	
	if (pFrame->pFrameSibling != NULL)
	{
		if (CompareStr(pFrame->pFrameSibling->Name, name))
		{
			m_pConnetFrame = pFrame;
			m_pSubRootFrame = pFrame->pFrameSibling;
			pFrame->pFrameSibling = NULL;
		}
	}

	if (m_pSubRootFrame)
	{
		pAC_Src->CloneAnimationController(
			m_pAC->GetMaxNumAnimationOutputs(),
			m_pAC->GetMaxNumAnimationSets(),
			m_pAC->GetMaxNumTracks(),
			m_pAC->GetMaxNumEvents(),
			&pAC_Dest);
		return;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		SeparateFrame(pFrame->pFrameSibling, name, pAC_Dest, pAC_Src);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		SeparateFrame(pFrame->pFrameFirstChild, name, pAC_Dest, pAC_Src);
	}
}


void SkinnedMesh::AddAnimSet(LPD3DXANIMATIONCONTROLLER &pAC_Dest, LPD3DXANIMATIONCONTROLLER &pAC_Src, UINT index)
{
	LPD3DXANIMATIONCONTROLLER tmpAC;
	pAC_Dest->CloneAnimationController(
		m_pAC->GetMaxNumAnimationOutputs(),
		m_pAC->GetMaxNumAnimationSets() + 1,
		m_pAC->GetMaxNumTracks(),
		m_pAC->GetMaxNumEvents(),
		&tmpAC);

	LPD3DXANIMATIONSET set;
	pAC_Src->GetAnimationSet(index, &set);
	tmpAC->RegisterAnimationSet(set);

	pAC_Dest = tmpAC;
}


// 각 프레임의 매시 컨테이너에 있는 pSkinInfo를 이용하여 
// 현재 매쉬에 영향을 주는 프레임들의 월드행렬 포인터를 연결 
//Called to setup the pointers for a given bone to its transformation matrix
void SkinnedMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		SetupBoneMatrixPointers(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
	}
}

void SkinnedMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DWORD numBones;
	FRAME_EX* pFrameExInfluence;
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pMeshContainerBase;

	if (pMeshContainerEx->pSkinInfo != NULL)
	{
		numBones = pMeshContainerEx->pSkinInfo->GetNumBones();

		for (DWORD i = 0; i < numBones; ++i)
		{
			pFrameExInfluence = (FRAME_EX*)D3DXFrameFind(m_pRootFrame,
				pMeshContainerEx->pSkinInfo->GetBoneName(i));

			pMeshContainerEx->ppBoneMatrixPtrs[i] = &pFrameExInfluence->CombinedTM;
		}
	}
}


void SkinnedMesh::Update()
{
	/*
	//Anim 별 플레이 시간 체크
	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	D3DXTRACK_DESC track;
	m_pAC->GetTrackDesc(0, &track);
	m_pAC->GetAnimationSet(0, &pCurrAnimSet);
	
	if (pCurrAnimSet->GetPeriodicPosition(track.Position)
		>= pCurrAnimSet->GetPeriod() - 0.02f)
	{
		m_animIndex++;
		SetAnimationIndex(m_pAC, m_animIndex, true);
		SetAnimationIndex(m_pSubAC, m_animIndex, true);
	}
	pCurrAnimSet->Release();
	*/

	Debug::Get()->AddText(_T("Anim Index = "));
	Debug::Get()->AddText((int)m_animIndex + 1);
	Debug::Get()->AddText(_T(" / "));
	Debug::Get()->AddText((int)m_pAC->GetMaxNumAnimationSets());
	Debug::Get()->EndLine();
	Debug::Get()->AddText(m_pAC->GetTime());
	Debug::Get()->EndLine();

	if (Input::KeyDown('1'))
	{
		if (m_animIndex < m_pAC->GetMaxNumAnimationSets() - 1)
			m_animIndex++;

		SetAnimationIndex(m_pAC, m_animIndex, true);
		if (m_pSubAC)
			SetAnimationIndex(m_pSubAC, 2, true);
	}
	else if (Input::KeyDown('2'))
	{
		if (m_animIndex > 0)
			m_animIndex--;
		
		SetAnimationIndex(m_pAC, m_animIndex, true);
		if (m_pSubAC)
			SetAnimationIndex(m_pSubAC, 2, true);
	}
	else if (Input::KeyDown(VK_F1))
	{
		m_bDrawFrame = !m_bDrawFrame;
	}
	else if (Input::KeyDown(VK_F2))
	{
		m_bDrawSkeleton = !m_bDrawSkeleton;
	}
	else if (Input::KeyDown(VK_F3))
	{
		m_bWireFrame = !m_bWireFrame;
	}

	IUnitObject::UpdateInput();
	IUnitObject::UpdatePositionToDestination();

	UpdateAnim(m_pAC);
	UpdateFrameMatrices(m_pRootFrame, NULL);

	if (m_pSubRootFrame)
	{
		FRAME_EX pFrameEx;
		static float angle = 0;
		
		if (Input::KeyPress('E'))
			angle += 0.04f;
		else if (Input::KeyPress('Q'))
			angle -= 0.04f;

		D3DXMATRIXA16 mat;
		D3DXMatrixRotationX(&mat, angle);

		pFrameEx.CombinedTM = mat * ((FRAME_EX*)m_pConnetFrame)->CombinedTM;

		UpdateAnim(m_pSubAC);
		UpdateFrameMatrices(m_pSubRootFrame, (LPD3DXFRAME)&pFrameEx);
	}

}


void SkinnedMesh::UpdateAnim(LPD3DXANIMATIONCONTROLLER pAC)
{
	float fDeltaTime = Time::Get()->GetDeltaTime();
	// AdvanceTime 함수가 호출된 간격으로 Anim 키프레임 계산
	pAC->AdvanceTime(fDeltaTime, NULL);
	
	if (m_fPassedBlendTime <= m_fBlendTime)
	{
		m_fPassedBlendTime += fDeltaTime;

		if (m_fPassedBlendTime < m_fBlendTime)
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;

			pAC->SetTrackWeight(0, fWeight);
			pAC->SetTrackWeight(1, 1.0f - fWeight);
		}
		else
		{
			pAC->SetTrackWeight(0, 1);
			pAC->SetTrackWeight(1, 0);
			pAC->SetTrackEnable(1, false);
		}
	}
	
}


void SkinnedMesh::UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;

	if (pParent != NULL)
	{
		pFrameEx->CombinedTM = pFrameEx->TransformationMatrix * ((FRAME_EX*)pParent)->CombinedTM;
	}
	else
	{
		pFrameEx->CombinedTM = pFrameEx->TransformationMatrix;
	}

	if (pFrameEx->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrameEx->pFrameSibling, pParent);
	}

	if (pFrameEx->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrameEx->pFrameFirstChild, pFrameEx);
	}
}


void SkinnedMesh::Render()
{
	if (m_renderMode != RenderMode_Default) return;

	m_numFrame = 0;
	m_numMesh = 0;
	Debug::Get()->AddText(_T("=====DrawFrame====="));
	Debug::Get()->EndLine();
	if ( m_bDrawFrame)DrawFrame(m_pRootFrame);
	 if (m_pSubRootFrame) if ( m_bDrawFrame)DrawFrame(m_pSubRootFrame);
	
	Debug::Get()->EndLine();
	Debug::Get()->AddText(_T("numFrame = "));
	Debug::Get()->AddText(m_numFrame);
	Debug::Get()->EndLine();
	Debug::Get()->AddText(_T("numMesh = "));
	Debug::Get()->AddText(m_numMesh);
	Debug::Get()->EndLine();
	if ( m_bDrawSkeleton)DrawSkeleton(m_pRootFrame, NULL);	
}

// Desc: Called to render a frame in the hierarchy
void SkinnedMesh::DrawFrame(LPD3DXFRAME pFrame)
{
	m_numFrame++;
	if (m_numFrame % 10 == 0)
	{
		Debug::Get()->EndLine();
	}
	if (pFrame->Name == NULL)
		Debug::Get()->AddText(_T("NULL"));
	else
		Debug::Get()->AddText(pFrame->Name);
	

	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		m_numMesh++;
		Debug::Get()->AddText(_T("(MESH)"));
		DrawMeshContainer(pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	Debug::Get()->AddText(_T(" / "));
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void SkinnedMesh::DrawMeshContainer(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer->pSkinInfo == NULL)
		return;
	
	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pFrameEx->pMeshContainer;
	DWORD numBones = pMeshContainerEx->pSkinInfo->GetNumBones();

	for (DWORD i = 0; i < numBones; ++i)
	{
		pMeshContainerEx->pFinalBoneMatrices[i] =
			pMeshContainerEx->pBoneOffsetMatrices[i] * *(pMeshContainerEx->ppBoneMatrixPtrs[i]);
	}

	PBYTE pVerticesSrc = NULL;
	PBYTE pVerticesDest = NULL;

	pMeshContainerEx->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);
	pMeshContainerEx->pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

	// generate skinned mesh
	pMeshContainerEx->pSkinInfo->UpdateSkinnedMesh(
		pMeshContainerEx->pFinalBoneMatrices, NULL, pVerticesSrc, pVerticesDest);

	pMeshContainerEx->pWorkMesh->UnlockVertexBuffer();
	pMeshContainerEx->pOrigMesh->UnlockVertexBuffer();

	
	if (m_bWireFrame) DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	
	//RenderUseShader(pFrame); return;
	if (m_renderMode == RenderMode::RenderMode_Default)
	{
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_worldMatrix);

		for (size_t i = 0; i < pMeshContainerEx->vecMtlTex.size(); ++i)
		{
			DX::GetDevice()->SetMaterial(&pMeshContainerEx->vecMtlTex[i]->material);
			DX::GetDevice()->SetTexture(0, pMeshContainerEx->vecMtlTex[i]->pTexture);
			pMeshContainerEx->pWorkMesh->DrawSubset(i);
		}
	}
	else if (m_renderMode == RenderMode::RenderMode_Lighting)
	{
		for (size_t i = 0; i < pMeshContainerEx->vecMtlTex.size(); ++i)
		{
			Shaders::Get()->GetCurrentShader()->SetWorldMatrix(&m_worldMatrix);
			Shaders::Get()->GetCurrentShader()->SetTexture(pMeshContainerEx->vecMtlTex[i]->pTexture);
			Shaders::Get()->GetCurrentShader()->SetMaterial(&pMeshContainerEx->vecMtlTex[i]->material);
			Shaders::Get()->GetCurrentShader()->Commit();
			pMeshContainerEx->pWorkMesh->DrawSubset(i);
		}
	}
	else if (m_renderMode == RenderMode::RenderMode_ShadowMapping)
	{
		for (size_t i = 0; i < pMeshContainerEx->vecMtlTex.size(); ++i)
		{
			Shaders::Get()->GetCurrentShader()->SetWorldMatrix(&m_worldMatrix);
			Shaders::Get()->GetCurrentShader()->SetTexture(pMeshContainerEx->vecMtlTex[i]->pTexture);
			Shaders::Get()->GetCurrentShader()->SetMaterial(&pMeshContainerEx->vecMtlTex[i]->material);
			Shaders::Get()->GetCurrentShader()->Commit();
			pMeshContainerEx->pWorkMesh->DrawSubset(i);
		}
	}
	
	DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void SkinnedMesh::DrawSkeleton(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
	FRAME_EX* pParentFrameEx = (FRAME_EX*)pParent;

	DX::GetDevice()->SetTransform(D3DTS_WORLD, &(pFrameEx->CombinedTM));
	//DX::GetDevice()->SetTransform(D3DTS_WORLD, &(pFrameEx->CombinedTM * m_worldMatrix));
	
	DX::GetDevice()->SetMaterial(&DXUtil::WHITE_MTRL);
	DX::GetDevice()->SetTexture(0, NULL);
	m_pSphereMesh->DrawSubset(0);

	if (pParent != NULL && pFrame->Name != NULL && pParent->Name != NULL)
	{
		D3DXMATRIXA16 matThis = pFrameEx->CombinedTM;
		D3DXMATRIXA16 matParent = pParentFrameEx->CombinedTM;

		D3DXVECTOR3 posThis(matThis(3, 0), matThis(3, 1), matThis(3, 2));
		D3DXVECTOR3 posParent(matParent(3, 0), matParent(3, 1), matParent(3, 2));

		vector<VERTEX_PC> line{ VERTEX_PC(posThis, BLUE), VERTEX_PC(posParent, YELLOW) };
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		DX::GetDevice()->SetFVF(VERTEX_PC::FVF);
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity(&mat);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &mat);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 1, &line[0], sizeof(VERTEX_PC));
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawSkeleton(pFrame->pFrameSibling, pParent);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawSkeleton(pFrame->pFrameFirstChild, pFrame);
	}
}

void SkinnedMesh::RenderUseShader(LPD3DXFRAME pFrame)
{
	
	//쉐이더 지정
	LPD3DXEFFECT m_pEffect = NULL;
	m_pEffect = Shaders::Get()->GetShader(SHADER_PATH + _T("Lighting.fx"));

	//광원 위치 업데이트
	static float angle = -D3DX_PI / 6;
	//angle += 0.02f * 0.1f;
	float radius = 500;
	D3DXVECTOR4 lightPos(cosf(angle) * radius, radius, sinf(angle) * radius, 1);
	m_pEffect->SetVector("gWorldLightPos", &lightPos);

	//카메라 위치 업데이트
	D3DXVECTOR4 camPos(Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPos", &camPos);

	//매트릭스 업데이트
	m_pEffect->SetMatrix("gWorldMatrix", &m_worldMatrix);
	m_pEffect->SetMatrix("gViewMatrix", Camera::Get()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", Camera::Get()->GetProjMatrix());


	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pFrameEx->pMeshContainer;
	
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (int i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);

		for (size_t i = 0; i < pMeshContainerEx->vecMtlTex.size(); i++)
		{
			m_pEffect->SetTexture("DiffuseMap_Tex", pMeshContainerEx->vecMtlTex[i]->pTexture);
			m_pEffect->CommitChanges();
			pMeshContainerEx->pWorkMesh->DrawSubset(i);
		}
		m_pEffect->EndPass();
	}

	m_pEffect->End();
	
}

void SkinnedMesh::RenderUseShader_0()
{
	if (m_bDrawFrame)DrawFrame(m_pRootFrame);
}

void SkinnedMesh::RenderUseShader_1()
{
	if (m_bDrawFrame)DrawFrame(m_pRootFrame);
	if (m_pSubRootFrame) if (m_bDrawFrame)DrawFrame(m_pSubRootFrame);
}


void SkinnedMesh::SetAnimationIndex(LPD3DXANIMATIONCONTROLLER pAC, int nIndex, bool isBlend)
{
	LPD3DXANIMATIONSET pNextAnimSet = NULL;
	pAC->GetAnimationSet(nIndex, &pNextAnimSet);

	if (isBlend)
	{
		LPD3DXANIMATIONSET pPrevAnimSet = NULL;

		pAC->GetTrackAnimationSet(0, &pPrevAnimSet);
		pAC->SetTrackAnimationSet(1, pPrevAnimSet);

		D3DXTRACK_DESC trackDesc;
		pAC->GetTrackDesc(0, &trackDesc);
		pAC->SetTrackDesc(1, &trackDesc);
		
		pAC->SetTrackWeight(0, 0.0f);
		pAC->SetTrackWeight(1, 1.0f);

		SAFE_RELEASE(pPrevAnimSet);

		m_fPassedBlendTime = 0.0f;
	}

	pAC->SetTrackAnimationSet(0, pNextAnimSet);
	//pAC->ResetTime();
	pAC->SetTrackPosition(0, 0);
	SAFE_RELEASE(pNextAnimSet);
}
