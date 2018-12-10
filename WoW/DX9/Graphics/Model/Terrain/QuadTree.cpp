#include "stdafx.h"
#include "QuadTree.h"

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

//최초 루트 노드 생성자
QuadTree::QuadTree(int _dimension)
{
	m_pParent = NULL;

	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
		m_pNeighbor[i] = NULL;
	}
	
	_SetCorners(_dimension * (_dimension - 1), _dimension * _dimension - 1, 0,  _dimension - 1 );
	
	m_bCulled = false;
	m_fRadius = 0.0f;
}

//하위 자식 노드 생성자
QuadTree::QuadTree(QuadTree *pParent)
{
	m_pParent = pParent;
	m_nCenter = 0;

	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
		m_pNeighbor[i] = NULL;
		m_nCorner[i] = 0;
	}

	m_bCulled = false;
	m_fRadius = 0.0f;
}


QuadTree::~QuadTree()
{
	for (auto p : m_pChild)
		SAFE_DELETE(p);
}



BOOL QuadTree::_SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;
	m_nCenter =
		(m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

	return true;
}


//쿼드트리를 만든다.
BOOL QuadTree::Build(D3DXVECTOR3 *pHeightMap)
{

	_BuildQuadTree(pHeightMap);
	_BuildNeighborNode(this, pHeightMap, m_nCorner[CORNER_BR] + 1);

	return TRUE;
}

BOOL QuadTree::_BuildQuadTree(D3DXVECTOR3 * pHeightMap)
{
	if (_SubDivideQuadTree())
	{
		// 좌측상단과, 우측 하단의 거리를 구한다.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TR])) -
			*((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BL]));
		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 
		// 2로 나누어 반지름을 구한다.
		m_fRadius = D3DXVec3Length(&v) / 2.0f;
		m_pChild[CORNER_TL]->_BuildQuadTree(pHeightMap);
		m_pChild[CORNER_TR]->_BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BL]->_BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BR]->_BuildQuadTree(pHeightMap);
	}

	return true;
}

//쿼드트리를 4개의 자식 트리로 하위분할 한다.
BOOL QuadTree::_SubDivideQuadTree()
{
	int nTopEdgeCenter;
	int nBottomEdgeCenter;
	int nLeftEdgeCenter;
	int nRightEdgeCenter;
	int nCentralPoint;

	//상단변 가운데
	nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
	//하단변 가운데
	nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
	//좌측변 가운데
	nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
	//우측변 가운데
	nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
	//한가운데
	nCentralPoint =
		(m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;


	//더 이상 분할이 불가능한가? 그렇다면 SubDivide() 종료
	if ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1) return false;


	//4개의 자식 노드 추가
	QuadTree* pChild = NULL;

	pChild = new QuadTree(this);
	pChild->_SetCorners(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
	m_pChild[CORNER_TL] = pChild;

	pChild = new QuadTree(this);
	pChild->_SetCorners(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
	m_pChild[CORNER_TR] = pChild;

	pChild = new QuadTree(this);
	pChild->_SetCorners(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
	m_pChild[CORNER_BL] = pChild;

	pChild = new QuadTree(this);
	pChild->_SetCorners(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);
	m_pChild[CORNER_BR] = pChild;

	return true;
}

void QuadTree::_BuildNeighborNode(QuadTree * pRoot, D3DXVECTOR3 * pHeightMap, int cx)
{
	int	n;
	int	_0, _1, _2, _3;

	for (int i = 0; i<4; i++)
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];
		// 이웃노드의 4개 코너값을 얻는다.
		n = _GetNodeIndex(i, cx, _0, _1, _2, _3);
		// 코너값으로 이웃노드의 포인터를 얻어온다.
		if (n >= 0) m_pNeighbor[i] = pRoot->_FindNode(pHeightMap, _0, _1, _2, _3);
	}

	// 자식노드로 재귀호출
	if (m_pChild[0])
	{
		m_pChild[0]->_BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[1]->_BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[2]->_BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[3]->_BuildNeighborNode(pRoot, pHeightMap, cx);
	}
}


int QuadTree::_GetNodeIndex(int ed, int cx, int & _0, int & _1, int & _2, int & _3)
{
	int		n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _c - _a;	// 현재 노드의 좌우폭값

	switch (ed)
	{
	case EDGE_UP:	// 위쪽 방향 이웃노드의 인덱스
		_1 = _b + cx * gap;
		_3 = _d + cx * gap;
		_0 = _b;
		_2 = _d;
		break;
	case EDGE_DN:	// 아래 방향 이웃노드의 인덱스
		_1 = _a;
		_3 = _c;
		_0 = _a - cx * gap;
		_2 = _c - cx * gap;
		break;
	case EDGE_LT:	// 좌측 방향 이웃노드의 인덱스
		_1 = _b - gap;
		_3 = _b;
		_0 = _a - gap;
		_2 = _a;
		break;
	case EDGE_RT:	// 우측 방향 이웃노드의 인덱스
		_1 = _d;
		_3 = _d + gap;
		_0 = _c;
		_2 = _c + gap;
		if ((_0 / cx) != (_a / cx)) return -1;
		if ((_1 / cx) != (_b / cx)) return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;	// 가운데 인덱스
	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

	return n;
}


QuadTree * QuadTree::_FindNode(D3DXVECTOR3 * pHeightMap, int _0, int _1, int _2, int _3)
{
	QuadTree*	p = NULL;
	// 일치하는 노드라면 노드값을 리턴
	if ((m_nCorner[0] == _0) && (m_nCorner[1] == _1) && (m_nCorner[2] == _2) && (m_nCorner[3] == _3))
		return this;

	// 자식 노드가 있가?
	if (m_pChild[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) / 4;

		// 현재 맵상에서의 위치
		pt.x = (int)pHeightMap[n].x;
		pt.y = (int)pHeightMap[n].z;

		// 4개의 코너값을 기준으로 자식노드의 맵 점유범위를 얻는다.
		SetRect(&rc, (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].z);
		// pt값이 점유범위안에 있다면 자식노드로 들어간다.
		if (IsInRect(&rc, pt))
			return m_pChild[0]->_FindNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].z);
		if (IsInRect(&rc, pt))
			return m_pChild[1]->_FindNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].z);
		if (IsInRect(&rc, pt))
			return m_pChild[2]->_FindNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].z);
		if (IsInRect(&rc, pt))
			return m_pChild[3]->_FindNode(pHeightMap, _0, _1, _2, _3);
	}

	return NULL;
}

BOOL QuadTree::IsInRect(RECT* rc, POINT pt)
{
	if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
		(rc->bottom <= pt.y) && (pt.y <= rc->top))
		return TRUE;

	return FALSE;
}




//삼각형의 인덱스를 만들고 출력할 삼각형의 개수를 반환한다.
int QuadTree::GenerateIndex(LPVOID pIndex, D3DXVECTOR3* pHeightMap, Frustum* pFrustum, float fLODRatio)
{
	_FrustumCull(pHeightMap, pFrustum);
	return _GenTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
}

// 함수의 결과에 따라 절두체 컬링 수행
void QuadTree::_FrustumCull(D3DXVECTOR3 *pHeightMap, Frustum *pFrustum)
{
	int ret;

	ret = _IsInFrustum(pHeightMap, pFrustum);
	switch (ret)
	{
	case Frustum_COMPLETELY_IN:
		//절두체에 완전히 포함됨. 하위노드 검색 필요 없음
		_AllInFrustum();
		return;
	case Frustum_PARTIALLY_IN:
		//절두체에 일부 포함됨. 하위노드 검색 필요함
		m_bCulled = FALSE;
		break;
	case Frustum_OUT:
		//절두체에서 완전히 벗어남. 하위노드 검색 필요 없음
		m_bCulled = TRUE;
		return;
	}

	if (m_pChild[0]) m_pChild[0]->_FrustumCull(pHeightMap, pFrustum);
	if (m_pChild[1]) m_pChild[1]->_FrustumCull(pHeightMap, pFrustum);
	if (m_pChild[2]) m_pChild[2]->_FrustumCull(pHeightMap, pFrustum);
	if (m_pChild[3]) m_pChild[3]->_FrustumCull(pHeightMap, pFrustum);
}



int QuadTree::_IsInFrustum(D3DXVECTOR3* pHeightMap, Frustum* pFrustum)
{
	BOOL b[4];
	BOOL bInSphere;

	//경계구 안에 있는가?
	//if(m_fRadius == 0.0f) g_pLog->Log("Index : [%d], Radius : [%f]", m_nCenter, m_fRadius);
	bInSphere = pFrustum->IsSphereInsideFrustum((D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);

	if (!bInSphere)
	{
		return Frustum_OUT; //경계구 안에 없으면 점 단위의 절두체 테스트 생략
	}

	//쿼드트리의 4군데 경계 절두체 테스트
	b[0] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
	b[1] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
	b[2] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
	b[3] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

	//4개 모두 절두체 안에 있음
	if ((b[0] + b[1] + b[2] + b[3]) == 4)
	{
		return Frustum_COMPLETELY_IN;
	}

	//일부분이 절두체에 있는 경우
	return Frustum_PARTIALLY_IN;
}


void QuadTree::_AllInFrustum()
{
	m_bCulled = FALSE;
	if (m_pChild[0] == false) return;

	m_pChild[0]->_AllInFrustum();
	m_pChild[1]->_AllInFrustum();
	m_pChild[2]->_AllInFrustum();
	m_pChild[3]->_AllInFrustum();

}


//출력할 폴리곤의 인덱스를 생성한다.
int QuadTree::_GenTriIndex(int nTris, LPVOID pIndex, D3DXVECTOR3* pHeightMap,
	Frustum* pFrustum, float fLODRatio)
{
	//컬링된 노드라면 그냥 리턴한다.
	if (m_bCulled)
	{
		m_bCulled = FALSE;
		return nTris;
	}

	//현재 노드가 출력되어야 하는가?
	D3DXVECTOR3 camPos = *Camera::Get()->GetPosition();

	if (IsVisible(pHeightMap, &camPos, fLODRatio))
	{

		LPDWORD p = ((LPDWORD)pIndex) + nTris * 3;

		// 만약 최하위 노드라면 부분분할(subdivide)이 불가능하므로 그냥 출력하고 리턴한다.
		if (m_nCorner[CORNER_BR] - m_nCorner[CORNER_BL] <= 1)
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTris++;
			return nTris;
		}

		D3DXVECTOR3 camPos = *Camera::Get()->GetPosition();

		BOOL	b[4] = { 0,0,0,0 };
		// 상단 이웃노드(neightbor node)가 출력가능한가?
		if (m_pNeighbor[EDGE_UP]) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(pHeightMap, &camPos, fLODRatio);
		// 하단 이웃노드(neightbor node)가 출력가능한가?
		if (m_pNeighbor[EDGE_DN]) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(pHeightMap, &camPos, fLODRatio);
		// 좌측 이웃노드(neightbor node)가 출력가능한가?
		if (m_pNeighbor[EDGE_LT]) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(pHeightMap, &camPos, fLODRatio);
		// 우측 이웃노드(neightbor node)가 출력가능한가?
		if (m_pNeighbor[EDGE_RT]) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(pHeightMap, &camPos, fLODRatio);

		// 이웃노드들이 모두다 출력가능하다면 현재노드와 이웃노드들이 같은 LOD이므로 
		// 부분분할이 필요없다.
		if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTris++;
			return nTris;
		}

		int		n;

		if (!b[EDGE_UP]) // 상단 부분분할이 필요한가?
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TR]; nTris++;
		}
		else	// 상단 부분분할이 필요없을 경우
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = m_nCorner[CORNER_TR]; nTris++;
		}

		if (!b[EDGE_DN]) // 하단 부분분할이 필요한가?
		{
			n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BL]; nTris++;
		}
		else	// 하단 부분분할이 필요없을 경우
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = m_nCorner[CORNER_BL]; nTris++;
		}

		if (!b[EDGE_LT]) // 좌측 부분분할이 필요한가?
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TL]; nTris++;
		}
		else	// 좌측 부분분할이 필요없을 경우
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = m_nCorner[CORNER_TL]; nTris++;
		}

		if (!b[EDGE_RT]) // 우측 부분분할이 필요한가?
		{
			n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BR]; nTris++;
		}
		else	// 우측 부분분할이 필요없을 경우
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = m_nCorner[CORNER_BR]; nTris++;
		}

		return nTris;	// 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!
	}

	// 자식 노드들 검색
	if (m_pChild[CORNER_TL]) nTris = m_pChild[CORNER_TL]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_TR]) nTris = m_pChild[CORNER_TR]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BL]) nTris = m_pChild[CORNER_BL]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BR]) nTris = m_pChild[CORNER_BR]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);


	return nTris;
}