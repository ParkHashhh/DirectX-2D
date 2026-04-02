#include "BossGround.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "ArmSkill.h"
CBossGround::CBossGround()
{
	SetClassType<CBossGround>();
}

CBossGround::CBossGround(const CBossGround& ref) :
	CGameObject(ref)
{
}

CBossGround::CBossGround(CBossGround&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CBossGround::~CBossGround()
{
}

bool CBossGround::Init()
{
	CGameObject::Init();

	mBody = CreateComponent<CColliderBox2D>("GroundBody");
	auto	Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(80.f, 70.f);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("BossGround");
		Body->SetCollisionBeginFunction<CBossGround>(this, &CBossGround::OnHit);
	}


	return true;
}



void CBossGround::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}


void CBossGround::OnHit(const FVector3& HitPoint, CCollider* Dest)
{

}


