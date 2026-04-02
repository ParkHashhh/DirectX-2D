#include "PlayerSkillStateWidget.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "SkillSlotIcon.h"
#include "SkillSlot.h"
#include "PlayerSkillStateWidget.h"
CPlayerSkillStateWidget::CPlayerSkillStateWidget()
{
}

CPlayerSkillStateWidget::~CPlayerSkillStateWidget()
{
}


void CPlayerSkillStateWidget::CreateSkillSlot(const std::string& Name, const TCHAR* FileName, float MaxTime)
{
	auto iter = mBuffList.begin();
	auto iterEnd = mBuffList.end();
	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetSlotName() == Name)
		{
			(*iter)->SetBuffTime(MaxTime);
			return;
		}
	}
	std::shared_ptr<CSkillSlot> Slot = CreateWidget<CSkillSlot>(Name).lock();

    if (Slot)
    {
        Slot->SetSlotName(Name);
        Slot->SetPrgressSlot();
        Slot->SetSize(30.f, 30.f);
        Slot->SetPlayer(mPlayer);
        Slot->SetProgressImage(EProgressBarImageType::Back, Name, FileName);
        Slot->SetProgressTint(EProgressBarImageType::Back, FVector4(1.f, 1.f, 1.f, 1.f));

        Slot->SetProgressImage(EProgressBarImageType::Fill, "Back", TEXT("Slot/Back.png"));
        Slot->SetProgressTint(EProgressBarImageType::Fill, FVector4(0.2f, 0.2f, 0.2f, 0.5f));
        Slot->SetProgressDir(EProgressBarDir::TopToBottom);
        Slot->SetBuffTime(MaxTime);
        Slot->EnableIcon(true);

        mBuffList.push_back(Slot);
        ResetBuffPositions();
    }
}

bool CPlayerSkillStateWidget::Init()
{
	CWidgetContainer::Init();
	return true;
}

void CPlayerSkillStateWidget::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);

    bool bNeedsReorder = false;
    auto iter = mBuffList.begin();

    while (iter != mBuffList.end())
    {
        if ((*iter)->GetCurrent() <= 0.0f)
        {
            (*iter)->SetEnable(false);
            iter = mBuffList.erase(iter); 
            bNeedsReorder = true;
        }
        else
        {
            ++iter;
        }
    }

    if (bNeedsReorder)
    {
        ResetBuffPositions();
    }
}

void CPlayerSkillStateWidget::Render()
{
	CWidgetContainer::Render();
}

void CPlayerSkillStateWidget::ResetBuffPositions()
{
    float StartX = 1240.0f;
    float StartY = 10.0f;  
    float OffsetX = -35.0f;

    for (int i = 0; i < (int)mBuffList.size(); ++i)
    {
        mBuffList[i]->SetPos(StartX + (i * OffsetX), StartY);
    }
}