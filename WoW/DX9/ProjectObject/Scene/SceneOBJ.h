#pragma once

class SceneOBJ : public IScene
{
private:
	
public:
	SceneOBJ(WORD index);
	~SceneOBJ();

	// IScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

