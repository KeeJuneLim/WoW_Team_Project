#pragma once

//프로그램 전체의 랜더링을 관리

class IDisplayObject;

class Rendering
{
public:

	static Rendering*	Get();
	static void		Delete();
private:
	static Rendering*	instance;
	Rendering();
	~Rendering();

	vector<list<IDisplayObject*>> renderingList;
	list<IDisplayObject*> shadowCastList;
public:
	void Init();
	void Render();

	void AddRenderingObject(IDisplayObject* pObj);
	void RemoveRenderingObject(IDisplayObject* pObj);

	void AddShadowCastObject(IDisplayObject* pObj)
	{		shadowCastList.push_back(pObj);	}
	void RemoveShadowCastObject(IDisplayObject* pObj)
	{
		shadowCastList.remove(pObj);
	}
	
};