#pragma once
#include "UI/WidgetContainer.h"
#include "../ClientInfo.h"
class CStatement :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CStatement();

public:
    virtual ~CStatement();

private:
    std::weak_ptr<class CGameObject> mPlayer;

    std::vector<std::weak_ptr<class CStatementSlot>> mSlotArray;
    std::shared_ptr<class CButton> mTabButtons[EItemType::End];
    EItemType::Type mCurrentTab;


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

    void SetPlayer(std::weak_ptr<class CGameObject> Player)
    {
        mPlayer = Player;
    }
    std::weak_ptr<class CGameObject> GetPlayer()
    {
        return mPlayer;
    }



};

