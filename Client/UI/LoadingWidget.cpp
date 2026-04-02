#include "LoadingWidget.h"
#include "UI/Button.h"
#include "UI/Image.h"
#include "UI/TextBlock.h"
#include "Device.h"
#include "Engine.h"
#include "World/WorldManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/TextureManager.h"
#include "../World/MainWorld.h"

CLoadingWidget::CLoadingWidget()
{
}

CLoadingWidget::~CLoadingWidget()
{
}

bool CLoadingWidget::Init()
{
	CWidgetContainer::Init();

	std::shared_ptr<CImage> Back = CreateWidget<CImage>("Back").lock();

	FResolution	RS = CDevice::GetInst()->GetResolution();

	Back->SetSize((float)RS.Width, (float)RS.Height);
	Back->SetTexture("LoadingBack", TEXT("Loading/LoadingBackGround.png"));

	std::shared_ptr<CTextBlock> Text =
		CreateWidget<CTextBlock>("Text", 1).lock();

	Text->SetText(TEXT("LOADING..."));
	Text->SetTextColor(255, 255, 255, 255);
	Text->SetPos(900.f, 550.f);
	Text->SetSize(500.f, 150.f);
	Text->SetFontSize(50.f);
	Text->EnableShadow(true);
	Text->SetShadowOffset(3.f, 3.f);
	Text->SetShadowTextColor(128, 128, 128, 255);

	auto AnimWidget = CreateWidget<CImage>("Animation").lock();

	std::vector<const TCHAR*>	TextureFileName;
	for (int i = 1; i <= 3; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName,
			TEXT("Loading/Loading_Animation_%03d.png"), i);
		TextureFileName.push_back(FileName);
	}

	AnimWidget->SetSize(150.f, 100.f);
	AnimWidget->SetPos(580, 320);
	AnimWidget->SetTexture("LoadingAnimation", TextureFileName);
	AnimWidget->AddBrushFrame(0.f, 0.f, 320.f, 240.f,3);
	AnimWidget->SetBrushAnimation(true);


	for (int i = 0; i <= 2; ++i)
	{
		delete[] TextureFileName[i];
	}
	TextureFileName.clear();


	return true;
}

void CLoadingWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CLoadingWidget::Render()
{
	CWidgetContainer::Render();
}

