#pragma once

class ObjMap : public IMap
{
private:
	vector<D3DXVECTOR3> surfaceVertices;
	ID3DXMesh* mesh;
	vector<Material*> materials;
	VARIABLE(float, rayOffsetY, RayOffsetY);
	UINT currentSubset = 0;

	bool playerMoving;

public:
	ObjMap();
	~ObjMap();

	// IMap을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override;

	virtual bool ComputeHeight(float & height, const D3DXVECTOR3 & pos) override;
	virtual bool ComputeClickPosition(D3DXVECTOR3 &vOut, WORD screenX, WORD screenY) override;

	void Load(wstring filePath, wstring fileName, D3DXMATRIX* transform);
	
};

