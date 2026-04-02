#pragma once
#include "Slot.h"
#include "../ClientInfo.h"

class CInventorySlot :
    public CSlot
{
    friend class CWorldUIManager;
    friend class CInventory;
    friend class CWidgetContainer;

protected:
    CInventorySlot();
    CInventorySlot(const CInventorySlot& ref);

public:
    virtual ~CInventorySlot();

private:
    int mSlotIndex = -1;
    std::shared_ptr<class CItem> mItem;
    std::weak_ptr<class CWidget> mOwner;


public:
    void EnableIcon(bool Enable);
    void SetInventory(std::weak_ptr<class CWidget> Inventory)
    {
        mOwner = Inventory;
    }

    EItemType::Type GetCurrentTab();

public:
    void SetSlotIndex(int Index)
    {
        mSlotIndex = Index;
    }
    int GetSlotIndex() const
    { 
        return mSlotIndex;
    }
    void SetItem(std::shared_ptr<class CItem> Item);
    std::shared_ptr<class CItem> GetItem()
    {
        return mItem;
    }


    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    virtual CInventorySlot* Clone()	const;
    virtual bool OnDrop(std::weak_ptr<CWidget>& Result);
    virtual bool OutDrop();


};

