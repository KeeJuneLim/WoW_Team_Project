#pragma once

class SceneTerrain : public IScene
{
private:
	
public:
	SceneTerrain(WORD index);
	~SceneTerrain();

	// IScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

