#pragma once

class NormalMapping : public IDisplayObject
{
private:
	LPD3DXMESH	 m_pMesh;
	LPD3DXEFFECT m_pEffect;
	
public:
	NormalMapping();
	~NormalMapping();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Draw() override {};

};