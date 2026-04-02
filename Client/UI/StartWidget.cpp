#include "StartWidget.h"
#include "UI/Button.h"
#include "UI/Image.h"
#include "Device.h"
#include "Engine.h"
#include "World/WorldManager.h"
#include "../World/MainWorld.h"
#include "../World/LoadingWorld.h"
#include "FadeOutWidget.h"


CStartWidget::CStartWidget()
{
}

CStartWidget::~CStartWidget()
{
}

bool CStartWidget::Init()
{
	CWidgetContainer::Init();

	mFadeOut = CreateWidget<CFadeOutWidget>("FadeOutWidget",999);

	std::shared_ptr<CImage> Back = CreateWidget<CImage>("Back").lock();
	 
	FResolution	RS = CDevice::GetInst()->GetResolution();

	Back->SetSize((float)RS.Width, (float)RS.Height);
	Back->SetTexture("StartBack", TEXT("UI/StartBackground.png"));

	std::shared_ptr<CButton> StartButton =
		CreateWidget<CButton>("StartButton", 1).lock();

	FVector3	ButtonPos;
	ButtonPos.x = RS.Width / 2.f + 380;
	ButtonPos.y = RS.Height / 2.f - 70.f;

	StartButton->SetPivot(0.5f, 0.5f);
	StartButton->SetPos(ButtonPos);
	StartButton->SetSize(140.f, 70.f);
	StartButton->SetTexture(EButtonState::Normal, "StartButton",
		TEXT("UI/StartButton.png"));
	StartButton->SetTint(EButtonState::Normal, FVector4(1.f, 1.f, 1.f, 1.f));

	StartButton->SetTexture(EButtonState::Hovered, "StartButton",
		TEXT("UI/StartButton.png"));
	StartButton->SetTint(EButtonState::Hovered, FVector4(0.8f, 0.8f, 0.8f, 1.f));

	StartButton->SetTexture(EButtonState::Click, "StartButton",
		TEXT("UI/StartButton.png"));
	StartButton->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

	StartButton->SetTexture(EButtonState::Disable, "StartButton",
		TEXT("UI/StartButton.png"));

	StartButton->SetSound(EButtonEventState::Hovered,
		"ButtonHovered", "MouseHoverd.mp3");
	StartButton->SetSound(EButtonEventState::Click,
		"ButtonClick", "LoginClick.mp3");

	StartButton->SetEventCallback<CStartWidget>(
		EButtonEventState::Click, this, &CStartWidget::StartClick);


	std::shared_ptr<CButton> ExitButton =
		CreateWidget<CButton>("ExitButton", 1).lock();

	ButtonPos.x = RS.Width / 2.f+ 370;
	ButtonPos.y = RS.Height / 2.f  + 110.f;

	ExitButton->SetPivot(0.5f, 0.5f);
	ExitButton->SetPos(ButtonPos);
	ExitButton->SetSize(140.f, 70.f);
	ExitButton->SetTexture(EButtonState::Normal, "ExitButton",
		TEXT("UI/EndButton.png"));
	ExitButton->SetTint(EButtonState::Normal, FVector4(1.f, 1.0f, 1.0f, 1.f));

	ExitButton->SetTexture(EButtonState::Hovered, "ExitButton",
		TEXT("UI/EndButton.png"));
	ExitButton->SetTint(EButtonState::Hovered, FVector4(0.8f, 0.8f, 0.8f, 1.f));

	ExitButton->SetTexture(EButtonState::Click, "ExitButton",
		TEXT("UI/EndButton.png"));
	ExitButton->SetTint(EButtonState::Click, FVector4(0.6f, 0.6f, 0.6f, 1.f));

	ExitButton->SetTexture(EButtonState::Disable, "ExitButton",
		TEXT("UI/EndButton.png"));

	ExitButton->SetSound(EButtonEventState::Hovered,
		"ButtonHovered", "MouseHoverd.mp3");
	ExitButton->SetSound(EButtonEventState::Click,
		"ButtonClick", "LoginClick.mp3");

	ExitButton->SetEventCallback<CStartWidget>(
		EButtonEventState::Click, this, &CStartWidget::ExitClick);



	return true;
}

void CStartWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
	auto FadeOut_ = mFadeOut.lock();

	if (FadeOut_)
	{
		if (FadeOut_->Finish())
		{
			auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock();
			World->Load(EWorldType::Main);
			FadeOut_->Destroy();
		}
	}
}

void CStartWidget::Render()
{
	CWidgetContainer::Render();
}

void CStartWidget::StartClick()
{
	
	auto FadeOut_ = mFadeOut.lock();
	FadeOut_->Start();
	
}

void CStartWidget::ExitClick()
{
	CEngine::GetInst()->Destroy();
}

