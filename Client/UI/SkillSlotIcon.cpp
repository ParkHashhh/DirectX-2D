#include "SkillSlotIcon.h"
#include "SlotIcon.h"
#include "UI/Button.h"
#include "UI/TextBlock.h"
#include "UI/Image.h"
#include "UI/ProgressBar.h"
#include "Inventory.h"
#include "Slot.h"
#include "UI/MouseWidget.h"
#include "Render/RenderManager.h"


CSkillSlotIcon::CSkillSlotIcon()
{
}

CSkillSlotIcon::CSkillSlotIcon(const CSkillSlotIcon& ref) :
	CSlotIcon(ref)
{
}

CSkillSlotIcon::~CSkillSlotIcon()
{
}
bool CSkillSlotIcon::Init()
{
	CSlotIcon::Init();



	return true;
}


void CSkillSlotIcon::SetIconImage(const std::string& Name)
{
	auto	Image = mImage.lock();

	Image->SetTexture(Name);
}

void CSkillSlotIcon::SetIconImage(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	auto	Image = mImage.lock();

	Image->SetTexture(Name, FileName, PathName);
}


void CSkillSlotIcon::Update(float DeltaTime)
{
	CSlotIcon::Update(DeltaTime);
}

void CSkillSlotIcon::Render()
{
	CSlotIcon::Render();
}

void CSkillSlotIcon::MouseHovered()
{
}

void CSkillSlotIcon::MouseUnHovered()
{
}

bool CSkillSlotIcon::MouseDragStart(const FVector2& MousePos,
	std::shared_ptr<CWidget>& DragOperator)
{
	return true;
}

bool CSkillSlotIcon::MouseDrag(const FVector2& MousePos,
	const FVector2& MouseMove)
{
	return true;
}

bool CSkillSlotIcon::MouseDragEnd(const FVector2& MousePos)
{

	return true;
}


CSkillSlotIcon* CSkillSlotIcon::Clone()	const
{
	return new CSkillSlotIcon(*this);
}

bool CSkillSlotIcon::OnDrop(std::weak_ptr<CWidget>& Result)
{

	return false;
}

