#pragma once

class RectObject
{
	typedef VERTEX_PC VertexType;
private:
	VARIABLE_P(D3DXVECTOR3, position, Position);
	VARIABLE_P(D3DXVECTOR3, rotation, Rotation);
	VARIABLE_P(D3DXVECTOR3, scale, Scale);
	
	VARIABLE_P(D3DXMATRIXA16, localMatrix, LocalMatrix);
	VARIABLE_P(D3DXMATRIXA16, worldMatrix, WorldMatrix);
	
	vector<VertexType> vertices;
	vector<UINT> indices;
	bool useBuffer;
	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9* ib;

public:
	RectObject();
	~RectObject();

	void Init();
	void Update();
	void Render();
};