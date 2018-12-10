#include "stdafx.h"
#include "QuadTree.h"

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

//���� ��Ʈ ��� ������
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

//���� �ڽ� ��� ������
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


//����Ʈ���� �����.
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
		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TR])) -
			*((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BL]));
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
		m_fRadius = D3DXVec3Length(&v) / 2.0f;
		m_pChild[CORNER_TL]->_BuildQuadTree(pHeightMap);
		m_pChild[CORNER_TR]->_BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BL]->_BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BR]->_BuildQuadTree(pHeightMap);
	}

	return true;
}

//����Ʈ���� 4���� �ڽ� Ʈ���� �������� �Ѵ�.
BOOL QuadTree::_SubDivideQuadTree()
{
	int nTopEdgeCenter;
	int nBottomEdgeCenter;
	int nLeftEdgeCenter;
	int nRightEdgeCenter;
	int nCentralPoint;

	//��ܺ� ���
	nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
	//�ϴܺ� ���
	nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
	//������ ���
	nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
	//������ ���
	nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
	//�Ѱ��
	nCentralPoint =
		(m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;


	//�� �̻� ������ �Ұ����Ѱ�? �׷��ٸ� SubDivide() ����
	if ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1) return false;


	//4���� �ڽ� ��� �߰�
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
		// �̿������ 4�� �ڳʰ��� ��´�.
		n = _GetNodeIndex(i, cx, _0, _1, _2, _3);
		// �ڳʰ����� �̿������ �����͸� ���´�.
		if (n >= 0) m_pNeighbor[i] = pRoot->_FindNode(pHeightMap, _0, _1, _2, _3);
	}

	// �ڽĳ��� ���ȣ��
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
	gap = _c - _a;	// ���� ����� �¿�����

	switch (ed)
	{
	case EDGE_UP:	// ���� ���� �̿������ �ε���
		_1 = _b + cx * gap;
		_3 = _d + cx * gap;
		_0 = _b;
		_2 = _d;
		break;
	case EDGE_DN:	// �Ʒ� ���� �̿������ �ε���
		_1 = _a;
		_3 = _c;
		_0 = _a - cx * gap;
		_2 = _c - cx * gap;
		break;
	case EDGE_LT:	// ���� ���� �̿������ �ε���
		_1 = _b - gap;
		_3 = _b;
		_0 = _a - gap;
		_2 = _a;
		break;
	case EDGE_RT:	// ���� ���� �̿������ �ε���
		_1 = _d;
		_3 = _d + gap;
		_0 = _c;
		_2 = _c + gap;
		if ((_0 / cx) != (_a / cx)) return -1;
		if ((_1 / cx) != (_b / cx)) return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;	// ��� �ε���
	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

	return n;
}


QuadTree * QuadTree::_FindNode(D3DXVECTOR3 * pHeightMap, int _0, int _1, int _2, int _3)
{
	QuadTree*	p = NULL;
	// ��ġ�ϴ� ����� ��尪�� ����
	if ((m_nCorner[0] == _0) && (m_nCorner[1] == _1) && (m_nCorner[2] == _2) && (m_nCorner[3] == _3))
		return this;

	// �ڽ� ��尡 �ְ�?
	if (m_pChild[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) / 4;

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pHeightMap[n].x;
		pt.y = (int)pHeightMap[n].z;

		// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
		SetRect(&rc, (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].z,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].z);
		// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
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




//�ﰢ���� �ε����� ����� ����� �ﰢ���� ������ ��ȯ�Ѵ�.
int QuadTree::GenerateIndex(LPVOID pIndex, D3DXVECTOR3* pHeightMap, Frustum* pFrustum, float fLODRatio)
{
	_FrustumCull(pHeightMap, pFrustum);
	return _GenTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
}

// �Լ��� ����� ���� ����ü �ø� ����
void QuadTree::_FrustumCull(D3DXVECTOR3 *pHeightMap, Frustum *pFrustum)
{
	int ret;

	ret = _IsInFrustum(pHeightMap, pFrustum);
	switch (ret)
	{
	case Frustum_COMPLETELY_IN:
		//����ü�� ������ ���Ե�. ������� �˻� �ʿ� ����
		_AllInFrustum();
		return;
	case Frustum_PARTIALLY_IN:
		//����ü�� �Ϻ� ���Ե�. ������� �˻� �ʿ���
		m_bCulled = FALSE;
		break;
	case Frustum_OUT:
		//����ü���� ������ ���. ������� �˻� �ʿ� ����
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

	//��豸 �ȿ� �ִ°�?
	//if(m_fRadius == 0.0f) g_pLog->Log("Index : [%d], Radius : [%f]", m_nCenter, m_fRadius);
	bInSphere = pFrustum->IsSphereInsideFrustum((D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);

	if (!bInSphere)
	{
		return Frustum_OUT; //��豸 �ȿ� ������ �� ������ ����ü �׽�Ʈ ����
	}

	//����Ʈ���� 4���� ��� ����ü �׽�Ʈ
	b[0] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
	b[1] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
	b[2] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
	b[3] = pFrustum->IsInTerrainVertex((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

	//4�� ��� ����ü �ȿ� ����
	if ((b[0] + b[1] + b[2] + b[3]) == 4)
	{
		return Frustum_COMPLETELY_IN;
	}

	//�Ϻκ��� ����ü�� �ִ� ���
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


//����� �������� �ε����� �����Ѵ�.
int QuadTree::_GenTriIndex(int nTris, LPVOID pIndex, D3DXVECTOR3* pHeightMap,
	Frustum* pFrustum, float fLODRatio)
{
	//�ø��� ����� �׳� �����Ѵ�.
	if (m_bCulled)
	{
		m_bCulled = FALSE;
		return nTris;
	}

	//���� ��尡 ��µǾ�� �ϴ°�?
	D3DXVECTOR3 camPos = *Camera::Get()->GetPosition();

	if (IsVisible(pHeightMap, &camPos, fLODRatio))
	{

		LPDWORD p = ((LPDWORD)pIndex) + nTris * 3;

		// ���� ������ ����� �κк���(subdivide)�� �Ұ����ϹǷ� �׳� ����ϰ� �����Ѵ�.
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
		// ��� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_UP]) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(pHeightMap, &camPos, fLODRatio);
		// �ϴ� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_DN]) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(pHeightMap, &camPos, fLODRatio);
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_LT]) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(pHeightMap, &camPos, fLODRatio);
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if (m_pNeighbor[EDGE_RT]) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(pHeightMap, &camPos, fLODRatio);

		// �̿������� ��δ� ��°����ϴٸ� ������� �̿������� ���� LOD�̹Ƿ� 
		// �κк����� �ʿ����.
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

		if (!b[EDGE_UP]) // ��� �κк����� �ʿ��Ѱ�?
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TR]; nTris++;
		}
		else	// ��� �κк����� �ʿ���� ���
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = m_nCorner[CORNER_TR]; nTris++;
		}

		if (!b[EDGE_DN]) // �ϴ� �κк����� �ʿ��Ѱ�?
		{
			n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BL]; nTris++;
		}
		else	// �ϴ� �κк����� �ʿ���� ���
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = m_nCorner[CORNER_BL]; nTris++;
		}

		if (!b[EDGE_LT]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TL]; nTris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = m_nCorner[CORNER_TL]; nTris++;
		}

		if (!b[EDGE_RT]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BR]; nTris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = m_nCorner[CORNER_BR]; nTris++;
		}

		return nTris;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
	}

	// �ڽ� ���� �˻�
	if (m_pChild[CORNER_TL]) nTris = m_pChild[CORNER_TL]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_TR]) nTris = m_pChild[CORNER_TR]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BL]) nTris = m_pChild[CORNER_BL]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BR]) nTris = m_pChild[CORNER_BR]->_GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);


	return nTris;
}