#pragma once

class NormalMapping : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;
	
public:
	NormalMapping();
	~NormalMapping();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Draw() override {};

};