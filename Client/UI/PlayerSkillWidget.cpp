#include "PlayerSkillWidget.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "SkillSlotIcon.h"
#include "SkillSlot.h"
#include "PlayerSkillStateWidget.h"
CPlayerSkillWidget::CPlayerSkillWidget()
{
}

CPlayerSkillWidget::~CPlayerSkillWidget()
{
}


void CPlayerSkillWidget::CreateSkillSlot()
{

	auto BackImage = CreateWidget<CImage>("Back", 1).lock();
	BackImage->SetTexture("PlayerSkillBack", TEXT("Slot/QuickSlot.png"));
	BackImage->SetSize(150.f, 80.f);


	for (int j = 1; j <= 2; j++)
	{
		int OffsetY = 35;
		if (j == 2)
			OffsetY = 47;
		for (int i = 0; i < 4; ++i)
		{
			std::string	Name = "Slot" + i + '_' + j;


			std::shared_ptr<CSkillSlot> Slot =
				CreateWidget<CSkillSlot>(Name, 1).lock();

			Slot->SetPos(float(i * 35 + 10) , float(j * 45 - OffsetY));
			Slot->SetPrgressSlot();
			mSlotArray.push_back(Slot);
		}
	}
	auto Slot = mSlotArray[0].lock();


	Slot->SetProgressImage(EProgressBarImageType::Back, "AvengerSlot", TEXT("Slot/Avenger.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));
	Slot->SetProgressDir(EProgressBarDir::TopToBottom);

	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[1].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "Heist", TEXT("Slot/Heist.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f)); // 밝게

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f,0.2f,0.2f,0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[2].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "Shadow", TEXT("Slot/Shadow.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[3].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "Boost", TEXT("Slot/Boost.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[4].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "Throw", TEXT("Slot/Throw.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[5].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "HP", TEXT("Slot/HP.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[6].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "MP", TEXT("Slot/MP.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

	Slot = mSlotArray[7].lock();

	Slot->SetProgressImage(EProgressBarImageType::Back, "MapleMan", TEXT("Slot/MapleMan.png"));
	Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

	Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
	Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));

	Slot->SetProgressDir(EProgressBarDir::TopToBottom);
	Slot->SetProgressPercent(0.f);
	Slot->EnableIcon(true);

}
void CPlayerSkillWidget::SetAvengerSkillCoolTime(float Time,float SkillCoolTime)
{
	auto Slot = mSlotArray[0].lock();
	if (Slot)
	{
		Slot->SetProgressPercent(Time / SkillCoolTime);
	}
}

void CPlayerSkillWidget::SetWillSkillCoolTime(float Time, float SkillCoolTime)
{
	auto Slot = mSlotArray[7].lock();
	if (Slot)
	{
		Slot->SetProgressPercent(Time / SkillCoolTime);
	}
}	


bool CPlayerSkillWidget::Init()
{
	CWidgetContainer::Init();
	CreateSkillSlot();
	return true;
}

void CPlayerSkillWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CPlayerSkillWidget::Render()
{
	CWidgetContainer::Render();
}
