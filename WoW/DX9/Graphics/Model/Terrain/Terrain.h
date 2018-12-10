#pragma once

class QuadTree;
class TerrainEditor;

class Terrain : public IMap
{
	friend TerrainEditor;

	struct VertexType
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3	n;
		D3DXVECTOR2	t;
		D3DXVECTOR2	t1;

		VertexType() {}
		VertexType(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}
	};
protected:
	vector<D3DXVECTOR3> surfaceVertices;
	float lodRatio = 0.025f;
	VARIABLE(float, heightFactor, HeightFactor);
	VARIABLE(float, tiling, Tiling);
	
private:
	UINT dimension = 65;
	UINT numTile;
	D3DXVECTOR3 size;
	
	vector<VertexType> vertices;
	vector<UINT> indices;
	vector<UINT> usedIndices;
	vector<UINT> editedIndices;
	
	QuadTree* quadTree;
	TerrainEditor* editor;
	
public:
	Terrain();
	virtual ~Terrain();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override {};
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

	// IMap을(를) 통해 상속됨
	virtual bool ComputeHeight(float & height, const D3DXVECTOR3 & pos) override;
	virtual bool ComputeClickPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) override;

	void Load(wstring filepath);
	
	void UpdateDimension(UINT dim);
	void InitVertices();
	void InitIndices();
	void InitBuffers();
	void UpdateVB();
	void UpdateIB();
	void UpdateNormal();
	void ApplyHeight(vector<float> &heights);
	void InitQuadTree();

	void UpdateEditedVertexNormal();

	void SaveData(wstring fullpath);
	void OpenData(wstring fullpath);

};


/*
파일 정보 읽기
fopen_s(&fp, szFullPath, "rb");
y = (unsigned char)fgetc(fp)

하이트맵 사이즈 257 * 257

버텍스의 갯수 257 * 257
타일의 갯수 256 * 256

1. 버텍스 좌표 설정(노말은 0, 1, 0)
인덱스 번호 = z * 257 + x
y = 색정보 / 10.0f; 분모로 높이 조절

2. 인덱스 구성(시계방향)
1--3	0 = z * 257 + x
|\ |	1 = (z + 1) * 257 + x
| \|	2 = z * 257 + x + 1
0--2	3 = (z + 1) * 257 + x + 1

3. 노말값 셋팅
---u---
|\ |\ |
| \| \|
l--n--r
|\ |\ |
| \| \|
---d---

du 벡터와 lr 벡터를 외적해서
n위치의 노말 값을 구한다.

4. 메쉬를 생성(최적화까지)

5. MtlTex 정보 셋팅

== == == == == == == == == == == == == == == == == == ==
GetHeight 함수
캐릭터의 높이 설정
1. 기준 페이스 선택
1--3	0의 x y 좌표 찾기
|\ |	x = (int)위치x
| \|	y = (int)위치y
0--2	deltaX = 위치x - x
deltaY = 위치y - y

deltaX + deltaY <  1 : 아래쪽 삼각형
deltaX + deltaY	>= 1 : 위쪽 삼각형

2. 실제 높이 계산하기

1--3	두 점 사이의 높이 차이 계산
|\ |	아래쪽 삼각형일 경우 : 0 + 높이차이 * 델타값
| \|	위쪽 삼각형일 경우 : 3 + 높이차이 * 델타값
0--2
*/
