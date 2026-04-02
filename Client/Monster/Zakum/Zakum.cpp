#include "Zakum.h"
#include "Component/ObjectMovementComponent.h"
#include "Component/Animation2DComponent.h"
#include "Component/MeshComponent.h"
#include "World/World.h"
#include "../../Component/StateComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderLine2D.h"
#include "UI/NumberWidget.h"
#include "ZakumArm.h"
#include "Render/RenderManager.h"
#include "FallingStone.h"
#include "DestinationStone.h"
#include "../../Player/Player.h"
#include "../DamageObject.h"
#include "../../Portal/Portal.h"
#include "../../UI/MonsterHPWidget.h"
#include "../../Item/ItemManager.h"
#include "../../Item/FieldItem.h"
#include "../../Item/Item.h"

CZakum::CZakum()
{
	SetClassType<CZakum>();
}

CZakum::CZakum(const CZakum& ref)
	: CMonster(ref)
{
}


CZakum::CZakum(CZakum&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CZakum::~CZakum()
{
	
}

bool CZakum::Init()
{
	if (!CMonster::Init())
		return false;

	CItemManager::GetInst()->FindDropItemData("Zakum", mDropItemArray);

	mMeshComponent = CreateComponent<CMeshComponent>("ZakumMesh");
	mStateComponent = CreateComponent<CStateComponent>("ZakumState");
	mAnimation2DComponent = CreateComponent<CAnimation2DComponent>("ZakumAnimation2D");
	mBody = CreateComponent<CColliderBox2D>("ZakumBody");

	auto	Mesh = mMeshComponent.lock();
	auto    Anim = mAnimation2DComponent.lock();
	auto	Body = mBody.lock();
	auto	World = mWorld.lock();
	auto UIMng = World->GetUIManager().lock();
	if (UIMng)
	{
		mMonsterHPWidget = UIMng->CreateWidget<CMonsterHPWidget>("MonsterHPWidget");
		mMonsterHPWidget.lock()->AddPos(FVector2(200, 0));
		mMonsterHPWidget.lock()->SetIcon("ZakumIcon", TEXT("Slot/ZakumIcon.png"));
	}

	mMonsterHPWidgetFunc = std::bind(&CMonsterHPWidget::SetMonsterHP, mMonsterHPWidget.lock().get(),
		std::placeholders::_1, std::placeholders::_2);

	auto State = mStateComponent.lock();
	State->SetHPMax(300000);
	mSumHP += State->GetHPMax();
	if (Mesh)
	{
		Mesh->SetWorldPos(80, -230);
		Mesh->SetRenderLayer(0);
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetWorldScale(400, 400);
		Mesh->SetBlendState(0, "AlphaBlend");
	}
	if (Body)
	{
		Body->SetCollisionProfile("Monster");
		Body->SetWorldPos(80, -230);
		Body->SetWorldScale(400, 400);
		Body->SetBoxSize(200.f, 400.f);
		Body->SetDebugDraw(false);
		Body->SetEnable(true);
		Body->SetInheritScale(false);
		Body->SetCollisionBeginFunction<CZakum>(this, &CZakum::OnHit);
	}


	if (Anim)
	{
		Anim->SetUpdateComponent(Mesh);
		Anim->AddAnimation("ZakumBody_Idle");
		Anim->AddAnimation("ZakumBody_Die");
		Anim->AddAnimation("ZakumBody_Skill");
		Anim->SetPlayRate("ZakumBody_Skill", 0.7f);
		Anim->SetPlayRate("ZakumBody_Die", 0.5f);
		Anim->AddNotify<CZakum>("ZakumBody_Skill",
			"ZakumBody_Skill", 0, this, &CZakum::Skill);
		Anim->SetFinishNotify<CZakum>("ZakumBody_Skill", this, &CZakum::SkillEnd);

		Anim->ChangeAnimation("ZakumBody_Idle");

		Anim->SetLoop("ZakumBody_Idle", true);
		Anim->SetFinishNotify<CZakum>("ZakumBody_Die", this, &CZakum::DieFinish);
	}


	SetXPosList();
	GetGravityList();
	CreateArm();
	SetArmPos();
	
	return true;
}


void CZakum::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	auto    Anim = mAnimation2DComponent.lock();
	auto State = mStateComponent.lock();

	mSkillCooltime -= DeltaTime;
	if (mSkillCooltime <= 0)
	{
		mSkillCooltime = 10.f;
		if (Anim)
		{
			if (mIsDead)
				return;
			Anim->ChangeAnimation("ZakumBody_Skill");
		}
	}

	if (mMonsterHPWidgetFunc)
	{
		float HP = 0.f;
		for (int i = 0; i < 8; i++)
		{
			if (!mArmList[i].expired())
				HP += mArmList[i].lock()->GetHP();
		}
		if (State)
		{
			HP += State->GetHP();
		}

		mMonsterHPWidgetFunc(HP, mSumHP);
	}

}

void CZakum::CreateArm()
{
	auto World = mWorld.lock();
	for (int i = 1; i <= 8; ++i)
	{
		
		auto Arm = World->CreateGameObject<CZakumArm>("ZakumArm" + std::to_string(i));
		auto ArmPtr = Arm.lock();
		if (ArmPtr)
		{
			ArmPtr->SetIndex((ArmIndex::Index)i);
			mSumHP += ArmPtr->GetHPMax();
			mArmList[i - 1] = ArmPtr;
		}
	}
}

void CZakum::SetArmPos()
{
	// LT
	auto Arm = mArmList[0].lock();
	Arm->SetWorldPos(-60.f, -150.f);
	Arm->SetRelativeRotationZ(-15);
	auto Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(40);
	Body->AddWorldPos(-15, 50);

	// LMT
	Arm = mArmList[1].lock();
	Arm->SetWorldPos(-60.f, -220.f);
	Arm->SetRelativeRotationZ(-10);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(60);
	Body->AddWorldPos(-30, 40);

	// LMB
	Arm = mArmList[2].lock();
	Arm->SetWorldPos(-60.f, -270.f);
	Arm->SetRelativeRotationZ(-5);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(80);
	Body->AddWorldPos(-30, 20);

	// LB
	Arm = mArmList[3].lock();
	Arm->SetWorldPos(-60.f, -320.f);
	Arm->SetRelativeRotationZ(5);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(80);
	Body->AddWorldPos(-30, -20);

	// RT
	Arm = mArmList[4].lock();
	Arm->SetWorldPos(230.f, -150.f);
	Arm->SetRelativeRotationZ(15);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(-40);
	Body->AddWorldPos(15, 50);

	// RMT
	Arm = mArmList[5].lock();
	Arm->SetWorldPos(230.f, -220.f);
	Arm->SetRelativeRotationZ(10);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(-60);
	Body->AddWorldPos(20, 40);

	// RMB
	Arm = mArmList[6].lock();
	Arm->SetWorldPos(230.f, -270.f);
	Arm->SetRelativeRotationZ(5);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(-80);
	Body->AddWorldPos(20, 20);

	// RB
	Arm = mArmList[7].lock();
	Arm->SetWorldPos(230.f, -320.f);
	Arm->SetRelativeRotationZ(-5);
	Body = Arm->GetBody().lock();
	Body->AddRelativeRotationZ(-80);
	Body->AddWorldPos(30, -20);
}

void CZakum::ShuffleXPosList()
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

void CZakum::OnHit(const FVector3& HitPoint,
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

void CZakum::SetXPosList()
{
	for (float i = -550.f; i < 550; i += 100)
	{
		XPosList.push_back(i);
	}
}

void CZakum::GetGravityList()
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

void CZakum::Skill()
{

	auto World = mWorld.lock();
	if(World)
	{
		ShuffleXPosList();
		for (int i = 0; i < 8; i++)
		{
			auto _Stone = World->CreateGameObject<CDestinationStone>("Stone" + std::to_string(rand()));
			auto Stone = _Stone.lock();
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
			Stone->SetWorldPos(RandomX, RandomY + Stone->GetWorldScale().y * 0.5f - 5);
			Stone->SetTargetPos(Collider->GetWorldPos());
		}
	}
}


void CZakum::SkillEnd()
{
	auto Anim = mAnimation2DComponent.lock();
	if (Anim)
	{
		Anim->ChangeAnimation("ZakumBody_Idle");
	}

}

void CZakum::DieFinish()
{
	Destroy();
	CreatePortal();
	DropItems();
}

void CZakum::CreatePortal()
{
	std::weak_ptr<CPortal> Portal = mWorld.lock()->CreateGameObject<CPortal>("Portal");
	auto _Portal = Portal.lock();
	if (_Portal)
	{
		_Portal->SetName("Portal");
		_Portal->SetWorldPos(100, -370, 0);
		_Portal->SetLoadType(EWorldType::Horntail);
	}
}
void CZakum::DropItems()
{
	auto World = mWorld.lock();

	auto iter = mDropItemArray.begin();
	auto iterEnd = mDropItemArray.end();
	for (; iter != iterEnd; iter++)
	{
		int Rand = rand() % 100;
		if (iter->DropPercent >= Rand)
		{
			int Count = iter->MinCount + (rand() % (iter->MaxCount - iter->MinCount + 1));
			for (int i = 0; i < Count; i++)
			{
				auto FieldItem = World->CreateGameObject<CFieldItem>(iter->Name + "FieldItem" + std::to_string(i)).lock();
				auto Item = World->CreateGameObject<CItem>(iter->Name + "Item" + std::to_string(i)).lock();
				Item->SetData(*iter);
				FieldItem->SetItem(Item);
				mDropResultArray.push_back(FieldItem);
			}
		}
	}
	for (size_t i = 0; i < mDropResultArray.size(); ++i)
	{
		float distanceStep = (float)((i + 1) / 2);
		float dir = 0.f;
		if (i % 2 == 0)
			dir = 1.f;
		else
			dir = -1.f;
		float speedX = distanceStep * 60.f * dir;
		float speedY = 500.f + (i + 1) / 2.f * 60.f;
		mDropResultArray[i]->SetWorldPos(GetWorldPos().x, GetWorldPos().y - 200.f);
		mDropResultArray[i]->StartDropArray(speedX, speedY);
	}
}
CZakum* CZakum::Clone()
{
	return new CZakum(*this);
}

void CZakum::Damage(float Damage)
{
	auto State = mStateComponent.lock();

	bool IsArmClear = true;
	for (int i = 0; i < 8; i++)
	{
		if (!mArmList[i].expired())
		{
			IsArmClear = false;
			break;
		}
	}

	if (!IsArmClear)
		return;

	CMonster::Damage(Damage);
	if (State)
	{
		if (State->IsDead())
		{
			auto Anim = mAnimation2DComponent.lock();
			auto Mesh = mMeshComponent.lock();
			auto Body = mBody.lock();
			if (Body)
			{
				Body->SetEnable(false);
			}
			if (Anim)
			{
				Anim->ChangeAnimation("ZakumBody_Die");
				mIsDead = true;
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
	DamageObj->SetHitWorldPos(GetWorldPos().x - 45, GetWorldPos().y + SizeY + 10);
}