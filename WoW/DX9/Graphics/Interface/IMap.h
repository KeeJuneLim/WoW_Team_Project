#pragma once
#include "stdafx.h"
#include "Manager/Objects.h"

class IMap : public IDisplayObject
{
protected:
	
	IMap()
	{
		tag = TAG_MAP;
		Objects::AddToTagList(tag, this);
	}
	
public:
	virtual ~IMap()
	{
		Objects::RemoveFromTagList(tag, this);
	}

	virtual bool ComputeHeight(float & height, const D3DXVECTOR3 & pos) = 0;
	virtual bool ComputeClickPosition(D3DXVECTOR3 &vOut, WORD screenX, WORD screenY) = 0;
};