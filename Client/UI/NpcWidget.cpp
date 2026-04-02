#include "NpcWidget.h"
#include "UI/Button.h"
#include "UI/Image.h"
#include "Device.h"
#include "Engine.h"
#include "World/WorldManager.h"
#include "../World/MainWorld.h"
#include "../World/LoadingWorld.h"
#include "UI/TextBlock.h"
#include "../Player/Player.h"
CNpcWidget::CNpcWidget()
{
}

CNpcWidget::~CNpcWidget()
{
}

bool CNpcWidget::Init()
{
	CWidgetContainer::Init();

	FResolution	RS = CDevice::GetInst()->GetResolution();

	std::shared_ptr<CImage> Top = CreateWidget<CImage>("YesNoBox_Top").lock();
	Top->SetPos(RS.Width * 0.5f - 133, RS.Height * 0.5f - 231);
	Top->SetSize(266, 21);
	Top->SetTexture("YesNoBox_Top", TEXT("UI/YesNoBox_Top.png"));

	std::shared_ptr<CImage> Middle = CreateWidget<CImage>("YesNoBox_Middle").lock();
	Middle->SetSize(266, 63);
	Middle->SetPos(RS.Width * 0.5f - 133, RS.Height * 0.5f - 211);
	Middle->SetTexture("YesNoBox_Middle", TEXT("UI/YesNoBox_Middle.png"));

	std::shared_ptr<CTextBlock> Text =
		CreateWidget<CTextBlock>("EndingText", 1).lock();

	FVector3	TextPos = Middle->GetPos();
	TextPos.x +=90;
	TextPos.y += 11;
	Text->SetText(TEXT("저 예쁜가요 ?"));
	Text->SetTextColor(255, 255, 255, 255);
	Text->SetPos(TextPos);
	Text->SetSize(200, 50);
	Text->SetFontSize(13.f);

	std::shared_ptr<CImage> Bottom = CreateWidget<CImage>("YesNoBox_Bottom").lock();
	Bottom->SetPos(RS.Width * 0.5f - 133, RS.Height * 0.5f - 150);
	Bottom->SetSize(266, 55);
	Bottom->AddPos(FVector2(0.f, 1.f));
	Bottom->SetTexture("YesNoBox_Bottom", TEXT("UI/YesNoBox_Bottom.png"));


	std::shared_ptr<CButton> OKButton =
		CreateWidget<CButton>("OKButton", 1).lock();

	FVector3	ButtonPos = Bottom->GetPos();
	ButtonPos.x += 100;
	ButtonPos.y += 35;

	OKButton->SetPivot(0.5f, 0.5f);
	OKButton->SetPos(ButtonPos);
	OKButton->SetSize(40.f, 20.f);
	OKButton->SetTexture(EButtonState::Normal, "OKButton",
		TEXT("UI/OkNormal.png"));
	OKButton->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));

	OKButton->SetTexture(EButtonState::Hovered, "OKButton",
		TEXT("UI/OkNormal.png"));
	OKButton->SetTint(EButtonState::Hovered, FVector4(0.8f, 0.8f, 0.8f, 1.f));

	OKButton->SetTexture(EButtonState::Click, "OKButton",
		TEXT("UI/OkNormal.png"));
	OKButton->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

	OKButton->SetTexture(EButtonState::Disable, "OKButton",
		TEXT("UI/OkNormal.png"));


	OKButton->SetEventCallback<CNpcWidget>(
		EButtonEventState::Click, this, &CNpcWidget::OkClick);

	OKButton->SetSound(EButtonEventState::Click,
		"Click", "Click.mp3");


	std::shared_ptr<CButton> CancleButton =
		CreateWidget<CButton>("CancleButton", 1).lock();

	ButtonPos = Bottom->GetPos();
	ButtonPos.x += 166;
	ButtonPos.y += 35;

	CancleButton->SetPivot(0.5f, 0.5f);
	CancleButton->SetPos(ButtonPos);
	CancleButton->SetSize(40.f, 20.f);
	CancleButton->SetTexture(EButtonState::Normal, "CancleButton",
		TEXT("UI/CancleNormal.png"));
	CancleButton->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));

	CancleButton->SetTexture(EButtonState::Hovered, "CancleButton",
		TEXT("UI/CancleNormal.png"));
	CancleButton->SetTint(EButtonState::Hovered, FVector4(0.8f, 0.8f, 0.8f, 1.f));

	CancleButton->SetTexture(EButtonState::Click, "CancleButton",
		TEXT("UI/CancleNormal.png"));
	CancleButton->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

	CancleButton->SetTexture(EButtonState::Disable, "CancleButton",
		TEXT("UI/CancleNormal.png"));

	CancleButton->SetEventCallback<CNpcWidget>(
		EButtonEventState::Click, this, &CNpcWidget::CancleClick);
	CancleButton->SetSound(EButtonEventState::Click,
		"Click", "Click.mp3");


	return true;
}

void CNpcWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CNpcWidget::Render()
{
	CWidgetContainer::Render();
}

void CNpcWidget::OkClick()
{
	auto World = mWorld.lock();
	if (World)
	{
		auto Player = World->FindObject<CPlayer>("Player").lock();
		if (Player)
		{
			Player->AddDragonMilk();
		}
	}
	Destroy();
}

void CNpcWidget::CancleClick()
{
	CEngine::GetInst()->Destroy();
}

