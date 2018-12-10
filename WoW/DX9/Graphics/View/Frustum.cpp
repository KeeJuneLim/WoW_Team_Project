#include "stdafx.h"
#include "Frustum.h"

Frustum* Frustum::instance = NULL;

Frustum * Frustum::Get()
{
	if (instance == NULL)
		instance = new Frustum();

	return instance;
}

void Frustum::Delete()
{
	SAFE_DELETE(instance);
}

Frustum::Frustum()
{
	m_margin = 5;
}


Frustum::~Frustum()
{
	
}

void Frustum::Init()
{
	projectionPoints.push_back(D3DXVECTOR3(-1, 1, 1));//좌상후
	projectionPoints.push_back(D3DXVECTOR3(1, 1, 1));//우상후
	projectionPoints.push_back(D3DXVECTOR3(-1, 1, 0));//좌상전
	projectionPoints.push_back(D3DXVECTOR3(1, 1, 0));//우상전

	projectionPoints.push_back(D3DXVECTOR3(-1, -1, 1));//좌하후
	projectionPoints.push_back(D3DXVECTOR3(1, -1, 1));//우하후
	projectionPoints.push_back(D3DXVECTOR3(-1, -1, 0));//좌하전
	projectionPoints.push_back(D3DXVECTOR3(1, -1, 0));//우하전
	
	worldPoints.resize(projectionPoints.size());
	planes.resize(6);

	//그리기용 변수 초기화
	vertices.resize(projectionPoints.size());
	
	indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(3);
	indices.push_back(0); indices.push_back(2); indices.push_back(1); indices.push_back(3);
	indices.push_back(4); indices.push_back(5); indices.push_back(6); indices.push_back(7);
	indices.push_back(4); indices.push_back(6); indices.push_back(5); indices.push_back(7);
	indices.push_back(0); indices.push_back(4); indices.push_back(1); indices.push_back(5);
	indices.push_back(2); indices.push_back(6); indices.push_back(3); indices.push_back(7);

	UpdateFrustum(); UpdateFrustumVertices();
}

void Frustum::Update()
{
	//if ( Input::Get()->KeyDown('0') )
		UpdateFrustum(); UpdateFrustumVertices();
}

void Frustum::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	DX::GetDevice()->SetTransform(D3DTS_WORLD, &mat);
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetTexture(0, NULL);
	DX::GetDevice()->SetFVF(VertexType::FVF);
	DX::GetDevice()->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0,
		vertices.size(), indices.size() / 2, &indices[0], D3DFMT_INDEX16,
		&vertices[0], sizeof(VertexType));
}

void Frustum::UpdateFrustum()
{
	D3DXMATRIX *viewMat = Camera::Get()->GetViewMatrix();
	D3DXMATRIX *projMat = Camera::Get()->GetProjectionMatrix();
	
	for (size_t i = 0; i < worldPoints.size(); i++)
	{
		D3DXVec3Unproject(&worldPoints[i], 
			&projectionPoints[i], NULL, projMat, viewMat, NULL);
	}

	//근평면//좌상전//우상전//좌하전
	D3DXPlaneFromPoints(&planes[0], &worldPoints[2],
		&worldPoints[3], &worldPoints[6]);

	//원평면//우상후//좌상후//우하후
	D3DXPlaneFromPoints(&planes[1], &worldPoints[1],
		&worldPoints[0], &worldPoints[5]);

	//좌평면//좌상후//좌상전//좌하후
	D3DXPlaneFromPoints(&planes[2], &worldPoints[0],
		&worldPoints[2], &worldPoints[4]);

	//우평면//우상전//우상후//우하전
	D3DXPlaneFromPoints(&planes[3], &worldPoints[3],
		&worldPoints[1], &worldPoints[7]);

	//상평면//좌상후//우상후//좌상전
	D3DXPlaneFromPoints(&planes[4], &worldPoints[0],
		&worldPoints[1], &worldPoints[2]);

	//하평면//좌하전//우하전//좌하후
	D3DXPlaneFromPoints(&planes[5], &worldPoints[6],
		&worldPoints[7], &worldPoints[4]);
}

void Frustum::UpdateFrustumVertices()
{
	for (int i = 0; i < worldPoints.size(); i++)
	{
		vertices[i] = VertexType(worldPoints[i], Color::GREEN);
	}
}

bool Frustum::IsSphereInsideFrustum(BoundingSphere * pSphere)
{
	for (auto p : planes)
	{
		if (D3DXPlaneDotCoord(&p, &pSphere->center) > pSphere->radius)
		{
			return false;
		}
	}
	return true;
}


bool Frustum::IsSphereInsideFrustum(D3DXVECTOR3 * pv, float radius)
{
	for (auto p : planes)
	{
		if (D3DXPlaneDotCoord(&p, pv) > radius + m_margin)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::IsInTerrainVertex(D3DXVECTOR3 * pv)
{
	for (auto p : planes)
	{
		if (D3DXPlaneDotCoord(&p, pv) > m_margin)
		{
			return false;
		}
	}
	return true;
}
