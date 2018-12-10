#pragma once

class InstancingTest : public IDisplayObject
{
	typedef VERTEX_PT VertexType;

	struct InstanceType
	{
		D3DXMATRIX matrix;
		D3DXVECTOR3 color;
	};

	struct DataType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 color;
	};
private:

	LPD3DXEFFECT effect;
	IDirect3DTexture9* tex;

	IDirect3DVertexBuffer9* vbIns = 0;
	UINT numInstance = 100;
	
	vector<DataType> datas;
	vector<InstanceType> instances;
public:
	InstancingTest();
	~InstancingTest();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override {};

	
};
