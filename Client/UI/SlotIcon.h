#pragma once

#include "UI/WidgetContainer.h"

class CSlotIcon :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CInventory;
    friend class CWidgetContainer;

protected:
    CSlotIcon();
    CSlotIcon(const CSlotIcon& ref);

public:
    virtual ~CSlotIcon();

protected:
    std::weak_ptr<class CImage> mImage;
    std::weak_ptr<class CWidget> mSlot;

public:
    void SetImageEnable(bool Enable);
    void SetSizeImage(float X, float Y);
    void SetSlot(std::weak_ptr<class CWidget> Slot)
    {
        mSlot = Slot;
    }
    virtual void SetIconImage(const std::string& Name);
    virtual void SetIconImage(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = "Texture");
    void SetTexture(const std::weak_ptr<class CTexture>& Texture);
    std::weak_ptr<class CImage> GetImage()
    {
        return mImage;
    }
    
    std::weak_ptr<class CWidget> GetSlot()
    {
        return mSlot;
    }
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    virtual void MouseHovered();
    virtual void MouseUnHovered();
    virtual bool MouseDragStart(const FVector2& MousePos,
        std::shared_ptr<CWidget>& DragOperator);
    virtual bool MouseDrag(const FVector2& MousePos,
        const FVector2& MouseMove);
    virtual bool MouseDragEnd(const FVector2& MousePos);
    virtual bool OnDrop(std::weak_ptr<CWidget>& Result);
    virtual bool OutDrop();
    virtual CSlotIcon* Clone()	const;

};

