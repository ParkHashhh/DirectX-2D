#include "PlayerStateWidget.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "SlotIcon.h"
#include "SkillSlot.h"
CPlayerStateWidget::CPlayerStateWidget()
{
}

CPlayerStateWidget::~CPlayerStateWidget()
{
}

void CPlayerStateWidget::SetPlayerName(const TCHAR* Name)
{
	auto	Text = mNameText.lock();

	Text->SetText(Name);
}

void CPlayerStateWidget::SetPlayerHP(float HP, float HPMax)
{
	auto	HPBar = mHPBar.lock();
	auto Text = mHPText.lock();

	std::wstring hpStr = std::to_wstring(int(HP)) + L" / " + std::to_wstring(int(HPMax));
	Text->SetText(hpStr.c_str());

	HPBar->SetPercent(HP / HPMax);
}

void CPlayerStateWidget::SetPlayerMP(float MP, float MPMax)
{
	auto	MPBar = mMPBar.lock();
	auto Text = mMPText.lock();

	std::wstring hpStr = std::to_wstring(int(MP)) + L" / " + std::to_wstring(int(MPMax));

	Text->SetText(hpStr.c_str());

	MPBar->SetPercent(MP / MPMax);
}

void CPlayerStateWidget::MultipleWidth(float Width)
{
	auto	Text = mNameText.lock();
	auto BackImage = mBackImage.lock();
	auto	HPBar = mHPBar.lock();
	auto	MPBar = mMPBar.lock();

	Text->SetSize(Text->GetSize().x * Width, Text->GetSize().y);
	BackImage->SetSize(BackImage->GetSize().x * Width, BackImage->GetSize().y);
	HPBar->SetSize(HPBar->GetSize().x * Width, HPBar->GetSize().y);
	MPBar->SetSize(MPBar->GetSize().x * Width, MPBar->GetSize().y);

}

void CPlayerStateWidget::MultipleHeight(float Height)
{
	auto	Text = mNameText.lock();
	auto BackImage = mBackImage.lock();
	auto	HPBar = mHPBar.lock();
	auto	MPBar = mMPBar.lock();

	Text->SetSize(Text->GetSize().x, Text->GetSize().y * Height);
	BackImage->SetSize(BackImage->GetSize().x, BackImage->GetSize().y * Height);
	HPBar->SetSize(HPBar->GetSize().x, HPBar->GetSize().y * Height);
	MPBar->SetSize(MPBar->GetSize().x, MPBar->GetSize().y * Height);

}

void CPlayerStateWidget::CreateStateWidget()
{
	mBackImage = CreateWidget<CImage>("Back", 1).lock();
	auto BackImage = mBackImage.lock();
	BackImage->SetTexture("PlayerStateBack", TEXT("Slot/HPSlot.png"));
	BackImage->SetSize(200.f, 80.f);

	mNameText = CreateWidget<CTextBlock>("Text", 2);

	auto	Text = mNameText.lock();

	Text->SetText(TEXT("zl존주석123"));
	Text->SetTextColor(255, 255, 255, 255);
	Text->SetSize(200.f, 26.f);
	Text->SetFontSize(15.f);
	Text->SetAlignH(ETextAlignH::Center);

	mHPText = CreateWidget<CTextBlock>("Text", 2);
	auto	Text1 = mHPText.lock();
	Text1->SetTextColor(255, 255, 255, 255);
	Text1->SetSize(200.f, 26.f);
	Text1->SetFontSize(10.f);
	Text1->SetAlignH(ETextAlignH::Center);
	Text1->AddPos(FVector2(50, 33.f));

	mMPText = CreateWidget<CTextBlock>("Text", 2);
	auto	Text2 = mMPText.lock();
	Text2->SetTextColor(255, 255, 255, 255);
	Text2->SetSize(200.f, 26.f);
	Text2->SetFontSize(10.f);
	Text2->SetAlignH(ETextAlignH::Center);
	Text2->AddPos(FVector2(50, 56.f));


	mHPBar = CreateWidget<CProgressBar>("HPBar");

	auto	HPBar = mHPBar.lock();

	HPBar->SetPos(24.f, 40.f);
	HPBar->SetSize(180.f, 14.f);
	HPBar->SetTexture(EProgressBarImageType::Fill, "HPBar",
		TEXT("Slot/HPGauge.png"));

	HPBar->SetTint(EProgressBarImageType::Back,
		0.3f, 0.3f, 0.3f, 1.f);

	HPBar->SetBarDir(EProgressBarDir::RightToLeft);

	HPBar->SetPercent(1.f);

	mMPBar = CreateWidget<CProgressBar>("MPBar");

	auto	MPBar = mMPBar.lock();

	MPBar->SetPos(24.f, 60.f);
	MPBar->SetSize(180.f, 14.f);
	MPBar->SetTexture(EProgressBarImageType::Fill, "MPBar",
		TEXT("Slot/MPGauge.png"));

	MPBar->SetTint(EProgressBarImageType::Back,
		0.3f, 0.3f, 0.3f, 1.f);

	MPBar->SetBarDir(EProgressBarDir::RightToLeft);

	MPBar->SetPercent(1.f);

}


bool CPlayerStateWidget::Init()
{
	CWidgetContainer::Init();

	CreateStateWidget();
	return true;
}

void CPlayerStateWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CPlayerStateWidget::Render()
{
	CWidgetContainer::Render();
}
