#pragma once

class Sky : public IDisplayObject
{
private:
	ID3DXMesh * mesh;

public:
	Sky();
	~Sky();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Draw() override;
};