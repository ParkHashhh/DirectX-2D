#include "Equipment.h"
#include "UI/Button.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "UI/TitleBar.h"
#include "Slot.h"
#include "EquipmentSlot.h"
#include "../Item/Item.h"
#include "World/World.h"
CEquipment::CEquipment()
{
}

CEquipment::~CEquipment()
{
}

bool CEquipment::Init()
{
	CWidgetContainer::Init();
	std::shared_ptr<CTitleBar> TitleBar =
		CreateWidget<CTitleBar>("TitleBar").lock();

	TitleBar->SetSize(180, 13);
	TitleBar->SetOpacity(0.f);
	TitleBar->SetUpdateWidget(mSelf);

	std::shared_ptr<CImage> Back =
		CreateWidget<CImage>("Back").lock();
	Back->SetPos(0.f, 0.f);
	Back->SetSize(184, 290);
	Back->SetTexture("EquipBack", TEXT("Slot/Equip.png"));

	Back = CreateWidget<CImage>("Back1").lock();
	Back->SetPos(6.f, 23.f);
	Back->SetSize(172, 262);
	Back->SetTexture("EquipBack1", TEXT("Slot/Equip1.png"));
	
	Back = CreateWidget<CImage>("Back2").lock();
	Back->SetPos(10.f, 33.f);
	Back->SetSize(164, 240);
	Back->SetTexture("EquipBack2", TEXT("Slot/Equip2.png"));


	SetItemSlot();

	return true;
}

void CEquipment::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CEquipment::Render()
{
	CWidgetContainer::Render();
}

void CEquipment::SetZakumEquip()
{
	auto Item = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();

	//모자
	FDropItemData CapData;
	CapData.Name = "Cap";
	CapData.IconPath = TEXT("Equip/Brownbamboo.png");
	CapData.Type = EItemType::Equip;
	Item->SetData(CapData);
	auto Slot = mSlotArray[0].lock();
	Slot->SetItem(Item);
	Slot->AddPos(FVector2(43, 35.f));


	//눈장식
	auto Item1 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData EyeData;
	EyeData.IconPath = TEXT("Equip/DragonSlave.png");
	EyeData.Name = "Eye";
	EyeData.Type = EItemType::Equip;
	Item1->SetData(EyeData);
	Slot = mSlotArray[1].lock();
	Slot->SetEnable(false);
	Slot->SetOpacity(0.f);
	Slot->SetItem(Item1);
	Slot->AddPos(FVector2(43, 105.f));

	//상의 
	auto Item2 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData TopData;
	TopData.Name = "Top";
	TopData.Type = EItemType::Equip;
	TopData.IconPath = TEXT("Equip/BlueGown.png");
	Item2->SetData(TopData);
	Slot = mSlotArray[2].lock();
	Slot->SetItem(Item2);
	Slot->AddPos(FVector2(43, 140.f));

	//무기 
	auto Item3 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData WeaponData;
	WeaponData.Name = "Weapon";
	WeaponData.Type = EItemType::Equip;
	WeaponData.IconPath = TEXT("Equip/MapleKandeo.png");
	Item3->SetData(WeaponData);
	Slot = mSlotArray[3].lock();
	Slot->SetItem(Item3);
	Slot->AddPos(FVector2(110, 140.f));

	//망토 
	auto Item4 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();

	FDropItemData CloakData;
	CloakData.Name = "Cloak";
	CloakData.Type = EItemType::Equip;
	CloakData.IconPath = TEXT("Equip/ShabbyCloak.png");
	Item4->SetData(CloakData);
	Slot = mSlotArray[4].lock();
	Slot->SetItem(Item4);
	Slot->AddPos(FVector2(9, 140.f));


	//하의 
	auto Item5 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData BottomsData;
	BottomsData.Name = "Bottoms";
	BottomsData.Type = EItemType::Equip;
	BottomsData.IconPath = TEXT("Slot/DragonSlave.png");
	Item5->SetData(BottomsData);
	Slot = mSlotArray[5].lock();
	Slot->SetEnable(false);
	Slot->SetOpacity(0.f);
	Slot->SetItem(Item5);
	Slot->AddPos(FVector2(43, 172.f));

	//장갑 
	auto Item6 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData GlovesData;
	GlovesData.Type = EItemType::Equip;
	GlovesData.Name = "Gloves";
	GlovesData.IconPath = TEXT("Equip/MokGlove.png");
	Item6->SetData(GlovesData);
	Slot = mSlotArray[6].lock();
	Slot->SetItem(Item6);
	Slot->AddPos(FVector2(9, 172.f));


	//신발
	auto Item7 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData ShoesData;
	ShoesData.Name = "Shoes";
	ShoesData.Type = EItemType::Equip;
	ShoesData.IconPath = TEXT("Equip/BlackIgen.png");
	Item7->SetData(ShoesData);
	Slot = mSlotArray[7].lock();
	Slot->SetItem(Item7);
	Slot->AddPos(FVector2(78, 208.f));
}

void CEquipment::SetHornTailEquip()
{
	auto Item = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();

	//모자
	FDropItemData CapData;
	CapData.Name = "Cap";
	CapData.IconPath = TEXT("Equip/ZakumHead.png");
	CapData.Type = EItemType::Equip;
	Item->SetData(CapData);
	auto Slot = mSlotArray[0].lock();
	Slot->SetItem(Item);
	Slot->AddPos(FVector2(43, 35.f));


	//눈장식
	auto Item1 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData EyeData;
	EyeData.IconPath = TEXT("Equip/Crackedglasses.png");
	EyeData.Name = "Eye";
	EyeData.Type = EItemType::Equip;

	Item1->SetData(EyeData);
	Slot = mSlotArray[1].lock();
	Slot->SetItem(Item1);
	Slot->AddPos(FVector2(43, 105.f));

	//상의 
	auto Item2 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData TopData;
	TopData.Name = "Top";
	TopData.IconPath = TEXT("Equip/DarkPirate.png");
	TopData.Type = EItemType::Equip;
	Item2->SetData(TopData);
	Slot = mSlotArray[2].lock();
	Slot->SetItem(Item2);
	Slot->AddPos(FVector2(43, 140.f));

	//무기 
	auto Item3 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData WeaponData;
	WeaponData.Name = "Weapon";
	WeaponData.IconPath = TEXT("Equip/MapleScanda.png");
	WeaponData.Type = EItemType::Equip;
	Item3->SetData(WeaponData);
	Slot = mSlotArray[3].lock();
	Slot->SetItem(Item3);
	Slot->AddPos(FVector2(110, 140.f));

	//망토 
	auto Item4 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();

	FDropItemData CloakData;
	CloakData.Name = "Cloak";
	CloakData.IconPath = TEXT("Equip/IcarusCloak.png");
	CloakData.Type = EItemType::Equip;
	Item4->SetData(CloakData);
	Slot = mSlotArray[4].lock();
	Slot->SetItem(Item4);
	Slot->AddPos(FVector2(9, 140.f));


	//하의 
	auto Item5 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData BottomsData;
	BottomsData.Name = "Bottoms";
	BottomsData.IconPath = TEXT("Equip/DarkPirateBottom.png");
	BottomsData.Type = EItemType::Equip;
	Item5->SetData(BottomsData);
	Slot = mSlotArray[5].lock();
	Slot->SetItem(Item5);
	Slot->AddPos(FVector2(43, 172.f));

	//장갑 
	auto Item6 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData GlovesData;
	GlovesData.Name = "Gloves";
	GlovesData.IconPath = TEXT("Equip/BloodRover.png");
	GlovesData.Type = EItemType::Equip;
	Item6->SetData(GlovesData);
	Slot = mSlotArray[6].lock();
	Slot->SetItem(Item6);
	Slot->AddPos(FVector2(9, 172.f));


	//신발
	auto Item7 = mWorld.lock()->CreateGameObject<CItem>("EquipItem" + std::to_string(rand())).lock();
	FDropItemData ShoesData;
	ShoesData.Name = "Shoes";
	ShoesData.IconPath = TEXT("Equip/DarkGatina.png");
	ShoesData.Type = EItemType::Equip;
	Item7->SetData(ShoesData);
	Slot = mSlotArray[7].lock();
	Slot->SetItem(Item7);
	Slot->AddPos(FVector2(78, 208.f));
}


void CEquipment::SetItemSlot()
{

	for (int i = 0; i < 8; i++)
	{
		auto Slot = CreateWidget<CEquipmentSlot>("EquipmentSlot_" + std::to_string(i),1).lock();
		mSlotArray.push_back(Slot);
	}
	
}
