#include "stdafx.h"
#include "Heap.h"
#include "AStarNode.h"

Heap::Heap()
{
}


Heap::~Heap()
{
}

void Heap::Insert(AStarNode* pNode)
{
	m_vecHeap.push_back(pNode);
	Upheap(m_vecHeap.size() - 1);
}

void Heap::Upheap(int targetIdx)
{
	if (targetIdx == 0) return;

	int parentIdx = (targetIdx - 1) / 2;

	if (m_vecHeap[parentIdx]->m_f > m_vecHeap[targetIdx]->m_f)
	{
		Swap(targetIdx, parentIdx);
		Upheap(parentIdx);
	}
}


AStarNode* Heap::ExtractMin()
{
	if (m_vecHeap.empty()) return NULL;

	AStarNode* minFNode = m_vecHeap.front();
	Swap(0, m_vecHeap.size() - 1);
	m_vecHeap.pop_back();
	Downheap(0);
	
	return minFNode;
}

void Heap::Downheap(int targetIdx)
{
	int LChildIdx = targetIdx * 2 + 1;

	if (LChildIdx >= m_vecHeap.size()) return;
	
	int minChildIdx = LChildIdx;
	int RChildIdx = LChildIdx + 1;

	//2 ���� Child �� �� ���� ���� ã�´�
	if (RChildIdx < m_vecHeap.size())
	{
		if (m_vecHeap[RChildIdx]->m_f < m_vecHeap[LChildIdx]->m_f)
			minChildIdx = RChildIdx;
	}
	//target �� ���� �� ũ�� �ش� Child �� Swap
	if (m_vecHeap[targetIdx]->m_f > m_vecHeap[minChildIdx]->m_f)
	{
		Swap(targetIdx, minChildIdx);
		Downheap(minChildIdx);
	}
}

void Heap::Swap(int idx1, int idx2)
{
	AStarNode* pTemp = m_vecHeap[idx2];
	m_vecHeap[idx2] = m_vecHeap[idx1];
	m_vecHeap[idx1] = pTemp;
}

void Heap::Clear()
{
	m_vecHeap.clear();
}