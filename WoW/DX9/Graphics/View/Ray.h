#pragma once

class Ray
{
public:
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	
	Ray() : position(0, 0, 0), direction(0, 0, 1) {}
	Ray(D3DXVECTOR3 pos, D3DXVECTOR3 dir) :position(pos), direction(dir) {}
	~Ray() {}

	static Ray RayFromScreenToWorld(int x, int y);

	bool IntersectTri(D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2, float *distance);
};

 