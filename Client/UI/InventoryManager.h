#pragma once

#include "EngineInfo.h"
#include "../ClientInfo.h"

class CInventoryManager
{
private:
	CInventoryManager();
	~CInventoryManager();

private:
	static CInventoryManager* mInst;

public:
	void RefreshInventory();
	static CInventoryManager* GetInst()
	{
		if (!mInst)
			mInst = new CInventoryManager;
		return mInst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(mInst);
	}

private:
	std::vector<std::shared_ptr<class CItem>> mItemArray[EItemType::End];
	std::weak_ptr<class CInventory> mInventory;
public:
	std::vector<std::shared_ptr<class CItem>>& GetItemArray(EItemType::Type Type)
	{
		return mItemArray[Type];
	}

	void SetInventoryUI(std::shared_ptr<class CInventory> Inventory)
	{
		mInventory = Inventory;
	}

public:
	bool Init();
	void AddItem(std::shared_ptr<CItem> Item);
	int GetEmptyIndex(EItemType::Type Type);
	void SwapItem(EItemType::Type Type, int SrcIndex, int DestIndex);
	void EraseItem(EItemType::Type Type, int Index);
	void DropItem(EItemType::Type Type, int Index);
	void SetItem(EItemType::Type Type, int Index, std::weak_ptr<class CItem> Item);
	void EquipItem(int InvenIndex, std::weak_ptr<class CItem> Item);
};

