#pragma once

class Grid
{
	typedef VERTEX_PC VertexType;
private:
	VARIABLE_P(D3DXVECTOR3, position, Position);
	VARIABLE_P(D3DXVECTOR3, rotation, Rotation);
	VARIABLE_P(D3DXVECTOR3, scale, Scale);

	VARIABLE_P(D3DXMATRIXA16, localMatrix, LocalMatrix);
	VARIABLE_P(D3DXMATRIXA16, worldMatrix, WorldMatrix);

	vector<VertexType> vertices;
	
public:
	Grid();
	~Grid();

	void Init();
	void Update();
	void Render();
};

