#include "ZakumArm.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "UI/NumberWidget.h"
#include "ArmSkill.h"
#include "../../Player/Player.h"
#include "../DamageObject.h"


CZakumArm::CZakumArm()
{

	SetClassType<CZakumArm>();
}

CZakumArm::CZakumArm(const CZakumArm& ref)
	: CMonster(ref)
{
}


CZakumArm::CZakumArm(CZakumArm&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CZakumArm::~CZakumArm()
{

}

bool CZakumArm::Init()
{
	if (!CMonster::Init())
		return false;
	std::string Name = GetName();
	char IndexChar = Name.back();
	std::string IndexStr(1, IndexChar);

	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("ZakumArmAnimation2D" + IndexStr);
	mMeshComponent = CreateComponent<CMeshComponent>("ZakumArmMesh" + IndexStr);
	mBody = CreateComponent<CColliderBox2D>("ZakumArmBody" + IndexStr);
	mStateComponent = CreateComponent<CStateComponent>("ZakumArmState" + IndexStr);

	auto State = mStateComponent.lock();
	auto Mesh = mMeshComponent.lock();
	auto Anim = mAnimation2DComponent.lock();
	auto Body = mBody.lock();
	if (State)
	{
		State->SetHPMax(100000.f);
	}
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetBlendState(0, "AlphaBlend");
		Mesh->SetWorldScale(300, 250);
		Mesh->SetRenderLayer(0);
	}
	if (Body)
	{
		Body->SetCollisionProfile("Monster");
		Body->SetWorldScale(300, 250);
		Body->SetBoxSize(80.f, 150.f);
		Body->SetInheritScale(false);
		Body->SetCollisionBeginFunction<CZakumArm>(this, &CZakumArm::OnHit);
		Body->SetDebugDraw(false);
	}

	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		std::string IdleName = "ZakumArm" + IndexStr + "_Idle";
		std::string DieName = "ZakumArm" + IndexStr + "_Die";

		Anim->AddAnimation(IdleName);
		Anim->AddAnimation(DieName);
		Anim->ChangeAnimation(IdleName);
		Anim->SetLoop(IdleName, true);
		Anim->SetFinishNotify<CZakumArm>(DieName, this, &CZakumArm::DieFinish);
	}
	GetGravityList();
	return true;
}

void CZakumArm::GetGravityList()
{
	auto World = mWorld.lock();
	if (World)
	{
		auto Collision = World->GetCollision().lock();
		if (!Collision)
			return;
		auto List = Collision->GetColliderList();

		if (!List.empty())
		{
			auto iter = List.begin();
			auto iterEnd = List.end();
			for (; iter != iterEnd; iter++)
			{
				if (iter->lock()->GetCollisionProfile()->Name == "Gravity")
				{
					mGravityList.push_back(*iter);
				}
			}

		}
	}
}


float CZakumArm::GetHPMax()
{
	auto State = mStateComponent.lock();
	if (State)
	{
		return State->GetHPMax();
	}
	return -1.f;

}

float CZakumArm::GetHP()
{
	auto State = mStateComponent.lock();
	if (State)
	{
		return State->GetHP();
	}
	return -1.f;
}
void CZakumArm::SetMeshEnable(bool Enable)
{
	auto Mesh = mMeshComponent.lock();
	if (!Mesh)
		return;
	
	Mesh->SetEnable(Enable);
}

void CZakumArm::SetBodyEnable(bool Enable)
{
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetEnable(Enable);
	}
}

void CZakumArm::OnHit(const FVector3& HitPoint,
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

void CZakumArm::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	mSkillCoolTime -= DeltaTime;
	if (mSkillCoolTime <= 0)
	{
		SkillProcess();
	}
}


void CZakumArm::SetIndex(ArmIndex::Index Index)
{
	mIndex = Index;
	SetXPos();
}

void CZakumArm::Damage(float Damage)
{
	CMonster::Damage(Damage);
	auto State = mStateComponent.lock();

	if (State)
	{
		if (State->IsDead())
		{
			auto Anim = mAnimation2DComponent.lock();
			auto Mesh = mMeshComponent.lock();

			if (Anim)
			{
				std::string Name = GetName();
				char IndexChar = Name.back();
				std::string IndexStr(1, IndexChar);
				if (Mesh)
				{
					Mesh->SetWorldScale(400, 350);
				}
				Anim->ChangeAnimation("ZakumArm" + IndexStr + "_Die");
			}

		}
	}
	auto World = mWorld.lock();
	auto Body = mBody.lock();
	float SizeY = 0;
	int XOffset = -60;

	if (mIndex >= 5)
	{
		XOffset *= -1;
	}
	if (Body)
	{
		SizeY = Body->GetInfo().HalfSize.y;
	}
	auto DamageObj = World->CreateGameObject<CDamageObject>("Damage" + std::to_string(rand())).lock();
	DamageObj->SetWorldPos(GetWorldPos().x + XOffset, GetWorldPos().y + SizeY);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, GetWorldPos().y + SizeY + 10);
}

void CZakumArm::SetXPos()
{
	if (mIndex <= 4)
	{
		// -500 -350 -200 -50 
		XPos = -400.f + ((mIndex - 1) * 150.f);
	}
	else
	{
		// 50 200 350 500
		XPos = 150.f + ((mIndex - 5) * 150.f);
	}
}


void CZakumArm::DieFinish()
{
	Destroy();
}
void CZakumArm::SkillProcess()
{
	mSkillCoolTime = 7.5f;
	if (rand() % 2 == 1)
	{
		auto Mesh = mMeshComponent.lock();
		if (Mesh)
		{
			Mesh->SetEnable(false);
		}
		auto Body = mBody.lock();
		if (Body)
		{
			Body->SetEnable(false);
		}
		auto World = mWorld.lock();
		if (World)
		{
			auto SkillBox = World->CreateGameObject<CArmSkill>("ArmSkill" + std::to_string(rand()));

			auto _SkillBox = SkillBox.lock();
			_SkillBox->SetOwner(std::dynamic_pointer_cast<CZakumArm>(mSelf.lock()));

			if (mIndex >= 5)
			{
				_SkillBox->SetSymmetry(true);
			}
			// 여기서 Y를 GravityCollider들을 찾아서 자기 X좌표에 있는 Collider중 랜덤으로 하나 선택해서 Y좌표를 넣어야함
			auto iter = mGravityList.begin();
			auto iterEnd = mGravityList.end();

			std::vector<std::weak_ptr<CColliderBox2D>> BoxList;
			for (; iter != iterEnd; iter++)
			{
				auto Box = std::dynamic_pointer_cast<CColliderBox2D>(iter->lock());
				if (Box)
				{
					float HalfX = Box->GetInfo().HalfSize.x;
					float CenterX = Box->GetWorldPos().x;
					if (XPos >= (CenterX - HalfX) && XPos <= (CenterX + HalfX))
					{
						BoxList.push_back(Box);
					}
				}
			}
			if (BoxList.empty())
				return;

			int Idx = rand() % (int)BoxList.size();
			auto Collider = BoxList[Idx].lock();
			float RandomY = Collider->GetWorldPos().y + Collider->GetInfo().HalfSize.y;
			XPos += (rand() % 41 - 20);
			_SkillBox->SetWorldPos(XPos, RandomY + _SkillBox->GetWorldScale().y * 0.5f - 50);

		}
	}
}
CZakumArm* CZakumArm::Clone()
{
	return new CZakumArm(*this);
}