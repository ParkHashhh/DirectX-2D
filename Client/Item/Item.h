#pragma once

#include "Object/GameObject.h"
#include "../ClientInfo.h"


class CItem :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;
public:
	CItem();
	CItem(const CItem& ref);
	CItem(CItem&& ref) noexcept;
public:
	virtual ~CItem();


private:
	FDropItemData mItemData;
	int mIndex = -1;

	
public:
	int GetIndex()
	{
		return mIndex;
	}
	void SetIndex(int Index)
	{
		mIndex = Index;
	}
	FDropItemData GetData()
	{
		return mItemData;
	}
	void SetData(FDropItemData& Item)
	{
		mItemData = Item;
	}


};