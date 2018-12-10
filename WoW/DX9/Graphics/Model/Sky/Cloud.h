#pragma once

class Cloud : public IDisplayObject
{
	typedef VERTEX_PT VertexType;
private:
	
	vector<VertexType> vertices;
	vector<UINT> indices;
	
public:
	Cloud();
	~Cloud();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader);
	virtual void Draw() override;
};