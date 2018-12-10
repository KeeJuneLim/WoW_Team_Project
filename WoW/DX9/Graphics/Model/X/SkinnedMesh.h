#pragma once

class SkinnedMesh : public IUnitObject
{
protected:
	LPD3DXFRAME					m_pRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pAC;
	
	int							m_animIndex;
	float						m_fBlendTime;
	float						m_fPassedBlendTime;

	LPD3DXMESH					m_pSphereMesh;
	bool						m_bWireFrame;
	bool						m_bDrawFrame;
	bool						m_bDrawSkeleton;
	int							m_numFrame;
	int							m_numMesh;

	/////Frame Separate/////
protected:
	LPD3DXFRAME					m_pConnetFrame;
	LPD3DXFRAME					m_pSubRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pSubAC;
	void SeparateFrame(LPD3DXFRAME pFrame, LPCSTR name,
		LPD3DXANIMATIONCONTROLLER &pAC_Dest, LPD3DXANIMATIONCONTROLLER &pAC_Src);
	/////Frame Separate/////

public:
	SkinnedMesh();
	virtual ~SkinnedMesh();

	// IUnitObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void Load(LPD3DXFRAME &pRootFrame, LPD3DXANIMATIONCONTROLLER &pAC, LPCTSTR path, LPCTSTR filename);
	void AddAnimSet(LPD3DXANIMATIONCONTROLLER &pAC_Dest, LPD3DXANIMATIONCONTROLLER &pAC_Src, UINT index);
private:
	void SetupBoneMatrixPointers(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);

	void UpdateAnim(LPD3DXANIMATIONCONTROLLER pAC);
	void UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

	void DrawFrame(LPD3DXFRAME pFrame);
	void DrawMeshContainer(LPD3DXFRAME pFrame);
	void DrawSkeleton(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void RenderUseShader(LPD3DXFRAME pFrame);
	void RenderUseShader_0();
	void RenderUseShader_1();

public:
	void SetAnimationIndex(LPD3DXANIMATIONCONTROLLER pAC, int nIndex, bool isBlend);
};

// OnInit
// pMeshContainerEx->ppBoneMatrixPtrs[i] = &pFrameExInfluence->CombinedTM;

// OnUpdate
// pFrameEx->CombinedTM = pFrameEx->TransformationMatrix * ((FRAME_EX*)pParent)->CombinedTM;
//
// pMeshContainerEx->pFinalBoneMatrices[i] =
// pMeshContainerEx->pBoneOffsetMatrices[i] * *(pMeshContainerEx->ppBoneMatrixPtrs[i]);
