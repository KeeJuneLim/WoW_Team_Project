#pragma once

//실행 중 생성해 놓은 객체들을 관리

class IMap;

class Objects
{
public:
	static void		Delete();

	/////TagList/////
private:
	static map<WORD, list<IDisplayObject*>> m_tagList;
public:
	static void AddToTagList(WORD _tag, IDisplayObject* _pObject);
	static void RemoveFromTagList(WORD _tag, IDisplayObject* _pObject);
	static IDisplayObject* FindObjectByTag(WORD _tag);
	static list<IDisplayObject*> FindObjectsByTag(WORD _tag);
	/////TagList/////

	
public:
	static IDisplayObject* GetPlayer();
	static IMap* GetCurrentMap();
	
};

