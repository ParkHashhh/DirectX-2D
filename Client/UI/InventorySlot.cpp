#include "InventorySlot.h"
#include "SlotIcon.h"
#include "UI/Image.h"
#include "../Item/Item.h"
#include "Slot.h"
#include "World/World.h"
#include "../Item/FieldItem.h"
#include "Inventory.h"
#include "../Player/Player.h"
#include "EquipmentSlot.h"
#include "InventoryManager.h"

CInventorySlot::CInventorySlot()
{
}

CInventorySlot::CInventorySlot(const CInventorySlot& ref)
{
}

CInventorySlot::~CInventorySlot()
{
}

bool CInventorySlot::Init()
{
	CSlot::Init();

	std::shared_ptr<CImage> Back =
		CreateWidget<CImage>("BackSlot").lock();

	Back->SetSize(30.f, 30.f);
	Back->SetTint(FVector4(1.f, 1.f, 1.f, 1.f));

	mIcon = CreateWidget<CSlotIcon>("SlotIconUI", 1).lock();
	mIcon->SetSlot(mSelf);
	mIcon->SetSizeImage(30.f, 30.f);
	mIcon->SetSize(30.f, 30.f);
	mIcon->SetEnable(false);

	return true;
}

EItemType::Type CInventorySlot::GetCurrentTab()
{
	auto Owner = std::dynamic_pointer_cast<CInventory>(mOwner.lock());
	if (Owner)
	{
		return Owner->GetCurrentTab();
	}
	return  EItemType::End;
}
void CInventorySlot::SetItem(std::shared_ptr<class CItem> Item)
{
	mItem = Item;

	if (mItem)
	{
		auto Data = Item->GetData();
		mIcon->SetIconImage(Data.Name, Data.IconPath.c_str());
		mIcon->SetEnable(true);
		mItem->SetIndex(mSlotIndex);
	}
	else
	{
		mItem = nullptr;
		mIcon->SetEnable(false);
	}
}

void CInventorySlot::Update(float DeltaTime)
{
	CSlot::Update(DeltaTime);
}

void CInventorySlot::Render()
{
	CSlot::Render();
}

CInventorySlot* CInventorySlot::Clone()	const
{
	return new CInventorySlot(*this);
}

bool CInventorySlot::OnDrop(std::weak_ptr<CWidget>& Result)
{
	auto DropIcon = std::dynamic_pointer_cast<CSlotIcon>(Result.lock());
	auto InventoryManager = CInventoryManager::GetInst();
	if (!DropIcon)
		return false;
	auto Slot = DropIcon->GetSlot().lock();
	if (!Slot)
		return false;
	auto DropSlotEquip = std::dynamic_pointer_cast<CEquipmentSlot>(Slot);
	if (DropSlotEquip)
	{
		if (GetCurrentTab() != EItemType::Equip)
			return false;
		std::shared_ptr<CItem> DestItem = mItem;
		std::shared_ptr<CItem> TempItem = DropSlotEquip->GetItem();
		DropSlotEquip->SetItem(DestItem);
		InventoryManager->SetItem(GetCurrentTab(), GetSlotIndex(), TempItem);
		return true;
	}
	auto DropSlot = std::dynamic_pointer_cast<CInventorySlot>(Slot);
	if (!DropSlot)
		return false;
	if (DropSlot.get() == this) // 같은 슬롯일때
		return true; 
	auto Inventory = std::dynamic_pointer_cast<CInventory>(mOwner.lock());
	if (!Inventory)
		return false;

	std::shared_ptr<CItem> DestItem = mItem;
	std::shared_ptr<CItem> TempItem = DropSlot->GetItem();

	EItemType::Type ItemType;
	if (TempItem)
		ItemType = TempItem->GetData().Type;
	else
		ItemType = DestItem->GetData().Type;
	InventoryManager->SwapItem(ItemType, DropSlot->GetSlotIndex(),GetSlotIndex());
	

	//InventoryManager->SetItem(GetCurrentTab(), TempItem->GetIndex(), TempItem);
	//DropSlot->SetItem(DestItem);

	return true;
}
bool CInventorySlot::OutDrop()
{
	auto CurrentItem = GetItem();
	if (!CurrentItem) 
		return false;

	CInventoryManager::GetInst()->DropItem(GetCurrentTab(), GetSlotIndex());

	return true;
}