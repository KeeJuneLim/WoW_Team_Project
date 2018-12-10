#pragma once

class Cube
{
	typedef VERTEX_PNT VertexType;
private:
	VARIABLE_P(D3DXVECTOR3, position, Position);
	VARIABLE_P(D3DXVECTOR3, rotation, Rotation);
	VARIABLE_P(D3DXVECTOR3, scale, Scale);
	VARIABLE_P(D3DXVECTOR3, forward, Forward);
	VARIABLE_P(D3DXVECTOR3, deltaTransform, deltaTransform);

	VARIABLE_P(D3DXMATRIXA16, localMatrix, LocalMatrix);
	VARIABLE_P(D3DXMATRIXA16, worldMatrix, WorldMatrix);

	VARIABLE_P(Cube*, parent, Parent);
	VARIABLE_P(vector<Cube*>, children, Children);

	vector<VertexType> vertices;
	IDirect3DTexture9* tex;
	bool useBuffer;
	IDirect3DVertexBuffer9* vb;

	Boundary* boundary;

public:
	Cube();
	~Cube();

	void Init();
	void Update();
	void Render();
	void UpdateInput();
	void UpdateWorldMatrix();
};