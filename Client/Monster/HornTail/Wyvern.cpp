#include "Wyvern.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "UI/NumberWidget.h"
#include "../../Player/Player.h"
#include "../DamageObject.h"


CWyvern::CWyvern()
{

	SetClassType<CWyvern>();
}

CWyvern::CWyvern(const CWyvern& ref)
	: CMonster(ref)
{
}


CWyvern::CWyvern(CWyvern&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CWyvern::~CWyvern()
{

}

bool CWyvern::Init()
{
	if (!CMonster::Init())
		return false;
	int Random = rand();
	mIndex = (ColorIndex::Index)(Random % 3);

	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("WyvernAnimation2D" + std::to_string(Random));
	mMeshComponent = CreateComponent<CMeshComponent>("WyvernMesh" + std::to_string(Random));
	mBody = CreateComponent<CColliderBox2D>("WyvernBody" + std::to_string(Random));
	mStateComponent = CreateComponent<CStateComponent>("WyvernState" + std::to_string(Random));

	switch (mIndex)
	{
	case ColorIndex::Black:
		mColor = "Black";
		break;
	case ColorIndex::Red:
		mColor = "Red";
		break;
	case ColorIndex::Blue:
		mColor = "Blue";
		break;
	}


	auto State = mStateComponent.lock();
	auto Mesh = mMeshComponent.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Body = mBody.lock();
	if (State)
	{
		State->SetHPMax(30000.f);
	}
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetWorldScale(188, 171);
		Mesh->SetRenderLayer(0);
	}
	if (Body)
	{
		Body->SetCollisionProfile("Monster");
		Body->SetBoxSize(150.f, 150.f);
		Body->SetCollisionBeginFunction<CWyvern>(this, &CWyvern::OnHit);
		Body->SetDebugDraw(false);
		Body->SetEnable(true);
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);

		std::string IdleName = mColor + "Wyvern_Idle";
		std::string DieName = mColor + "Wyvern_Die";

		Anim->AddAnimation(IdleName);
		Anim->AddAnimation(DieName);
		Anim->ChangeAnimation(IdleName);
		Anim->SetLoop(IdleName, true);
		Anim->SetFinishNotify<CWyvern>(DieName, this, &CWyvern::DieFinish);
	}
	return true;
}

void CWyvern::OnHit(const FVector3& HitPoint,
	class CCollider* Dest)
{
	CMonster::OnHit(HitPoint, Dest);
	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.25f);
		}
	}
}

void CWyvern::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	if (mIsDead)
		return;

	mFloatingTime += DeltaTime;

	auto Target = mTargetObject.lock();
	if (Target)
	{
		auto Anim = mAnimation2DComponent.lock();

		if (Anim)
		{
			std::string IdleName = mColor + "Wyvern_Idle";
			std::string DieName = mColor + "Wyvern_Die";
			FVector3 TargetPos = Target->GetWorldPos();
			FVector3 MyPos = GetWorldPos();

			FVector3 TargetDir = TargetPos - MyPos;
			float Distance = TargetDir.Length();
			TargetDir.Normalize();
			if (TargetDir.x < 0)
			{
				Anim->SetSymmetry(IdleName, false);
				Anim->SetSymmetry(DieName, false);
			}
			else
			{
				Anim->SetSymmetry(IdleName, true);
				Anim->SetSymmetry(DieName, true);
			}

			FVector3 _TargetPos = TargetPos;
			if (Distance < 300)
			{
				_TargetPos = Target->GetWorldPos() + (TargetDir * 200.f);
			}
			FVector3 FinalMoveDir = _TargetPos - GetWorldPos();
			FinalMoveDir.Normalize();

			float WaveY = sinf(mFloatingTime * (float)mIndex * 2.0f) * 200.0f * (float)mIndex * DeltaTime;

			FVector3 FinalDir = FinalMoveDir * mSpeed * DeltaTime;
			FinalDir.y += WaveY;

			AddWorldPos(FinalDir);
		}
	}
}
void CWyvern::Damage(float Damage)
{
	CMonster::Damage(Damage);
	auto State = mStateComponent.lock();
	if (State)
	{
		if (State->IsDead())
		{
			mIsDead = true;
			auto Anim = mAnimation2DComponent.lock();
			auto Mesh = mMeshComponent.lock();

			if (Anim)
			{
				std::string Name = GetName();
				char IndexChar = Name.back();
				std::string IndexStr(1, IndexChar);
				if (Mesh)
				{
					Mesh->SetWorldScale(175, 191);
				}
				std::string DieName = mColor + "Wyvern_Die";
				Anim->ChangeAnimation(DieName);
				auto Body = mBody.lock();
				if (Body)
				{
					Body->SetEnable(false);
				}

			}

		}
	}
	auto World = mWorld.lock();
	auto Body = mBody.lock();
	float SizeY = 0;
	if (Body)
	{
		SizeY = Body->GetInfo().HalfSize.y;
	}
	auto DamageObj = World->CreateGameObject<CDamageObject>("Damage" + std::to_string(rand())).lock();
	DamageObj->SetWorldPos(GetWorldPos().x, GetWorldPos().y + SizeY);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, GetWorldPos().y + SizeY + 10);
}

void CWyvern::DieFinish()
{
	Destroy();
}
CWyvern* CWyvern::Clone()
{
	return new CWyvern(*this);
}