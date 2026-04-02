#include "DamageObject.h"
#include "Component/WidgetComponent.h"
#include "../UI/DamageUI.h"

CDamageObject::CDamageObject()
{
	SetClassType<CDamageObject>();
}

CDamageObject::CDamageObject(const CDamageObject& ref) :
	CGameObject(ref)
{
}

CDamageObject::CDamageObject(CDamageObject&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CDamageObject::~CDamageObject()
{
}

void CDamageObject::SetHitWorldPos(float x, float y)
{
	mDamageUI->SetHitPos(x, y);
}

bool CDamageObject::Init()
{
	CGameObject::Init();
	mWidgetComponent = CreateComponent<CWidgetComponent>("Damage" + std::to_string(rand()));
	auto UIMng = mWorld.lock()->GetUIManager().lock();
	auto Widget= mWidgetComponent.lock();
	std::string UIName = "DamageUI" + std::to_string(rand());
	mDamageUI = Widget->SetWidget<CDamageUI>(UIName).lock();


	
	return true;
}

void CDamageObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CDamageObject::SetDamage(int Damage)
{
	if (mDamageUI)
	{
		mDamageUI->SetNumber(Damage);
	}
}


CDamageObject* CDamageObject::Clone()
{
	return new CDamageObject(*this);
}
