#pragma once
#include "SlotIcon.h"
class CSkillSlotIcon :

    public CSlotIcon
{
    friend class CWorldUIManager;
    friend class CInventory;
    friend class CWidgetContainer;

protected:
    CSkillSlotIcon();
    CSkillSlotIcon(const CSkillSlotIcon& ref);

public:
    virtual ~CSkillSlotIcon();

private:
    std::weak_ptr<class CImage> mImage;
    std::weak_ptr<class CSlot> mSlot;
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
    virtual void SetIconImage(const std::string& Name);
    virtual void SetIconImage(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = "Texture");

    virtual void MouseHovered();
    virtual void MouseUnHovered();
    virtual bool MouseDragStart(const FVector2& MousePos,
        std::shared_ptr<CWidget>& DragOperator);
    virtual bool MouseDrag(const FVector2& MousePos,
        const FVector2& MouseMove);
    virtual bool MouseDragEnd(const FVector2& MousePos);
    virtual bool OnDrop(std::weak_ptr<CWidget>& Result);
    virtual CSkillSlotIcon* Clone()	const;
};

