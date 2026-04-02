#pragma once

#include "Slot.h"
#include "UI/ProgressBar.h"
class CSkillSlot :
    public CSlot
{
    friend class CWorldUIManager;
    friend class CInventory;
    friend class CWidgetContainer;

protected:
    CSkillSlot();
    CSkillSlot(const CSkillSlot& ref);

public:
    virtual ~CSkillSlot();

private:
    std::shared_ptr<class CProgressBar> mProgress;
    std::weak_ptr<class CPlayer> mPlayer;
    float mCurrentTime; 
    float mMaxTime;     
    bool  mIsBuffSlot;  
    std::string mSlotName;

public:
    void SetPlayer(std::weak_ptr<class CPlayer> Player)
    {
        mPlayer = Player;
    }
    void SetSlotName(const std::string& Name)
    {
        mSlotName = Name;
    }
    std::string GetSlotName()
    {
        return mSlotName;
    }
    void SetBuffTime(float MaxTime)
    {
        mMaxTime = MaxTime;
        mCurrentTime = MaxTime;
    }
    float GetCurrent() const 
    {
        return mCurrentTime; 
    }
    void SetSize(float x, float y);
    
    void EnableIcon(bool Enable);
    void SetIconImage(const std::string& Name);
    void SetProgressImage(EProgressBarImageType::Type State, const std::string& Name,
        const TCHAR* FileName,
        const std::string& PathName = "Texture");
    void SetProgressTint(EProgressBarImageType::Type State, FVector4 Tint);
    void SetProgressDir(EProgressBarDir Dir);
    void SetPrgressSlot();
    void SetProgressPercent(float Percent);

public:

    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    virtual CSkillSlot* Clone() const;
};

