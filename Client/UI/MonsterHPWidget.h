#pragma once

#include "UI/WidgetContainer.h"

class CMonsterHPWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CMonsterHPWidget();

public:
    virtual ~CMonsterHPWidget();

private:
    std::weak_ptr<class CProgressBar> mHPBar;
    std::weak_ptr<class CImage> mBackImage;
    std::weak_ptr<class CImage> mIcon;


public:
    void SetMonsterHP(float HP, float HPMax);
    void SetIcon(const std::string& Name, const TCHAR* FileName);
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

};

