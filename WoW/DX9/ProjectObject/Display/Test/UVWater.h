#pragma once

class UVWater : public IDisplayObject
{
	typedef VERTEX_PNT VertexType;
protected:
	
	LPD3DXEFFECT m_pEffect;
	vector<VertexType> m_vertices;

public:
	UVWater();
	~UVWater();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader) override;
	virtual void Draw() override {};
};

