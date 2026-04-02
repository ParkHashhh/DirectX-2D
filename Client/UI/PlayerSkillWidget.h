#pragma once
#include "UI/WidgetContainer.h"
class CPlayerSkillWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CPlayerSkillWidget();

public:
    virtual ~CPlayerSkillWidget();

private:
    std::vector<std::weak_ptr<class CSkillSlot>> mSlotArray;
private:
    void CreateSkillSlot();


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    void SetAvengerSkillCoolTime(float Time, float SkillCoolTime);
    void SetWillSkillCoolTime(float Time, float SkillCoolTime);

};

