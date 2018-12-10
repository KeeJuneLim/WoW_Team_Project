#pragma once
#include "stdafx.h"
#include "Graphics/View/Ray.h"
#include "Manager/Shaders.h"

class Boundary
{
public:
	bool isRender;
	bool isPicked;
	float cullingRadius;
	D3DXMATRIX	worldMatrix;

	Boundary() :isRender(false), isPicked(false) {};
	virtual ~Boundary() {};
	virtual void InitGeometry() {}
	virtual void UpdateTransform(D3DXMATRIX &_worldMat) {}
	virtual bool IntersectRay(Ray &ray) { return false; }
	virtual void Render() {}
	virtual void Draw() {}
};

class BoundingSphere : public Boundary
{
public:
	D3DXVECTOR3 center;
	D3DXVECTOR3 localCenter;
	float		radius;
	float		currentRadius;
	
	ID3DXMesh* mesh;

	BoundingSphere() :center(0, 0, 0), mesh(NULL) {}
	BoundingSphere(D3DXVECTOR3 &_center, float _radius)
		:center(0, 0, 0), localCenter(_center), radius(_radius), currentRadius(_radius), mesh(NULL) {}
	~BoundingSphere() {
		SAFE_RELEASE(mesh);
	}

	void InitGeometry() override
	{
		D3DXCreateSphere(DX::GetDevice(), radius, 10, 10, &mesh, NULL);
		D3DXMatrixIdentity(&worldMatrix);
	}

	void UpdateTransform(D3DXMATRIX &_worldMat) override
	{
		D3DXVECTOR3 scale, translation;
		D3DXQUATERNION rotation;

		D3DXMatrixDecompose(&scale, &rotation, &translation, &_worldMat);
		center = localCenter + translation;
		float averageScale = (scale.x + scale.y + scale.z) / 3.0f;
		currentRadius = radius * averageScale;

		D3DXMATRIX matT, matS;
		D3DXMatrixScaling(&matS, averageScale, averageScale, averageScale);
		D3DXMatrixTranslation(&matT, center.x, center.y, center.z);
		worldMatrix = matS * matT;

		cullingRadius = currentRadius;
	}

	void Render() override
	{
		DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		DX::GetDevice()->SetTexture(0, NULL);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &worldMatrix);
		mesh->DrawSubset(0);
		DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	bool IntersectRay(Ray &ray) override
	{
		D3DXVECTOR3 localPos = ray.position - center;
		
		float qv = D3DXVec3Dot(&localPos, &ray.direction);
		//float vv = D3DXVec3Dot(&direction, &direction);
		float qq = D3DXVec3Dot(&localPos, &localPos);
		float rr = currentRadius * currentRadius;

		//return qv * qv - vv * (qq - rr) >= 0;
		return qv * qv - (qq - rr) >= 0;
	}

};


class BoundingBox : public Boundary
{
public:
	D3DXVECTOR3 center;
	D3DXVECTOR3 minPos;
	D3DXVECTOR3 maxPos;

	vector<D3DXVECTOR3> vertices;
	vector<D3DXVECTOR3> currentVertices;

	BoundingBox() {}
	BoundingBox(D3DXVECTOR3 _minPos, D3DXVECTOR3 _maxPos)
		:minPos(_minPos), maxPos(_maxPos) {}
	~BoundingBox() {}

	D3DXVECTOR3 GetMinPos() { return D3DXVECTOR3(center.x + minPos.x, center.y + minPos.y, center.z + minPos.z); }
	D3DXVECTOR3 GetMaxPos() { return D3DXVECTOR3(center.x + maxPos.x, center.y + maxPos.y, center.z + maxPos.z); }

	void InitGeometry() override
	{
		vertices.clear();
		vertices.reserve(g_vecCubeIndex.size());

		vector<D3DXVECTOR3> positions = {
			D3DXVECTOR3(minPos.x, minPos.y, minPos.z),
			D3DXVECTOR3(minPos.x, maxPos.y, minPos.z),
			D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z),
			D3DXVECTOR3(maxPos.x, minPos.y, minPos.z),
			D3DXVECTOR3(minPos.x, minPos.y, maxPos.z),
			D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z),
			D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z),
			D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z)
		};
		
		for (int i = 0; i < g_vecCubeIndex.size(); i++)
		{
			vertices.push_back(positions[g_vecCubeIndex[i]]);
		}
		
		currentVertices = vertices;
	}

	void UpdateTransform(D3DXMATRIX &_worldMat) override
	{
		worldMatrix = _worldMat;

		for (int i = 0; i < vertices.size(); i++)
		{
			D3DXVec3TransformCoord(&currentVertices[i], &vertices[i], &_worldMat);
		}

		D3DXVECTOR3 scale, translation;
		D3DXQUATERNION rotation;

		D3DXMatrixDecompose(&scale, &rotation, &translation, &_worldMat);
		center = translation;
		float averageScale = (scale.x + scale.y + scale.z) / 3.0f;

		cullingRadius = D3DXVec3Length(&(maxPos - minPos)) / 2.0f * averageScale;
	}

	void Render() override
	{
		DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		
		Shaders::Get()->GetShader(ShaderType::ST_Boundary)->Render(this);

		DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	void Draw() override
	{
		DX::GetDevice()->SetFVF(D3DFVF_XYZ);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			vertices.size() / 3, &vertices[0], sizeof(D3DXVECTOR3));
	}
	bool IntersectRay(Ray &ray) override
	{
		float dist = 0;
		for (int i = 0; i < vertices.size(); i += 3)
		{
			if (ray.IntersectTri(
				&currentVertices[i],
				&currentVertices[i + 1],
				&currentVertices[i + 2], &dist))
				return true;
		}
		return false;
	}

	bool IntersectBox(BoundingBox &box)
	{
		if (GetMinPos().x > box.GetMaxPos().x || GetMaxPos().x < box.GetMinPos().x) return false;
		if (GetMinPos().y > box.GetMaxPos().y || GetMaxPos().y < box.GetMinPos().y) return false;
		if (GetMinPos().z > box.GetMaxPos().z || GetMaxPos().z < box.GetMinPos().z) return false;
		return true;
	}

	bool IntersectPoint(D3DXVECTOR3& p)
	{
		if (GetMinPos().x > p.x || GetMaxPos().x < p.x) return false;
		if (GetMinPos().y > p.y || GetMaxPos().y < p.y) return false;
		if (GetMinPos().z > p.z || GetMaxPos().z < p.z) return false;
		return true;
	}
	
};
