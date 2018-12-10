#pragma once
#include "stdafx.h"

enum ObjectTag { 
	TAG_DEFAULT,
	TAG_PLAYER,
	TAG_MAP
};

enum SceneIndex {
	SCENE_GRID,
	SCENE_TERRAIN,
	SCENE_OBJ,
	SCENE_TEST,
	SCENE_COUNT
};

enum ShaderType {
	ST_ShadowMap,
	ST_Sky,
	ST_SkyGradient,
	ST_Cloud,
	ST_Terrain,
	ST_TerrainEditor,
	ST_Color,
	ST_Texture,
	ST_Lighting,
	ST_Shadow,
	ST_Hiding,
	ST_Toon,
	ST_Particle,
	ST_None,
	ST_Boundary,
	ST_COUNT
};


const vector<D3DXVECTOR3> g_vecCubeVertex = {
	D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(-1.0f,  1.0f, -1.0f),
	D3DXVECTOR3(1.0f,  1.0f, -1.0f),
	D3DXVECTOR3(1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(-1.0f, -1.0f,  1.0f),
	D3DXVECTOR3(-1.0f,  1.0f,  1.0f),
	D3DXVECTOR3(1.0f,  1.0f,  1.0f),
	D3DXVECTOR3(1.0f, -1.0f,  1.0f)
};

const vector<UINT> g_vecCubeIndex = {
	0, 1, 2, 0, 2, 3,	// 근
	7, 6, 5, 7, 5, 4,	// 원
	4, 5, 1, 4, 1, 0,	// 좌
	3, 2, 6, 3, 6, 7,	// 우
	1, 5, 6, 1, 6, 2,	// 상
	4, 0, 3, 4, 3, 7	// 하
};

const vector<D3DXVECTOR2> g_vecCubeUV =
{
	D3DXVECTOR2(0, 1),
	D3DXVECTOR2(0, 0),
	D3DXVECTOR2(1, 0),
	D3DXVECTOR2(1, 1)
};

const vector<D3DXVECTOR3> g_vecQuadVertex = {
	D3DXVECTOR3(-1, -1, 0),
	D3DXVECTOR3(-1, 1, 0),
	D3DXVECTOR3(1, 1, 0),
	D3DXVECTOR3(1, -1, 0)
};

const vector<WORD> g_vecQuadIndex = {
	0, 1, 2, 0, 2, 3
};

const vector<D3DXVECTOR2> g_vecQuadUV = {
	D3DXVECTOR2(0, 1),
	D3DXVECTOR2(0, 0),
	D3DXVECTOR2(1, 0),
	D3DXVECTOR2(1, 1)
};

DWORD FtoDw(float f);

float GetRandomFloat(float lowBound, float highBound);

bool CompareStr(const char * str1, const char * str2);

void Lerp(float &pOut, float pV1, float pV2, float s);


namespace Color
{
	const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
	const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
}


namespace DXUtil
{
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(Color::WHITE, Color::WHITE, Color::WHITE, Color::BLACK, 8.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(Color::RED, Color::RED, Color::RED, Color::BLACK, 8.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(Color::GREEN, Color::GREEN, Color::GREEN, Color::BLACK, 8.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(Color::BLUE, Color::BLUE, Color::BLUE, Color::BLACK, 8.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(Color::YELLOW, Color::YELLOW, Color::YELLOW, Color::BLACK, 8.0f);

	void ComputeNormal(D3DXVECTOR3* out, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2);
	void ComputeBoundingBox(BYTE* pStart, UINT numVertices, UINT stride, D3DXVECTOR3 &min, D3DXVECTOR3 &max);
	void ComputeBoundingSphere(BYTE* pStart, UINT numVertices, UINT stride, D3DXVECTOR3 &center, float &radius);
	
	D3DLIGHT9 InitDirectional(D3DXVECTOR3* dir, D3DXCOLOR* c);
	D3DLIGHT9 InitPoint(D3DXVECTOR3* pos, D3DXCOLOR* c);
	D3DLIGHT9 InitSpot(D3DXVECTOR3* dir, D3DXVECTOR3* pos, D3DXCOLOR* c);

	void CreateVertexBuffer(LPDIRECT3DVERTEXBUFFER9 &pVb, LPVOID vertices, DWORD fvf, UINT vtxCnt, UINT stride);
	void CreateIndexBuffer(LPDIRECT3DINDEXBUFFER9 &pIb, LPVOID indices, UINT idxCnt, UINT stride);

}
