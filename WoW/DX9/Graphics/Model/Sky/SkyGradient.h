#pragma once

class SkyGradient : public IDisplayObject
{
private:
	ID3DXMesh * mesh;

public:
	SkyGradient();
	~SkyGradient();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Render(IShader* pShader);
	virtual void Draw() override;
};