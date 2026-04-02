#pragma once

#include "UI/WidgetContainer.h"

class CFadeOutWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;

public:
    CFadeOutWidget();
    virtual ~CFadeOutWidget();

private:
    std::shared_ptr<class CImage> mBack;

    bool mStart = false;
    float mAlpha = 0.f;
    bool mFinish = false;
    float mFadeSpeed = 2.f;

public:
    void SetFadeOutSpeed(float Speed)
    {
        mFadeSpeed = Speed;
    }
    void Start();
    bool Finish() const ; 
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

};

