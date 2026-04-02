#include "FadeOutWidget.h"
#include "UI/Image.h"
#include "Device.h"
#include "Engine.h"
#include "World/WorldManager.h"

CFadeOutWidget::CFadeOutWidget()
{
}

CFadeOutWidget::~CFadeOutWidget()
{
}

void CFadeOutWidget::Start()
{
	mStart = true;
	mBack->SetEnable(true);
}

bool CFadeOutWidget::Finish() const
{
	return mFinish;
}



bool CFadeOutWidget::Init()
{
	CWidgetContainer::Init();

	FResolution	RS = CDevice::GetInst()->GetResolution();

	auto Back = CreateWidget<CImage>("FadeOut",9999);

	mBack = Back.lock();
	
	if (mBack)
	{
		mBack->SetPos(0, 0);
		mBack->SetSize(float(RS.Width),float(RS.Height));
		mBack->SetOpacity(0.f);
		mBack->SetTexture("FadeOut", TEXT("Background/FadeOut.png"));
		mBack->SetEnable(false);
	}
	

	return true;
}

void CFadeOutWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
	if (mStart)
	{
		
		mAlpha += mFadeSpeed * DeltaTime;

		if (mAlpha > 1.f)
		{
			mAlpha = 1.f;
			mStart = false;
			mFinish = true;
		}
		mBack->SetOpacity(mAlpha);
	}
}

void CFadeOutWidget::Render()
{
	CWidgetContainer::Render();
}

