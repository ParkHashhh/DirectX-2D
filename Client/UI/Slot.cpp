#include "Slot.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "SlotIcon.h"

CSlot::CSlot()
{
}

CSlot::CSlot(const CSlot& ref)	:
	CWidgetContainer(ref)
{

}

CSlot::~CSlot()
{
}

void CSlot::EnableIcon(bool Enable)
{
	mIcon->SetEnable(Enable);
}

void CSlot::SetIconImage(const std::string& Name)
{
	mIcon->SetIconImage(Name);
}

void CSlot::SetIconImage(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	mIcon->SetIconImage(Name, FileName, PathName);
}


bool CSlot::Init()
{
	CWidgetContainer::Init();
	return true;
}

void CSlot::Update(float DeltaTime)
{
	CWidgetContainer::Update(DeltaTime);
}

void CSlot::Render()
{
	CWidgetContainer::Render();
}



CSlot* CSlot::Clone()	const
{
	return new CSlot(*this);
}

bool CSlot::OnDrop(std::weak_ptr<CWidget>& Result)
{
	return true;
}