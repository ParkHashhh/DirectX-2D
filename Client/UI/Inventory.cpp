#include "Inventory.h"
#include "UI/Button.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "UI/TitleBar.h"
#include "Slot.h"
#include "InventorySlot.h"
#include "../Item/Item.h"
#include "InventoryManager.h"

CInventory::CInventory()
{
}

CInventory::~CInventory()
{
}

bool CInventory::Init()
{
	CWidgetContainer::Init();
	CInventoryManager::GetInst()->SetInventoryUI(std::static_pointer_cast<CInventory>(mSelf.lock()));

	std::shared_ptr<CTitleBar> TitleBar =
		CreateWidget<CTitleBar>("TitleBar").lock();	

	TitleBar->SetSize(160, 13);
	TitleBar->SetUpdateWidget(mSelf);

	std::shared_ptr<CImage> Back =
		CreateWidget<CImage>("Back").lock();
	Back->SetPos(0.f, 0.f);
	Back->SetSize(175, 289);
	Back->SetTexture("InventoryBack", TEXT("Slot/Inventory.png"));

	mTabButtons[EItemType::Equip] =
		CreateWidget<CButton>("Equip",1).lock();

	mTabButtons[EItemType::Equip]->SetPos(2.f, 24.f);
	mTabButtons[EItemType::Equip]->SetSize(34.f, 18.f);
	mTabButtons[EItemType::Equip]->SetTexture(EButtonState::Normal, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Equip]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Equip]->SetTexture(EButtonState::Hovered, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Equip]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Equip]->SetTexture(EButtonState::Click, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Equip]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Equip]->SetTexture(EButtonState::Disable, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Equip]->SetEventCallback<CInventory>(EButtonEventState::Click, this, &CInventory::EquipButtonClick);

	std::shared_ptr<CImage> EquipText = CreateWidget<CImage>("EquipText",1).lock();
	EquipText->SetSize(22, 14);
	EquipText->AddPos(FVector2(8, 28));
	EquipText->SetTexture("EquipText", TEXT("Slot/Equip_Text.png"));


	mTabButtons[EItemType::Consume] =
		CreateWidget<CButton>("Consume", 1).lock();

	mTabButtons[EItemType::Consume]->SetPos(36.f, 24.f);
	mTabButtons[EItemType::Consume]->SetSize(34.f, 18.f);
	mTabButtons[EItemType::Consume]->SetTexture(EButtonState::Normal, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Consume]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Consume]->SetTexture(EButtonState::Hovered, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Consume]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Consume]->SetTexture(EButtonState::Click, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Consume]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Consume]->SetTexture(EButtonState::Disable, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Consume]->SetEventCallback<CInventory>(EButtonEventState::Click, this, &CInventory::ConsumeButtonClick);
	std::shared_ptr<CImage> ConsumeText = CreateWidget<CImage>("ConsumeText", 1).lock();
	ConsumeText->SetSize(22, 12);
	ConsumeText->AddPos(FVector2(42, 29));
	ConsumeText->SetTexture("ConsumeText", TEXT("Slot/Consume_Text.png"));


	mTabButtons[EItemType::Install] =
		CreateWidget<CButton>("Install", 1).lock();

	mTabButtons[EItemType::Install]->SetPos(70.f, 24.f);
	mTabButtons[EItemType::Install]->SetSize(34.f, 18.f);
	mTabButtons[EItemType::Install]->SetTexture(EButtonState::Normal, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Install]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Install]->SetTexture(EButtonState::Hovered, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Install]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Install]->SetTexture(EButtonState::Click, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Install]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Install]->SetTexture(EButtonState::Disable, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Install]->SetEventCallback<CInventory>(EButtonEventState::Click, this, &CInventory::InstallButtonClick);

	std::shared_ptr<CImage> InstallText = CreateWidget<CImage>("InstallText", 1).lock();
	InstallText->SetSize(22, 14);
	InstallText->AddPos(FVector2(75, 27));
	InstallText->SetTexture("InstallText", TEXT("Slot/Install_Text.png"));


	mTabButtons[EItemType::Etc] =
		CreateWidget<CButton>("Etc", 1).lock();

	mTabButtons[EItemType::Etc]->SetPos(104.f, 24.f);
	mTabButtons[EItemType::Etc]->SetSize(34.f, 18.f);
	mTabButtons[EItemType::Etc]->SetTexture(EButtonState::Normal, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Etc]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Etc]->SetTexture(EButtonState::Hovered, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Etc]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Etc]->SetTexture(EButtonState::Click, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Etc]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Etc]->SetTexture(EButtonState::Disable, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Etc]->SetEventCallback<CInventory>(EButtonEventState::Click, this, &CInventory::EtcButtonClick);


	std::shared_ptr<CImage> EtcText = CreateWidget<CImage>("EtcText", 1).lock();
	EtcText->SetSize(22, 12);
	EtcText->AddPos(FVector2(110, 29));
	EtcText->SetTexture("EtcText", TEXT("Slot/Etc_Text.png"));

	mTabButtons[EItemType::Cash] =
		CreateWidget<CButton>("Cash", 1).lock();

	mTabButtons[EItemType::Cash]->SetPos(138.f, 24.f);
	mTabButtons[EItemType::Cash]->SetSize(34.f, 18.f);
	mTabButtons[EItemType::Cash]->SetTexture(EButtonState::Normal, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Cash]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Cash]->SetTexture(EButtonState::Hovered, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Cash]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Cash]->SetTexture(EButtonState::Click, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Cash]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[EItemType::Cash]->SetTexture(EButtonState::Disable, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[EItemType::Cash]->SetEventCallback<CInventory>(EButtonEventState::Click, this, &CInventory::CashButtonClick);

	std::shared_ptr<CImage> CashText = CreateWidget<CImage>("CashText", 1).lock();
	CashText->SetSize(21, 12);
	CashText->AddPos(FVector2(144, 29));
	CashText->SetTexture("CashText", TEXT("Slot/Cash_Text.png"));

	SetItemSlot();

	mCurrentTab = EItemType::Equip;
	ChangeOnTab(EItemType::Equip);

	RefreshInventory();

	return true;
}

void CInventory::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CInventory::Render()
{
	CWidgetContainer::Render();
}

void CInventory::AddItem(std::shared_ptr<CItem> Item)
 {
	CInventoryManager::GetInst()->AddItem(Item);
	RefreshInventory();
}



void CInventory::ChangeOnTab(EItemType::Type Type)
{
	mTabButtons[Type]->SetTexture(EButtonState::Normal, "TabOn",
		TEXT("Slot/TabOn_Back.png"));
	mTabButtons[Type]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[Type]->SetTexture(EButtonState::Hovered, "TabOn",
		TEXT("Slot/TabOn_Back.png"));
	mTabButtons[Type]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[Type]->SetTexture(EButtonState::Click, "TabOn",
		TEXT("Slot/TabOn_Back.png"));
	mTabButtons[Type]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[Type]->SetTexture(EButtonState::Disable, "TabOn",
		TEXT("Slot/TabOn_Back.png"));

}

void CInventory::ChangeOffTab(EItemType::Type Type)
{
	mTabButtons[Type]->SetTexture(EButtonState::Normal, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[Type]->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[Type]->SetTexture(EButtonState::Hovered, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[Type]->SetTint(EButtonState::Hovered, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[Type]->SetTexture(EButtonState::Click, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
	mTabButtons[Type]->SetTint(EButtonState::Click, FVector4(1.f, 1.f, 1.f, 1.f));
	mTabButtons[Type]->SetTexture(EButtonState::Disable, "InvenEquipButton",
		TEXT("Slot/Tab_Back.png"));
}

void CInventory::EquipButtonClick()
{
	ChangeTab(EItemType::Equip);
}

void CInventory::ConsumeButtonClick()
{
	ChangeTab(EItemType::Consume);
}


void CInventory::InstallButtonClick()
{
	ChangeTab(EItemType::Install);
}


void CInventory::EtcButtonClick()
{
	ChangeTab(EItemType::Etc);
}


void CInventory::CashButtonClick()
{
	ChangeTab(EItemType::Cash);
}

void CInventory::SetItemSlot()
{
	float StartX = 9.f;
	float StartY = 52.f;
	float SlotSize = 30.f; 
	float OffsetY = 4.f;   
	float OffsetX = 6.f;  
	float StepX = SlotSize + OffsetX;
	float StepY = SlotSize + OffsetY;
	for (int i = 0; i < 24; i++)
	{
		auto Slot = CreateWidget<CInventorySlot>("InventorySlot_" + std::to_string(i),1).lock();
		int Row = i / 4;
		int Col = i % 4;
		Slot->SetSlotIndex(i);
		Slot->SetInventory(mSelf);
		Slot->SetSize(SlotSize, SlotSize);
		Slot->SetPos(StartX + (Col * StepX), StartY + (Row * StepY));
		mSlotArray.push_back(Slot);
	}


}

void CInventory::ChangeTab(EItemType::Type Type)
{
	if (mCurrentTab != Type)
	{
		ChangeOnTab(Type); // On색깔있는거
		ChangeOffTab(mCurrentTab); // Off색깔
	}
	mCurrentTab = Type;
	RefreshInventory();
}

void CInventory::RefreshInventory()
{
	auto& Data = CInventoryManager::GetInst()->GetItemArray(mCurrentTab);

	auto iter = Data.begin();
	auto iterEnd = Data.end();
	
	int SlotSize = (int)mSlotArray.size();
	int DataSize = (int)Data.size();


	for (int i = 0; i < SlotSize; i++)
	{
		mSlotArray[i].lock()->SetItem(nullptr);
	}

	for (int i = 0; i < DataSize; i++)
	{
		if (Data[i] == nullptr)
			continue;
		auto Item = Data[i];
		int Idx = Item->GetIndex();

		if (Idx >= 0 && Idx < SlotSize)
		{
			auto TargetSlot = mSlotArray[Idx].lock();
			if (TargetSlot)
			{
				TargetSlot->SetItem(Item);
			}
		}
	}
}

int CInventory::GetEmptyIndex(EItemType::Type Type)
{
	return CInventoryManager::GetInst()->GetEmptyIndex(Type);
}

void CInventory::SwapItem(EItemType::Type Type, int SrcIndex, int DestIndex)
{
	CInventoryManager::GetInst()->SwapItem(Type, SrcIndex, DestIndex);
}

void CInventory::EraseItem(EItemType::Type Type, int Index)
{
	CInventoryManager::GetInst()->EraseItem(Type, Index);
}

