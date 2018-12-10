#include "stdafx.h"
#include "Ray.h"

Ray Ray::RayFromScreenToWorld(int x, int y)
{
	D3DVIEWPORT9 vp;
	DX::GetDevice()->GetViewport(&vp);
	
	D3DXMATRIXA16 projMat;
	DX::GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);

	Ray ray;
	ray.direction.x = ((x * 2.0f) / vp.Width - 1.0f) / projMat._11;
	ray.direction.y = ((-y * 2.0f) / vp.Height + 1.0f) / projMat._22;

	D3DXMATRIXA16 viewMat, viewInvMat;
	DX::GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);
	D3DXMatrixInverse(&viewInvMat, NULL, &viewMat);
	
	D3DXVec3TransformCoord(&ray.position, &ray.position, &viewInvMat);
	D3DXVec3TransformNormal(&ray.direction, &ray.direction, &viewInvMat);
	D3DXVec3Normalize(&ray.direction, &ray.direction);

	return ray;
}

bool Ray::IntersectTri(D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2, float *distance)
{
	return D3DXIntersectTri(v0, v1, v2, &position, &direction, NULL, NULL, distance);
}



