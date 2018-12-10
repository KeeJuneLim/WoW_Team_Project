#pragma once

class Frustum
{
	typedef VERTEX_PC VertexType;
public:
	static Frustum*	Get();
	static void		Delete();
private:
	static Frustum*	instance;
	Frustum();
	~Frustum();

	vector<D3DXVECTOR3> projectionPoints;
	vector<D3DXVECTOR3> worldPoints;
	vector<D3DXPLANE> planes;
	
	VARIABLE(float, m_margin, Margin);
	
	vector<VertexType> vertices;
	vector<WORD> indices;
public:
	
	void Init();
	void Update();
	void Render();

	void UpdateFrustum();
	void UpdateFrustumVertices();

	bool IsSphereInsideFrustum(BoundingSphere * pSphere);
	bool IsSphereInsideFrustum(D3DXVECTOR3* pv, float radius);
	bool IsInTerrainVertex(D3DXVECTOR3* pv);
};

