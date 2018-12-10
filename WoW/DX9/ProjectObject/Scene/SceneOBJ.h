#pragma once

class SceneOBJ : public IScene
{
private:
	
public:
	SceneOBJ(WORD index);
	~SceneOBJ();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

