#pragma once

#include "UI/WidgetContainer.h"

class CPlayerStateWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CPlayerStateWidget();

public:
    virtual ~CPlayerStateWidget();

private:
    std::weak_ptr<class CTextBlock> mNameText;
    std::weak_ptr<class CTextBlock> mHPText;
    std::weak_ptr<class CTextBlock> mMPText;
    std::weak_ptr<class CProgressBar> mHPBar;
    std::weak_ptr<class CProgressBar> mMPBar;
    std::weak_ptr<class CImage> mBackImage;


public:
    void SetPlayerName(const TCHAR* Name);
    void SetPlayerHP(float HP, float HPMax);
    void SetPlayerMP(float MP, float MPMax);
    void MultipleWidth(float Size);
    void MultipleHeight(float Size);
    void CreateStateWidget();
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    
};

