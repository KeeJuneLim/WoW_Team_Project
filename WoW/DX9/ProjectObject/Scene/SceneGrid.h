#pragma once

class SceneGrid : public IScene
{
private:
	class RectObject* rect;
	class Cube* cube;
	class Cube* cubeChild;
	class Grid* grid;

public:
	SceneGrid(WORD index);
	~SceneGrid();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

