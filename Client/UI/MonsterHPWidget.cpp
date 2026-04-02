#include "MonsterHPWidget.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"

#include "SlotIcon.h"
#include "SkillSlot.h"
CMonsterHPWidget::CMonsterHPWidget()
{
}

CMonsterHPWidget::~CMonsterHPWidget()
{
}

void CMonsterHPWidget::SetMonsterHP(float HP, float HPMax)
{
	auto	HPBar = mHPBar.lock();

	HPBar->SetPercent(HP / HPMax);
}

void CMonsterHPWidget::SetIcon(const std::string& Name, const TCHAR* FileName)
{
	auto Icon = mIcon.lock();
	if (Icon)
	{
		Icon->SetTexture(Name, FileName);
	}
}


bool CMonsterHPWidget::Init()
{
	CWidgetContainer::Init();
	mBackImage = CreateWidget<CImage>("Back", 1).lock();
	auto BackImage = mBackImage.lock();
	if (BackImage)
	{
		BackImage->SetTexture("BossHpBar", TEXT("Slot/BossHpBar.png"));
		BackImage->SetSize(900.f, 40.f);
	}
	mIcon = CreateWidget<CImage>("Icon",1);
	auto Icon = mIcon.lock();
	if (Icon)
	{
		Icon->SetTexture("HornTailIcon", TEXT("Slot/HornTailIcon.png"));
		Icon->SetSize(30.f, 30.f);
		Icon->AddPos(FVector2(5, 3));
	}

	mHPBar = CreateWidget<CProgressBar>("BossHPBar",1);

	auto	HPBar = mHPBar.lock();

	HPBar->SetSize(853.f, 10.f);
	HPBar->SetTexture(EProgressBarImageType::Fill, "HPBar",
		TEXT("Slot/HPGauge.png"));
	HPBar->AddPos(FVector2(43, 5));
	HPBar->SetTint(EProgressBarImageType::Back,
		0.3f, 0.3f, 0.3f, 1.f);

	HPBar->SetBarDir(EProgressBarDir::RightToLeft);

	HPBar->SetPercent(1.f);

	return true;
}

void CMonsterHPWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CMonsterHPWidget::Render()
{
	CWidgetContainer::Render();
}
