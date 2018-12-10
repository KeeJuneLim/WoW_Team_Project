#include "stdafx.h"
#include "DXDefinition.h"

DWORD FtoDw(float f) { return *((DWORD*)&f); }

float GetRandomFloat(float lowBound, float highBound)
{
	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

bool CompareStr(const char * str1, const char * str2)
{
	return strcmp(str1, str2) == 0;
}

void Lerp(float & pOut, float pV1, float pV2, float s)
{
	pOut = pV1 + s * (pV2 - pV1);
}

float EaseTo(float * out, float start, float end, float speed)
{
	*out += (end - start) * speed;
	return *out;
}

D3DXVECTOR3 EaseTo(D3DXVECTOR3 * out, D3DXVECTOR3& start, D3DXVECTOR3& end, float speed)
{
	*out += (end - start) * speed;
	return *out;
}

namespace DXUtil
{

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = a;
		mtrl.Diffuse = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power = p;

		return mtrl;
	}

	void ComputeNormal(D3DXVECTOR3* out, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2)
	{
		D3DXVECTOR3 v01 = *p1 - *p0;
		D3DXVECTOR3 v02 = *p2 - *p0;

		D3DXVec3Cross(out, &v01, &v02);
		D3DXVec3Normalize(out, out);
	}

	void ComputeBoundingBox(BYTE * pStart, UINT numVertices, UINT stride, D3DXVECTOR3& min, D3DXVECTOR3& max)
	{
		vector<D3DXVECTOR3*> positions;
		positions.resize(numVertices);

		min = { FLT_MAX, FLT_MAX, FLT_MAX };
		max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		for (int i = 0; i < numVertices; i++)
		{
			D3DXVECTOR3* pos = (D3DXVECTOR3*)(pStart + i * stride);

			if (pos->x < min.x) min.x = pos->x;
			if (pos->y < min.y) min.y = pos->y;
			if (pos->z < min.z) min.z = pos->z;

			if (pos->x > max.x) max.x = pos->x;
			if (pos->y > max.y) max.y = pos->y;
			if (pos->z > max.z) max.z = pos->z;
		}
	}

	void ComputeBoundingSphere(BYTE * pStart, UINT numVertices, UINT stride, D3DXVECTOR3 & center, float & radius)
	{
		D3DXVECTOR3 minPoint, maxPoint;
		ComputeBoundingBox(pStart, numVertices, stride, minPoint, maxPoint);
		center = (maxPoint + minPoint) / 2.0f;
		radius = D3DXVec3Length(&(maxPoint - minPoint)) / 4.0f;
	}

	D3DLIGHT9 InitDirectional(D3DXVECTOR3* dir, D3DXCOLOR* c)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = *c * 0.4f;
		light.Diffuse = *c;
		light.Specular = *c * 0.6f;
		light.Direction = *dir;

		return light;
	}

	D3DLIGHT9 InitPoint(D3DXVECTOR3* pos, D3DXCOLOR* c)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_POINT;
		light.Ambient = *c * 0.4f;
		light.Diffuse = *c;
		light.Specular = *c * 0.6f;
		light.Position = *pos;
		light.Range = 20.0f;
		light.Falloff = 1.0f;
		light.Attenuation0 = 0.005f;
		light.Attenuation1 = 0.005f;
		light.Attenuation2 = 0.005f;

		return light;
	}

	D3DLIGHT9 InitSpot(D3DXVECTOR3* dir, D3DXVECTOR3* pos, D3DXCOLOR* c)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_SPOT;
		light.Ambient = *c * 0.4f;
		light.Diffuse = *c;
		light.Specular = *c * 0.6f;
		light.Direction = *dir;
		light.Position = *pos;
		light.Range = 50.0f;
		light.Falloff = 1.0f;
		light.Theta = 0.0f;
		light.Phi = D3DX_PI / 6;
		light.Attenuation0 = 0.005f;
		light.Attenuation1 = 0.005f;
		light.Attenuation2 = 0.005f;
		
		return light;
	}

	void CreateVertexBuffer(LPDIRECT3DVERTEXBUFFER9 &pVb, LPVOID vertices, DWORD fvf, UINT vtxCnt, UINT stride)
	{
		DX::GetDevice()->CreateVertexBuffer(vtxCnt * stride,
			NULL, fvf, D3DPOOL_MANAGED, &pVb, NULL);

		LPVOID pVertex;
		pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
		memcpy(pVertex, vertices, vtxCnt * stride);
		pVb->Unlock();
	}

	void CreateIndexBuffer(LPDIRECT3DINDEXBUFFER9 &pIb, LPVOID indices, UINT idxCnt, UINT stride)
	{
		D3DFORMAT format;
		if (stride == sizeof(WORD)) format = D3DFMT_INDEX16;
		else if (stride == sizeof(DWORD)) format = D3DFMT_INDEX32;

		DX::GetDevice()->CreateIndexBuffer(idxCnt * stride,
			NULL, format, D3DPOOL_MANAGED, &pIb, NULL);

		LPVOID pIndex;
		pIb->Lock(0, 0, (LPVOID*)&pIndex, 0);
		memcpy(pIndex, indices, idxCnt * stride);
		pIb->Unlock();
	}

}
