#pragma once

class SceneTest : public IScene
{
private:
	class RectObject* rect;
	class Cube* cube;
	class Cube* cubeChild;
	class Grid* grid;

public:
	SceneTest(WORD index);
	~SceneTest();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

