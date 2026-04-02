#pragma once

#include "UI/WidgetContainer.h"

class CEndingWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;

protected:
    CEndingWidget();

public:
    virtual ~CEndingWidget();

private:
    std::weak_ptr<class CGameObject> mPlayer;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    void SetPlayer(std::weak_ptr<class CGameObject> Player)
    {
        mPlayer = Player;
    }

private:
    void OkClick();
    void CancleClick();
};

