#include "Statement.h"
#include "UI/Button.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "UI/TitleBar.h"
#include "Slot.h"
#include "InventorySlot.h"
#include "../Item/Item.h"
#include "InventoryManager.h"
#include "../Player/Player.h"
CStatement::CStatement()
{
}

CStatement::~CStatement()
{
}

bool CStatement::Init()
{	
	CWidgetContainer::Init();

	std::shared_ptr<CTitleBar> TitleBar =
		CreateWidget<CTitleBar>("TitleBar").lock();

	TitleBar->SetSize(160, 17);
	TitleBar->SetUpdateWidget(mSelf);
	TitleBar->SetOpacity(0.f);

	std::shared_ptr<CImage> Back =
		CreateWidget<CImage>("Back").lock();
	Back->SetPos(0.f, 0.f);
	Back->SetSize(212, 373);
	Back->SetTexture("StatBackground", TEXT("Slot/StatBackground.png"));


	std::shared_ptr<CImage> Back2 =
		CreateWidget<CImage>("Back2").lock();
	Back2->SetPos(6.f, 20.f);
	Back2->SetSize(200, 344);
	Back2->SetTexture("StatBackground2", TEXT("Slot/StatBackground2.png"));

	std::shared_ptr<CTextBlock> Text =
		CreateWidget<CTextBlock>("Name",2).lock();
	Text->SetPos(72, 27.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("zl존주석123"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Job", 2).lock();
	Text->SetPos(72, 46.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("표도"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");


	Text = CreateWidget<CTextBlock>("Level", 2).lock();
	Text->SetPos(72, 64.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("52"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Exp", 2).lock();
	Text->SetPos(72, 83.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("754328 / 967845"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("HonorLevel", 2).lock();
	Text->SetPos(72, 100.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("1"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Reputation", 2).lock();
	Text->SetPos(72, 118.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("0"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Guild", 2).lock();
	Text->SetPos(72, 140.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT(""));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("HP", 2).lock();
	Text->SetPos(72, 154.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	/*
	auto Player = std::dynamic_pointer_cast<CPlayer>(mPlayer.lock());
	int HPMax = Player->GetHPMax();
	TCHAR	HPBuffer[64] = {};
	swprintf_s(HPBuffer, 64, TEXT("%d / %d"), HPMax,HPMax);
	*/
	Text->SetText(TEXT("4179 / 4179"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("MP", 2).lock();
	Text->SetPos(72, 172.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("625 / 625"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");


	Text = CreateWidget<CTextBlock>("Ingi", 2).lock();
	Text->SetPos(72, 190.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("-15"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Str", 2).lock();
	Text->SetPos(72, 259.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("4"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Dex", 2).lock();
	Text->SetPos(72, 277.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("4"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Int", 2).lock();
	Text->SetPos(72, 295.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("4"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	Text = CreateWidget<CTextBlock>("Luck", 2).lock();
	Text->SetPos(72, 313.f);
	Text->SetSize(100.f, 15.f);
	Text->SetFontSize(12.f);
	Text->SetText(TEXT("157"));
	Text->SetTextColor(FVector4(0, 0, 0, 1.f));
	Text->SetAlignH(ETextAlignH::Left);
	Text->SetFont("MapleBold");

	return true;
}

void CStatement::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CStatement::Render()
{
	CWidgetContainer::Render();
}