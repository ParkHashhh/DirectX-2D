#pragma once
#include "UI/WidgetContainer.h"
#include "../ClientInfo.h"
class CInventory :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CInventory();

public:
    virtual ~CInventory();

private:
    std::weak_ptr<class CGameObject> mPlayer;

    std::vector<std::weak_ptr<class CInventorySlot>> mSlotArray;
    std::shared_ptr<class CButton> mTabButtons[EItemType::End];
    EItemType::Type mCurrentTab;


public:
    std::weak_ptr<class CWorld> GetWorld()
    {
        return mWorld;
    }
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

    EItemType::Type GetCurrentTab()
    {
        return mCurrentTab;
    }

    void AddItem(std::shared_ptr<class CItem> Item);
    int GetEmptyIndex(EItemType::Type Type);
    void SetPlayer(std::weak_ptr<class CGameObject> Player)
    {
        mPlayer = Player;
    }

    std::weak_ptr<class CGameObject> GetPlayer()
    {
        return mPlayer;
    }


private:
    void ChangeOnTab(EItemType::Type Type);
    void ChangeOffTab(EItemType::Type Type);


    void EquipButtonClick();
    void ConsumeButtonClick();
    void InstallButtonClick();
    void EtcButtonClick();
    void CashButtonClick();

    void SetItemSlot();

    void ChangeTab(EItemType::Type Type);

public:
    void SwapItem(EItemType::Type Type, int SrcIndex,int DestIndex);
    void EraseItem(EItemType::Type Type, int Index);
    void RefreshInventory();

};

