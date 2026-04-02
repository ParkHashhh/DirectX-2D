#include "SkillSlot.h"
#include "SkillSlotIcon.h"
#include "UI/Image.h"
#include "Slot.h"
#include "../Player/Player.h"

CSkillSlot::CSkillSlot()
{
}

CSkillSlot::CSkillSlot(const CSkillSlot& ref)
{
}

CSkillSlot::~CSkillSlot()
{
}

void CSkillSlot::EnableIcon(bool Enable)
{
	mEnable = Enable;
}

bool CSkillSlot::Init()
{
	CSlot::Init();
	return true;
}



void CSkillSlot::SetPrgressSlot()
{
	mProgress = CreateWidget<CProgressBar>("SkillIcon", 1).lock();
	auto Self = std::dynamic_pointer_cast<CSkillSlot>(mSelf.lock());
	mProgress->SetSize(25.f, 25.f);
}

void CSkillSlot::SetProgressPercent(float Percent)
{
	mProgress->SetPercent(Percent);
}

void CSkillSlot::SetProgressImage(EProgressBarImageType::Type State,const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	mProgress->SetTexture(State, Name,FileName);
}

void CSkillSlot::SetProgressTint(EProgressBarImageType::Type State, FVector4 Tint)
{
	mProgress->SetTint(State, Tint);

}
void CSkillSlot::SetSize(float X, float Y)
{
	mProgress->SetSize(X, Y);
}
void CSkillSlot::SetProgressDir(EProgressBarDir Dir)
{
	mProgress->SetBarDir(Dir);
}

void CSkillSlot::Update(float DeltaTime)
{
	CSlot::Update(DeltaTime);

	if (mMaxTime > 0.0f)
	{
		mCurrentTime -= DeltaTime;

		float Percent = 1 - (mCurrentTime / mMaxTime);
		SetProgressPercent(Percent);
		if (mCurrentTime <= 0.0f)
		{
			// 여기서 Player한테 알려야함
			mCurrentTime = 0.0f;
			mPlayer.lock()->EndBuff(mSlotName);
		}
	}
}

void CSkillSlot::Render()
{
	CSlot::Render();
}

CSkillSlot* CSkillSlot::Clone()	const
{
	return new CSkillSlot(*this);
}
