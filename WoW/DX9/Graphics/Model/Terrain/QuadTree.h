#pragma once

class QuadTree
{
	// ����Ʈ���� �����Ǵ� 4���� �ڳʰ��� ���� �����
	enum			CornerType { CORNER_BL, CORNER_TL, CORNER_BR, CORNER_TR };
	// �̿���� ó���� �����
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT };
	// ����Ʈ���� �������Ұ��� ����
	enum			QuadLocation {
		Frustum_OUT = 0,				// �������ҿ��� �������
		Frustum_PARTIALLY_IN = 1,	// �������ҿ� �κ�����
		Frustum_COMPLETELY_IN = 2,	// �������ҿ� ��������
		Frustum_UNKNOWN = -1
	};		// �𸣰���(^^;)

private:
	QuadTree * m_pChild[4];		// QuadTree�� 4���� �ڽĳ��

	QuadTree*	m_pParent;			// Triangle Crack(Popping)�� �������ؼ� ����Ѵ�.
	QuadTree*	m_pNeighbor[4];		// Triangle Crack(Popping)�� �������ؼ� ����Ѵ�.

	int			m_nCenter;			// QuadTree�� ������ ù��° ��
	int			m_nCorner[4];		// QuadTree�� ������ �ι�° ��
									//    TopLeft(TL)      TopRight(TR)
									//              0------1
									//              |      |
									//              |      |
									//              2------3
									// BottomLeft(BL)      BottomRight(BR)
	BOOL		m_bCulled;			// �������ҿ��� �ø��� ����ΰ�?
	float		m_fRadius;			// ��带 ���δ� ��豸(bounding sphere)�� ������

private:

	/// ����Ʈ���� �����.(Build()�Լ����� �Ҹ���)
	BOOL		_BuildQuadTree(D3DXVECTOR3* pHeightMap);

	/// �̿���带 �����.(�ﰢ�� ������ ������)
	void		_BuildNeighborNode(QuadTree* pRoot, D3DXVECTOR3* pHeightMap, int cx);

	// Quadtree�� 4���� ���� Ʈ���� �κк���(subdivide)�Ѵ�.
	BOOL		_SubDivideQuadTree();

	/// 4���� �ڳʰ��� �����Ѵ�.
	BOOL		_SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);


	/// ���� ��尡 LOD������� ����  ����� ������ ����ΰ�?
	BOOL		IsVisible(D3DXVECTOR3* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		return ((m_nCorner[CORNER_BR] - m_nCorner[CORNER_BL]) <= _GetLODLevel(pHeightMap, pCamera, fLODRatio));
	}
	/// ī�޶�� ���� ������ �Ÿ����� �������� LOD���� ���Ѵ�.
	int			_GetLODLevel(D3DXVECTOR3* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		float d = D3DXVec3Length(&(*(pHeightMap + m_nCenter) - *pCamera));
		return max((int)(d * fLODRatio), 1);
	}



	/// �����尡 �������ҿ� ���ԵǴ°�?
	int			_IsInFrustum(D3DXVECTOR3* pHeightMap, Frustum* pFrustum);

	/// _IsInFrustum()�Լ��� ����� ���� �������� �ø� ����
	void		_FrustumCull(D3DXVECTOR3* pHeightMap, Frustum* pFrustum);

	// ��� �ڽ� ��尡 �������� �ȿ� ���Ե��ִٸ� ȣ���ϴ� �Լ�(m_bCulled���� false�� ����)
	void		_AllInFrustum();



	/// ����Ʈ���� �˻��ؼ� 4�� �ڳʰ��� ��ġ�ϴ� ��带 ã�´�.
	QuadTree*	_FindNode(D3DXVECTOR3* pHeightMap, int _0, int _1, int _2, int _3);

	/// 4�� ����(���,�ϴ�,����,����)�� �̿���� �ε����� ���Ѵ�.
	int			_GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);

	BOOL IsInRect(RECT* rc, POINT pt);

	///	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
	int			_GenTriIndex(int nTris, LPVOID pIndex, D3DXVECTOR3* pHeightMap,
		Frustum* pFrustum, float fLODRatio);

public:

	QuadTree(int _dimension); // ���� ��Ʈ��� ������
	QuadTree(QuadTree* pParent); // ���� �ڽĳ�� ������
	~QuadTree();

	BOOL		Build(D3DXVECTOR3* pHeightMap);
	//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
	int			GenerateIndex(LPVOID pIndex, D3DXVECTOR3* pHeightMap, Frustum* pFrustum, float fLODRatio);

};
