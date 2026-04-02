#pragma once
#include "UI/WidgetContainer.h"
#include "../ClientInfo.h"
class CEquipment :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CEquipment();

public:
    virtual ~CEquipment();

private:
    std::weak_ptr<class CGameObject> mPlayer;

    std::vector<std::weak_ptr<class CEquipmentSlot>> mSlotArray;

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
    void SetZakumEquip();
    void SetHornTailEquip();

private:
    void SetItemSlot();

};

