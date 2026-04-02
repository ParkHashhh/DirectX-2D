#include "SlotIcon.h"
#include "UI/Button.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "Inventory.h"
#include "Slot.h"
#include "UI/MouseWidget.h"
#include "Render/RenderManager.h"


CSlotIcon::CSlotIcon()
{
}

CSlotIcon::CSlotIcon(const CSlotIcon& ref) :
	CWidgetContainer(ref)
{
	mImage = FindWidget<CImage>("Back");
}

CSlotIcon::~CSlotIcon()
{
}

void CSlotIcon::SetImageEnable(bool Enable)
{
	mImage.lock()->SetEnable(Enable);

}

void CSlotIcon::SetSizeImage(float X, float Y)
{
	mImage.lock()->SetSize(X, Y);
}

void CSlotIcon::SetIconImage(const std::string& Name)
{
	auto	Image = mImage.lock();

	Image->SetTexture(Name);
}

void CSlotIcon::SetIconImage(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	auto	Image = mImage.lock();

	Image->SetTexture(Name, FileName, PathName);
}

bool CSlotIcon::Init()
{
	CWidgetContainer::Init();

	mImage = CreateWidget<CImage>("Back");

	std::shared_ptr<CImage> Back = mImage.lock();

	Back->SetSize(44.f, 44.f);

	return true;
}

void CSlotIcon::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CSlotIcon::Render()
{
	CWidgetContainer::Render();
}


void CSlotIcon::MouseHovered()
{
	CWidgetContainer::MouseHovered();
	CRenderManager::GetInst()->SetMouseState(EMouseState::Grab);

}

void CSlotIcon::MouseUnHovered()
{
	CWidgetContainer::MouseUnHovered();
	CRenderManager::GetInst()->SetMouseState(EMouseState::Normal);
}

bool CSlotIcon::MouseDragStart(const FVector2& MousePos,
	std::shared_ptr<CWidget>& DragOperator)
{
	CWidgetContainer::MouseDragStart(MousePos, DragOperator);

	DragOperator = mSelf.lock();

	return true;
}

bool CSlotIcon::MouseDrag(const FVector2& MousePos,
	const FVector2& MouseMove)
{
	CWidgetContainer::MouseDrag(MousePos, MouseMove);

	//AddPos(MouseMove);

	return true;
}

bool CSlotIcon::MouseDragEnd(const FVector2& MousePos)
{
	CWidgetContainer::MouseDragEnd(MousePos);

	return true;
}

CSlotIcon* CSlotIcon::Clone()	const
{
	return new CSlotIcon(*this);
}

bool CSlotIcon::OnDrop(std::weak_ptr<CWidget>& Result)
{
	auto Parent = GetSlot().lock();
	if (Parent)
	{
		return Parent->OnDrop(Result);
	}
	return false;
}
bool CSlotIcon::OutDrop()
{
	auto Parent = GetSlot().lock();
	if (Parent)
	{
		return Parent->OutDrop();
	}
	return false;
}
