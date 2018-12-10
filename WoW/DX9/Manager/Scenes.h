#pragma once

//프로그램 전체의 씬들을 관리

class Scenes
{
public:
	static Scenes*	Get() {
		if (instance == NULL) instance = new Scenes();
		return instance;
	}
	static void		Delete() { SAFE_DELETE(instance); }

private:
	static Scenes*	instance;
	Scenes();
	~Scenes();

	vector<IScene*> m_pSceneList;
	static IScene *	m_pCurrentScene;
	
public:
	void Init();
	void Update();
	void Render();
	
	void SetCurrentScene(int index);
	static IScene* GetCurrentScene() { return m_pCurrentScene; }
	void DeleteCurrentScene();
};

