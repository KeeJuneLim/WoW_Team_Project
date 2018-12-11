#pragma once
#include "ProjectObject/Display/UI/PlayerUI.h"

class SceneTest : public IScene
{
private:
	class RectObject* rect;
	class Cube* cube;
	class Cube* cubeChild;
	class Grid* grid;
	PlayerUI *playerUI = NULL;
public:
	SceneTest(WORD index);
	~SceneTest();

	// IScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

