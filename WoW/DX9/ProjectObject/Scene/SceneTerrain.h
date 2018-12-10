#pragma once

class SceneTerrain : public IScene
{
private:
	
public:
	SceneTerrain(WORD index);
	~SceneTerrain();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

