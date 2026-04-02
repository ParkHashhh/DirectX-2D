#include "LeftHead.h"
#include "Component/MeshComponent.h"
#include "Component/Animation2DComponent.h"
#include "World/World.h"
#include "Component/ColliderBox2D.h"
#include "../../Player/Player.h"
#include "LeftHeadObject.h"
#include "../../Component/StateComponent.h"
#include "../DamageObject.h"


CLeftHead::CLeftHead()
{
	SetClassType<CLeftHead>();
}

CLeftHead::CLeftHead(const CLeftHead& ref) :
	CHornTailPart(ref)
{
}

CLeftHead::CLeftHead(CLeftHead&& ref) noexcept :
	CHornTailPart(std::move(ref))
{
}

CLeftHead::~CLeftHead()
{
}

bool CLeftHead::Init()
{
	CMonster::Init();

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailLeftHeadMesh");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("HornTailLeftHeadAnim");
	mBody = CreateComponent<CColliderBox2D>("HornTailLeftHeadBody");
	mStateComponent = CreateComponent<CStateComponent>("HornTailLeftHeadState");

	// 애니메이션 지정

	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->SetUpdateComponent(mMeshComponent);

		Anim->AddAnimation("HornTailLeftHead_Idle");
		Anim->AddAnimation("HornTailLeftHead_Attack");
		Anim->AddAnimation("HornTailLeftHead_Die");

		Anim->SetLoop("HornTailLeftHead_Idle", true);

		Anim->SetPlayRate("HornTailLeftHead_Idle", 0.15f);
		Anim->SetPlayRate("HornTailLeftHead_Attack", 0.5f);
		Anim->SetPlayRate("HornTailLeftHead_Die", 0.3f);


		Anim->AddNotify<CLeftHead>("HornTailLeftHead_Attack",
			"HornTailLeftHead_Attack", 0, this, &CLeftHead::SkillStart);
		Anim->SetFinishNotify<CLeftHead>("HornTailLeftHead_Attack", this, &CLeftHead::SkillEnd);

		Anim->AddNotify<CLeftHead>("HornTailLeftHead_Die",
			"HornTailLeftHead_Die", 20, this, &CLeftHead::StartDie);
		Anim->SetFinishNotify<CLeftHead>("HornTailLeftHead_Die", this, &CLeftHead::EndDie);


		Anim->ChangeAnimation("HornTailLeftHead_Idle");
	}
	auto	Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(313.f, 618.f);
		Mesh->SetRenderLayer(0);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	auto Body = mBody.lock();
	if (Body)
	{
		Body->SetBoxSize(150.f, 150.f);
		Body->AddWorldPos(-50, 200);
		Body->SetEnable(true);
		Body->SetDebugDraw(false);
		Body->SetCollisionProfile("Monster");
		Body->SetCollisionBeginFunction<CLeftHead>(this, &CLeftHead::OnHit);
	}
	GetGravityList();
	SetXPosList();
	SetHPMax(500000.f);

	return true;
}

void CLeftHead::SetXPosList()
{
	for (float i = -550.f; i < 700; i += 150.f)
	{
		XPosList.push_back(i);
	}
}
void CLeftHead::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}


void CLeftHead::GetGravityList()
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
				if (iter->expired())
					continue;

				if (iter->lock()->GetCollisionProfile()->Name == "Gravity")
				{
					mGravityList.push_back(*iter);
				}
			}

		}
	}
}

void CLeftHead::OnHit(const FVector3& HitPoint, class CCollider* Dest)
{

	if (Dest->GetCollisionProfile()->Name == "Player")
	{
		auto Owner = std::dynamic_pointer_cast<CPlayer>(Dest->GetOwner().lock());
		if (Owner)
		{
			Owner->TakeDamage(Owner->GetHPMax() * 0.25f,false);
		}
	}
}

CLeftHead* CLeftHead::Clone()
{
	return new CLeftHead(*this);
}

void CLeftHead::ShuffleXPosList()
{
	int ShuffleCount = 30;

	for (int i = 0; i < ShuffleCount; i++)
	{
		int rand1 = rand() % (XPosList.size());
		int rand2 = rand() % (XPosList.size());
		float temp = XPosList[rand1];
		XPosList[rand1] = XPosList[rand2];
		XPosList[rand2] = temp;
	}
}

void CLeftHead::StartDie()
{

	auto Mesh = mMeshComponent.lock();
	auto Body = mBody.lock();
	if (Mesh)
	{
		Mesh->SetRelativePos(mDiePos);
		Mesh->SetWorldScale(304.f, 606.f);
	}
	if (Body)
	{
		Body->SetEnable(false);
	}
	mIsDead = true;

}

void CLeftHead::EndDie()
{
}

void CLeftHead::ChangeAttack()
{
	auto	Anim = mAnimation2DComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation("HornTailLeftHead_Attack");
		
	}
}

void CLeftHead::SkillStart()
{
	auto Mesh = mMeshComponent.lock();
	if (Mesh)
	{
		Mesh->SetWorldScale(423.6f, 606.f);

	}
	auto World = mWorld.lock();
	if (World)
	{
		ShuffleXPosList();
		for (int i = 0; i < 8; i++)
		{
			auto _LeftHeadObject = World->CreateGameObject<CLeftHeadObject>("LeftHeadObject" + std::to_string(rand()));
			auto LeftHeadObject = _LeftHeadObject.lock();
			float RandomX = XPosList[i];
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
					if (RandomX >= (CenterX - HalfX) && RandomX <= (CenterX + HalfX))
					{
						BoxList.push_back(Box);
					}
				}
			}
			if (BoxList.empty())
				continue;
			int Idx = rand() % (int)BoxList.size();
			auto Collider = BoxList[Idx].lock();
			float RandomY = Collider->GetWorldPos().y + Collider->GetInfo().HalfSize.y;
			RandomX += (rand() % 41 - 20);
			LeftHeadObject->SetWorldPos(RandomX, RandomY + LeftHeadObject->GetWorldScale().y * 0.5f - 5);
		}
	}
}
void CLeftHead::SkillEnd()
{
	auto	Anim = mAnimation2DComponent.lock();
	auto	Mesh = mMeshComponent.lock();

	if (Anim)
	{
		Anim->ChangeAnimation("HornTailLeftHead_Idle");
		if (Mesh)
		{
			Mesh->SetWorldScale(313.f, 618.f);
		}
	}
}

void CLeftHead::Damage(float Damage)
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
				Anim->ChangeAnimation("HornTailLeftHead_Die");
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
	DamageObj->SetWorldPos(GetWorldPos().x - 70, GetWorldPos().y + SizeY + 200);
	DamageObj->SetDamage(int(Damage));
	DamageObj->SetHitWorldPos(DamageObj->GetWorldPos().x - 45, DamageObj->GetWorldPos().y + 10);
}