#pragma once
#include "Slot.h"
#include "../ClientInfo.h"
class CEquipmentSlot :
    public CSlot
{
    friend class CWorldUIManager;
    friend class CInventory;
    friend class CWidgetContainer;

protected:
    CEquipmentSlot();
    CEquipmentSlot(const CEquipmentSlot& ref);

public:
    virtual ~CEquipmentSlot();

private:
    std::shared_ptr<class CItem> mItem;
    std::weak_ptr<class CWidget> mOwner;
    EItemType::Type mType;

public:
    void EnableIcon(bool Enable);
    void SetInventory(std::weak_ptr<class CWidget> Inventory)
    {
        mOwner = Inventory;
    }

public:
    void SetItem(std::shared_ptr<class CItem> Item);
    std::shared_ptr<class CItem> GetItem()
    {
        return mItem;
    }


    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    virtual CEquipmentSlot* Clone()	const;
    virtual bool OnDrop(std::weak_ptr<CWidget>& Result);
    virtual bool OutDrop();


};

