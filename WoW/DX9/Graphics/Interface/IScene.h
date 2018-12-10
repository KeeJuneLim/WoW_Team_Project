#pragma once

class IScene
{
protected:
	VARIABLE(WORD, m_index, Index);

protected:
	set<IDisplayObject*> m_pDisplayObjects;
	
	IScene(WORD index):m_index(index) {}
public:
	virtual ~IScene() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	
	void AddDisplayObject(IDisplayObject* p) { m_pDisplayObjects.insert(p); }
	void RemoveDisplayObject(IDisplayObject* p) { m_pDisplayObjects.erase(p); }
	void OnUpdateIScene() { for (auto p : m_pDisplayObjects) SAFE_UPDATE(p); }
	void OnRenderIScene() { }
	void OnDestructIScene() { for (auto p : m_pDisplayObjects) SAFE_DELETE(p); }
};

