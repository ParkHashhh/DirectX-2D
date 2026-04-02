#include "InventoryManager.h"
#include "../Item/Item.h"
#include "Inventory.h"
#include "../Item/FieldItem.h"
#include "World/World.h"
CInventoryManager* CInventoryManager::mInst = nullptr;


CInventoryManager::CInventoryManager()
{

}

CInventoryManager::~CInventoryManager()
{
}

bool CInventoryManager::Init()
{
	
	for (int t = 0; t < EItemType::End; ++t)
	{
		mItemArray[t].resize(24);
	}

	return true;
}


void CInventoryManager::AddItem(std::shared_ptr<CItem> Item)
{
	int EmptyIdx = GetEmptyIndex(Item->GetData().Type);

	if (EmptyIdx != -1)
	{
		mItemArray[Item->GetData().Type][EmptyIdx] = Item;

		Item->SetIndex(EmptyIdx);
		RefreshInventory();
	}
}

int CInventoryManager::GetEmptyIndex(EItemType::Type Type)
{

	auto& Data = mItemArray[Type];
	int Size = (int)Data.size();

	for (int i = 0; i < Size; i++)
	{
		if (!Data[i])
			return i;
	}
	return -1;
}



void CInventoryManager::SwapItem(EItemType::Type Type, int SrcIndex, int DestIndex)
{
	std::swap(mItemArray[Type][SrcIndex], mItemArray[Type][DestIndex]);

	if (mItemArray[Type][SrcIndex])
		mItemArray[Type][SrcIndex]->SetIndex(SrcIndex);

	if (mItemArray[Type][DestIndex])
		mItemArray[Type][DestIndex]->SetIndex(DestIndex);
	RefreshInventory();
}
void CInventoryManager::RefreshInventory()
{
	auto Inventory = mInventory.lock();
	if (Inventory)
		Inventory->RefreshInventory();
}


void CInventoryManager::EraseItem(EItemType::Type Type, int Index)
{
	if (Index >= 0 && Index < (int)mItemArray[Type].size())
	{
		mItemArray[Type][Index] = nullptr;
	}
	RefreshInventory();
}

void CInventoryManager::DropItem(EItemType::Type Type, int Index)
{
	if (mItemArray[Type][Index] == nullptr)
		return;

	auto CurrentItem = mItemArray[Type][Index];

	// 2. 월드(World)를 가져와서 필드 아이템 생성
	auto Inventory = mInventory.lock();
	if (!Inventory) return;

	auto World = Inventory->GetWorld().lock();
	if (World)
	{
		auto FieldItem = World->CreateGameObject<CFieldItem>("DropItem").lock();
		FieldItem->SetItem(CurrentItem);
		auto Player = Inventory->GetPlayer().lock();
		if (Player)
		{
			FVector3 Pos = Player->GetWorldPos();
			FieldItem->SetWorldPos(Pos);
			FieldItem->StartDrop(); 
		}
	}

	EraseItem(Type, Index);

	RefreshInventory();
}

void CInventoryManager::SetItem(EItemType::Type Type, int Index, std::weak_ptr<class CItem> Item)
{
	if (Index < 0 || Index >= (int)mItemArray[Type].size())
		return;
	mItemArray[Type][Index] = Item.lock();

	auto _Item = Item.lock();
	if (_Item)
	{
		_Item->SetIndex(Index);
	}
	RefreshInventory();
}

void CInventoryManager::EquipItem(int InvenIndex, std::weak_ptr<class CItem> EquipItem)
{
	auto Item = EquipItem.lock();
	mItemArray[EItemType::Equip][InvenIndex] = Item;

	if (mItemArray[EItemType::Equip][InvenIndex])
	{
		mItemArray[EItemType::Equip][InvenIndex]->SetIndex(InvenIndex);
	}
	RefreshInventory();
}