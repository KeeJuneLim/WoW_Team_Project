#pragma once

class QuadTree
{
	// 쿼드트리에 보관되는 4개의 코너값에 대한 상수값
	enum			CornerType { CORNER_BL, CORNER_TL, CORNER_BR, CORNER_TR };
	// 이웃노드 처리용 상수값
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT };
	// 쿼드트리와 프러스텀간의 관계
	enum			QuadLocation {
		Frustum_OUT = 0,				// 프러스텀에서 완전벗어남
		Frustum_PARTIALLY_IN = 1,	// 프러스텀에 부분포함
		Frustum_COMPLETELY_IN = 2,	// 프러스텀에 완전포함
		Frustum_UNKNOWN = -1
	};		// 모르겠음(^^;)

private:
	QuadTree * m_pChild[4];		// QuadTree의 4개의 자식노드

	QuadTree*	m_pParent;			// Triangle Crack(Popping)을 막기위해서 사용한다.
	QuadTree*	m_pNeighbor[4];		// Triangle Crack(Popping)을 막기위해서 사용한다.

	int			m_nCenter;			// QuadTree에 보관할 첫번째 값
	int			m_nCorner[4];		// QuadTree에 보관할 두번째 값
									//    TopLeft(TL)      TopRight(TR)
									//              0------1
									//              |      |
									//              |      |
									//              2------3
									// BottomLeft(BL)      BottomRight(BR)
	BOOL		m_bCulled;			// 프러스텀에서 컬링된 노드인가?
	float		m_fRadius;			// 노드를 감싸는 경계구(bounding sphere)의 반지름

private:

	/// 쿼드트리를 만든다.(Build()함수에서 불린다)
	BOOL		_BuildQuadTree(D3DXVECTOR3* pHeightMap);

	/// 이웃노드를 만든다.(삼각형 찢어짐 방지용)
	void		_BuildNeighborNode(QuadTree* pRoot, D3DXVECTOR3* pHeightMap, int cx);

	// Quadtree를 4개의 하위 트리로 부분분할(subdivide)한다.
	BOOL		_SubDivideQuadTree();

	/// 4개의 코너값을 셋팅한다.
	BOOL		_SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);


	/// 현재 노드가 LOD등급으로 볼때  출력이 가능한 노드인가?
	BOOL		IsVisible(D3DXVECTOR3* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		return ((m_nCorner[CORNER_BR] - m_nCorner[CORNER_BL]) <= _GetLODLevel(pHeightMap, pCamera, fLODRatio));
	}
	/// 카메라와 현재 노드와의 거리값을 기준으로 LOD값을 구한다.
	int			_GetLODLevel(D3DXVECTOR3* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		float d = D3DXVec3Length(&(*(pHeightMap + m_nCenter) - *pCamera));
		return max((int)(d * fLODRatio), 1);
	}



	/// 현재노드가 프러스텀에 포함되는가?
	int			_IsInFrustum(D3DXVECTOR3* pHeightMap, Frustum* pFrustum);

	/// _IsInFrustum()함수의 결과에 따라 프러스텀 컬링 수행
	void		_FrustumCull(D3DXVECTOR3* pHeightMap, Frustum* pFrustum);

	// 모든 자식 노드가 프러스텀 안에 포함돼있다면 호출하는 함수(m_bCulled값을 false로 변경)
	void		_AllInFrustum();



	/// 쿼드트리를 검색해서 4개 코너값과 일치하는 노드를 찾는다.
	QuadTree*	_FindNode(D3DXVECTOR3* pHeightMap, int _0, int _1, int _2, int _3);

	/// 4개 방향(상단,하단,좌측,우측)의 이웃노드 인덱스를 구한다.
	int			_GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);

	BOOL IsInRect(RECT* rc, POINT pt);

	///	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
	int			_GenTriIndex(int nTris, LPVOID pIndex, D3DXVECTOR3* pHeightMap,
		Frustum* pFrustum, float fLODRatio);

public:

	QuadTree(int _dimension); // 최초 루트노드 생성자
	QuadTree(QuadTree* pParent); // 하위 자식노드 생성자
	~QuadTree();

	BOOL		Build(D3DXVECTOR3* pHeightMap);
	//	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
	int			GenerateIndex(LPVOID pIndex, D3DXVECTOR3* pHeightMap, Frustum* pFrustum, float fLODRatio);

};
