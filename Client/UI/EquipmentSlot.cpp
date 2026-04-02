#include "EquipmentSlot.h"
#include "SlotIcon.h"
#include "UI/Image.h"
#include "../Item/Item.h"
#include "Slot.h"
#include "World/World.h"
#include "../Item/FieldItem.h"
#include "Inventory.h"
#include "../Player/Player.h"
#include "InventorySlot.h"
#include "InventoryManager.h"
CEquipmentSlot::CEquipmentSlot()
{
}

CEquipmentSlot::CEquipmentSlot(const CEquipmentSlot& ref)
{
}

CEquipmentSlot::~CEquipmentSlot()
{
}

bool CEquipmentSlot::Init()
{
	CSlot::Init();

	std::shared_ptr<CImage> Back =
		CreateWidget<CImage>("EquipmentSlotBack").lock();

	Back->SetSize(30.f, 30.f);
	Back->SetTint(FVector4(0.f, 0.f, 0.f, 0.f));

	mIcon = CreateWidget<CSlotIcon>("EquipmentSlot", 1).lock();
	mIcon->SetSlot(mSelf);
	mIcon->SetSizeImage(30.f, 30.f);
	mIcon->SetSize(30.f, 30.f);

	return true;
}


void CEquipmentSlot::SetItem(std::shared_ptr<class CItem> Item)
{
	mItem = Item;

	if (mItem)
	{
		auto Data = Item->GetData();
		mIcon->SetIconImage(Data.Name, Data.IconPath.c_str());
		mIcon->SetImageEnable(true);

	}
	else
	{
		mItem = nullptr;
		mIcon->SetImageEnable(false);	
	}
}

void CEquipmentSlot::Update(float DeltaTime)
{
	CSlot::Update(DeltaTime);
}

void CEquipmentSlot::Render()
{
	CSlot::Render();
}

CEquipmentSlot* CEquipmentSlot::Clone()	const
{
	return new CEquipmentSlot(*this);
}
bool CEquipmentSlot::OnDrop(std::weak_ptr<CWidget>& Result)
{
	auto DropIcon = std::dynamic_pointer_cast<CSlotIcon>(Result.lock());
	if (!DropIcon)
		return false;
	auto Slot = DropIcon->GetSlot().lock();
	if (!Slot)
		return false;
	auto DropSlotEquip = std::dynamic_pointer_cast<CInventorySlot>(Slot);
	if (DropSlotEquip)
	{
		if (DropSlotEquip->GetCurrentTab() != EItemType::Equip)
			return false;
		std::shared_ptr<CItem> DestItem = mItem;
		std::shared_ptr<CItem> TempItem = DropSlotEquip->GetItem();

		CInventoryManager::GetInst()->EquipItem(TempItem->GetIndex(), mItem);
		SetItem(TempItem);
		return true;
	}
	return false;
}
bool CEquipmentSlot::OutDrop()
{
	
	return false;
}