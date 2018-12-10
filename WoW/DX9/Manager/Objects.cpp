#include "stdafx.h"
#include "Objects.h"


map<WORD, list<IDisplayObject*>> Objects::m_tagList;

void Objects::Delete()
{
	UINT count = 0;
	for (auto i : m_tagList)
		for (auto j : i.second)
			if (j) count++;

	//assert(count == 0 && "생성된 객체 중 삭제되지 않은 객체가 있습니다.");
}


void Objects::AddToTagList(WORD _tag, IDisplayObject * _object)
{
	m_tagList[_tag].push_back(_object);
}

void Objects::RemoveFromTagList(WORD _tag, IDisplayObject* _pObject)
{
	m_tagList[_tag].remove(_pObject);
}

IDisplayObject * Objects::FindObjectByTag(WORD _tag)
{
	if (m_tagList[_tag].empty() == true) return NULL;

	return m_tagList[_tag].front();
}

list<IDisplayObject*> Objects::FindObjectsByTag(WORD _tag)
{
	return m_tagList[_tag];
}

IDisplayObject * Objects::GetPlayer()
{
	return (IDisplayObject*)FindObjectByTag(TAG_PLAYER);
}

IMap * Objects::GetCurrentMap()
{
	return (IMap*)FindObjectByTag(TAG_MAP);
}

