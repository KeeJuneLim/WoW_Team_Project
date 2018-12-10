#include "stdafx.h"
#include "SkinnedMeshHLSL.h"
#include "AllocateHierarchyHLSL.h"

SkinnedMeshHLSL::SkinnedMeshHLSL()
	:m_pRootFrame(NULL), m_pAC(NULL), m_pSubRootFrame(NULL), m_pSubAC(NULL), m_pConnetFrame(NULL)
{
}

SkinnedMeshHLSL::SkinnedMeshHLSL(LPCTSTR path, LPCTSTR filename)
	:m_pRootFrame(NULL), m_pAC(NULL), m_fBlendTime(0.3f), m_fPassedBlendTime(0),
	m_animIndex(0), m_bWireFrame(false), m_bDrawFrame(true), m_bDrawSkeleton(false),
	m_pSubRootFrame(NULL), m_pSubAC(NULL), m_pConnetFrame(NULL) 
{
	SkinnedMeshHLSL* pSkinnedMesh = Assets::GetSkinnedMesh(path, filename);

	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	NumBoneMatricesMax = pSkinnedMesh->NumBoneMatricesMax;
	g_pBoneMatrices = pSkinnedMesh->g_pBoneMatrices;
	m_pEffect = pSkinnedMesh->m_pEffect;

	pSkinnedMesh->m_pAC->CloneAnimationController(
		pSkinnedMesh->m_pAC->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAC->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAC->GetMaxNumTracks(),
		pSkinnedMesh->m_pAC->GetMaxNumEvents(),
		&m_pAC);

	//Load(path, filename);
	scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	baseRotY =  -D3DX_PI / 2;

}

SkinnedMeshHLSL::~SkinnedMeshHLSL()
{
	if (isOrigin == false)
	{
		SAFE_RELEASE(m_pSphereMesh);

		Objects::RemoveFromTagList(TAG_DEFAULT, this);
		//Rendering::Get()->RemoveRenderingObject(this);
	}
}

void SkinnedMeshHLSL::Init()
{
	Objects::AddToTagList(TAG_DEFAULT, this);
	shaderType = ShaderType::ST_None;
	//Rendering::Get()->AddRenderingObject(this);

	D3DXCreateSphere(DX::GetDevice(), 0.01f, 10, 10, &m_pSphereMesh, NULL);
	
	//test
	//int index = (rand() % m_pAC->GetMaxNumAnimationSets() - 1);
	//SetAnimationIndex(m_pAC, index, false);
	//double pos = (rand() % 100) / 10.0f;
	//m_pAC->SetTrackPosition(0, pos);



	//SeparateFrame(m_pRootFrame, "UpperChest", m_pSubAC, m_pAC);
	//SeparateFrame(m_pRootFrame, "Bone_Turret_Base", m_pSubAC, m_pAC);
	//SeparateFrame(m_pRootFrame, "BoneSpine02", m_pSubAC, m_pAC);
	
	//if (m_pSubAC) SetAnimationIndex(m_pSubAC, 2, true);
	
	/*
	/////Add Anim Set/////
	LPD3DXFRAME	pTmpRoot = NULL;
	LPD3DXANIMATIONCONTROLLER pTmpAC = NULL;
	Load(pTmpRoot, pTmpAC, ASSET_PATH + _T("Models/marine/"), _T("Marine_Death.X"));
	AddAnimSet(m_pAC, pTmpAC, 0);
	
	AllocateHierarchyHLSL alloc;
	D3DXFrameDestroy(pTmpRoot, &alloc);
	/////Add Anim Set/////
	*/
}

void SkinnedMeshHLSL::Load(LPCTSTR path, LPCTSTR filename)
{
	isOrigin = true;
	m_pEffect = Shaders::Get()->GetEffect((SHADER_PATH + L"skinnedmesh.fx"));

	AllocateHierarchyHLSL alloc(path);

	CString fullPath(path);
	fullPath.Append(filename);
	
	D3DXLoadMeshHierarchyFromX(fullPath, D3DXMESH_MANAGED, DX::GetDevice(),
		&alloc, NULL, &m_pRootFrame, &m_pAC);

	SetupBoneMatrixPointers(m_pRootFrame);

	NumBoneMatricesMax = alloc.GetNumBoneMatricesMax();
	g_pBoneMatrices = new D3DXMATRIXA16[NumBoneMatricesMax];
}

void SkinnedMeshHLSL::Delete()
{
	AllocateHierarchyHLSL alloc;
	if (m_pRootFrame)
		D3DXFrameDestroy(m_pRootFrame, &alloc);
	if (m_pSubRootFrame)
		D3DXFrameDestroy(m_pSubRootFrame, &alloc);

	SAFE_DELETE_ARRAY(g_pBoneMatrices);
}

void SkinnedMeshHLSL::SeparateFrame(LPD3DXFRAME pFrame, LPCSTR name, 
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


void SkinnedMeshHLSL::AddAnimSet(LPD3DXANIMATIONCONTROLLER &pAC_Dest, LPD3DXANIMATIONCONTROLLER &pAC_Src, UINT index)
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
void SkinnedMeshHLSL::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
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

void SkinnedMeshHLSL::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
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


void SkinnedMeshHLSL::Update()
{
	/**/
	//Anim 별 플레이 시간 체크
	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	D3DXTRACK_DESC track;
	m_pAC->GetTrackDesc(0, &track);
	m_pAC->GetAnimationSet(0, &pCurrAnimSet);



	if (pCurrAnimSet->GetPeriodicPosition(track.Position)
		>= pCurrAnimSet->GetPeriod() - 0.02f)
	{
		//m_pAC->SetTrackEnable(0, false);
		//m_animIndex++;
		//SetAnimationIndex(m_pAC, m_animIndex, true);
		//SetAnimationIndex(m_pSubAC, m_animIndex, true);


		//tmpFrame = (FRAME_EX*)D3DXFrameFind(m_pRootFrame, "Bip01");

	//	D3DXVECTOR3 vec = D3DXVECTOR3(tmpFrame->CombinedTM._41, 0, tmpFrame->CombinedTM._43);
	//	position.x += vec.x;
	//	position.z += vec.z;
	}
	pCurrAnimSet->Release();
	

	if (Input::KeyDown(VK_F1))
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
	//ImGui::Begin("bools");
	//ImGui::SliderInt("frame", (int*)m_bDrawFrame, 0, 1);
	//ImGui::SliderInt("skel", (int*)m_bDrawSkeleton, 0, 1);
	//ImGui::SliderInt("wire", (int*)m_bWireFrame, 0, 1);
	//ImGui::End();


	IDisplayObject::UpdateInput();
	IDisplayObject::ApplyInput();
	IDisplayObject::ApplyVelocity();

	//if (Objects::GetCurrentMap())
	//{
	//	if (velocity.y <= 0)
	//	{
	//		float height;
	//		if (Objects::GetCurrentMap()->ComputeHeight(height, targetPosition))
	//		{
	//			if (targetPosition.y <= height)
	//			{
	//				targetPosition.y = height;
	//				velocity.y = 0;
	//			}
	//			position = targetPosition;
	//		}
	//	}
	//	else
	//	{
	//		position = targetPosition;
	//	}
	//}
	//else
	//{
	//	position = targetPosition;
	//	position.y = 0;
	//}

	IDisplayObject::UpdateLocalMatrix();
	
	D3DXMATRIX baseRotYMat;
	D3DXMatrixRotationY(&baseRotYMat, baseRotY);
	localMatrix = baseRotYMat * localMatrix;
	
	IDisplayObject::UpdateWorldMatrix();
	

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


void SkinnedMeshHLSL::UpdateAnim(LPD3DXANIMATIONCONTROLLER pAC)
{
	
	float fDeltaTime = 0.5f * Time::Get()->GetDeltaTime();
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


void SkinnedMeshHLSL::UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
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


void SkinnedMeshHLSL::Render()
{

	//////////////////////
	UpdateAnim(m_pAC);
	UpdateFrameMatrices(m_pRootFrame, NULL);


	//if (Input::KeyDown('1'))
	//{
	//	if (m_animIndex < m_pAC->GetMaxNumAnimationSets() - 1)
	//		m_animIndex++;

	//	SetAnimationIndex(m_pAC, m_animIndex, true);
	//	if (m_pSubAC)
	//		SetAnimationIndex(m_pSubAC, 2, true);
	//}
	//else if (Input::KeyDown('2'))
	//{
	//	if (m_animIndex > 0)
	//		m_animIndex--;

	//	SetAnimationIndex(m_pAC, m_animIndex, true);
	//	if (m_pSubAC)
	//		SetAnimationIndex(m_pSubAC, 2, true);
	//}

	D3DXVECTOR4 camPos(*Camera::Get()->GetPosition(), 1);
	m_pEffect->SetVector("gWorldCameraPos", &camPos);

	D3DXVECTOR4 wLightDir(0, -1, 1, 0);
	m_pEffect->SetVector("gWorldLightDir", &wLightDir);

	D3DXMATRIX* matVP = Camera::Get()->GetVPMatrix();
	m_pEffect->SetMatrix("mViewProj", matVP);
	if (m_bWireFrame) DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (m_bDrawFrame)
	{
		DrawFrame(m_pRootFrame);
		if (m_pSubRootFrame) DrawFrame(m_pSubRootFrame);
	}
	
	DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	 
	if ( m_bDrawSkeleton)DrawSkeleton(m_pRootFrame, NULL);	
}

void SkinnedMeshHLSL::Render(IShader * pShader)
{
	Render();
}

void SkinnedMeshHLSL::Draw()
{
}



// Desc: Called to render a frame in the hierarchy
void SkinnedMeshHLSL::DrawFrame(LPD3DXFRAME pFrame)
{
	
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void SkinnedMeshHLSL::DrawMeshContainer(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer->pSkinInfo == NULL)
		return;

	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pFrame->pMeshContainer;
	
	LPD3DXBONECOMBINATION pBoneComb;

	pBoneComb = static_cast<LPD3DXBONECOMBINATION>
		(pMeshContainerEx->pBoneCombinationBuf->GetBufferPointer());
	

	UINT iMatrixIndex;
	D3DXMATRIX matTemp;

	for (UINT iAttrib = 0; iAttrib < pMeshContainerEx->NumAttributeGroups; iAttrib++)
	{
		// first calculate all the world matrices
		for (UINT iPaletteEntry = 0; iPaletteEntry < pMeshContainerEx->NumPaletteEntries; ++iPaletteEntry)
		{
			iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
			if (iMatrixIndex != UINT_MAX)
			{
				matTemp = pMeshContainerEx->pBoneOffsetMatrices[iMatrixIndex] *
					*pMeshContainerEx->ppBoneMatrixPtrs[iMatrixIndex];
				g_pBoneMatrices[iPaletteEntry] = matTemp * worldMatrix;
			}
		}
		m_pEffect->SetMatrixArray("mWorldMatrixArray", g_pBoneMatrices,
			pMeshContainerEx->NumPaletteEntries);
			
		// Sum of all ambient and emissive contribution
		D3DXCOLOR color1(pMeshContainerEx->vecMtlTex[pBoneComb[iAttrib].AttribId]->Ambient);
		D3DXCOLOR color2(.25, .25, .25, 1.0);
		D3DXCOLOR ambEmm;
		D3DXColorModulate(&ambEmm, &color1, &color2);
		ambEmm += D3DXCOLOR(pMeshContainerEx->vecMtlTex[pBoneComb[iAttrib].AttribId]->Emissive);

		// set material color properties 
		m_pEffect->SetVector("MaterialDiffuse",
			(D3DXVECTOR4*)&(
				pMeshContainerEx->vecMtlTex[pBoneComb[iAttrib].AttribId]->Diffuse));
		m_pEffect->SetVector("MaterialAmbient", (D3DXVECTOR4*)&ambEmm);

		// setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
		m_pEffect->SetTexture("DiffuseMap_Tex", pMeshContainerEx->vecMtlTex[pBoneComb[iAttrib].AttribId]->GetDiffuseMapTex());

		// Set CurNumBones to select the correct vertex shader for the number of bones
		m_pEffect->SetInt("CurNumBones", pMeshContainerEx->NumInfl - 1);
		m_pEffect->CommitChanges();
		// Start the effect now all parameters have been updated
		UINT numPasses;
		m_pEffect->Begin(&numPasses, D3DXFX_DONOTSAVESTATE);
		for (UINT iPass = 0; iPass < numPasses; iPass++)
		{
			m_pEffect->BeginPass(iPass);
			
			// draw the subset with the current world matrix palette and material state
			pMeshContainerEx->MeshData.pMesh->DrawSubset(iAttrib);

			m_pEffect->EndPass();
		}

		m_pEffect->End();
		
		DX::GetDevice()->SetVertexShader(NULL);
		DX::GetDevice()->SetPixelShader(NULL);
	}
	
	DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void SkinnedMeshHLSL::DrawSkeleton(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
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

		vector<VERTEX_PC> line{ VERTEX_PC(posThis, Color::BLUE), VERTEX_PC(posParent, Color::YELLOW) };
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



void SkinnedMeshHLSL::SetAnimationIndex(LPD3DXANIMATIONCONTROLLER pAC, int nIndex, bool isBlend)
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

	//pAC->SetTrackAnimationSet(0, pNextAnimSet);
	pAC->ResetTime();
	pAC->SetTrackPosition(0, 0);
	SAFE_RELEASE(pNextAnimSet);
}

//LPD3DXFRAME SkinnedMeshHLSL::FindBone(LPD3DXFRAME pMeshContainerBase)
//{
//	//DWORD numBones;
//	//FRAME_EX* pFrameExInfluence;
//	//MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pMeshContainerBase;
//
//	//if (pMeshContainerEx->pSkinInfo != NULL)
//	//{
//	//	numBones = pMeshContainerEx->pSkinInfo->GetNumBones();
//
//	//	for (DWORD i = 0; i < numBones; ++i)
//	//	{
//	//		pFrameExInfluence = (FRAME_EX*)D3DXFrameFind(m_pRootFrame,
//	//			"Bip01-Head");
//
//	//		pMeshContainerEx->ppBoneMatrixPtrs[i] = &pFrameExInfluence->CombinedTM;
//	//	}
//	//}
//
//	return (LPD3DXFRAME)D3DXFrameFind(m_pRootFrame,"Bip01-Head");
//}
