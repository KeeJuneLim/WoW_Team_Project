#pragma once

class Sky : public IDisplayObject
{
private:
	ID3DXMesh * mesh;

public:
	Sky();
	~Sky();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Draw() override;
};