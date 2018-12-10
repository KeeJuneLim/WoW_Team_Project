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

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override {};
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

	// IMap��(��) ���� ��ӵ�
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
���� ���� �б�
fopen_s(&fp, szFullPath, "rb");
y = (unsigned char)fgetc(fp)

����Ʈ�� ������ 257 * 257

���ؽ��� ���� 257 * 257
Ÿ���� ���� 256 * 256

1. ���ؽ� ��ǥ ����(�븻�� 0, 1, 0)
�ε��� ��ȣ = z * 257 + x
y = ������ / 10.0f; �и�� ���� ����

2. �ε��� ����(�ð����)
1--3	0 = z * 257 + x
|\ |	1 = (z + 1) * 257 + x
| \|	2 = z * 257 + x + 1
0--2	3 = (z + 1) * 257 + x + 1

3. �븻�� ����
---u---
|\ |\ |
| \| \|
l--n--r
|\ |\ |
| \| \|
---d---

du ���Ϳ� lr ���͸� �����ؼ�
n��ġ�� �븻 ���� ���Ѵ�.

4. �޽��� ����(����ȭ����)

5. MtlTex ���� ����

== == == == == == == == == == == == == == == == == == ==
GetHeight �Լ�
ĳ������ ���� ����
1. ���� ���̽� ����
1--3	0�� x y ��ǥ ã��
|\ |	x = (int)��ġx
| \|	y = (int)��ġy
0--2	deltaX = ��ġx - x
deltaY = ��ġy - y

deltaX + deltaY <  1 : �Ʒ��� �ﰢ��
deltaX + deltaY	>= 1 : ���� �ﰢ��

2. ���� ���� ����ϱ�

1--3	�� �� ������ ���� ���� ���
|\ |	�Ʒ��� �ﰢ���� ��� : 0 + �������� * ��Ÿ��
| \|	���� �ﰢ���� ��� : 3 + �������� * ��Ÿ��
0--2
*/
