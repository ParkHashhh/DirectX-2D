#include "HornTail.h"
#include "Leg.h"
#include "Tail.h"
#include "World/World.h"
#include "Component/MeshComponent.h"
#include "LeftHand.h"
#include "RightHand.h"
#include "LeftHead.h"
#include "RightHead.h"
#include "MiddleHead.h"
#include "Wing.h"
#include "Component/ColliderBox2D.h"
#include "../../UI/MonsterHPWidget.h"
#include "../../Portal/Portal.h"
#include "../../Item/ItemManager.h"
#include "../../Item/FieldItem.h"
#include "../../Item/Item.h"

CHornTail::CHornTail()
{
	SetClassType<CHornTail>();
}

CHornTail::CHornTail(const CHornTail& ref)
	: CMonster(ref)
{
}


CHornTail::CHornTail(CHornTail&& ref) noexcept
	: CMonster(std::move(ref))
{
}
CHornTail::~CHornTail()
{

}

bool CHornTail::Init()
{
	if (!CMonster::Init())
		return false;
	CItemManager::GetInst()->FindDropItemData("HornTail", mDropItemArray);

	mMeshComponent = CreateComponent<CMeshComponent>("HornTailRoot");
	auto Mesh = mMeshComponent.lock();
	auto World = mWorld.lock();

	auto UIMng = World->GetUIManager().lock();
	if (UIMng)
	{
		mMonsterHPWidget = UIMng->CreateWidget<CMonsterHPWidget>("MonsterHPWidget");
		mMonsterHPWidget.lock()->AddPos(FVector2(200, 0));
	}
	if (World)
	{
		mTail = World->CreateGameObject<CTail>("Tail").lock();
		mTail->SetRelativePos(250, -30);
		mTail->SetDiePos(mTail->GetRelativePos());
		mHPMax += mTail->GetHPMax();

		mLeg = World->CreateGameObject<CLeg>("Leg").lock();
		mLeg->SetRelativePos(0, -10);
		mLeg->SetDiePos(mLeg->GetRelativePos());

		mWing = World->CreateGameObject<CWing>("Wing").lock();
		mWing->SetRelativePos(0, 140);
		mWing->SetDiePos(mWing->GetRelativePos());


		mLeftHand = World->CreateGameObject<CLeftHand>("LeftHand").lock();
		mLeftHand->SetRelativePos(-128, 80);
		mLeftHand->SetDiePos(mLeftHand->GetRelativePos());

		mRightHand = World->CreateGameObject<CRightHand>("RightHand").lock();
		mRightHand->SetRelativePos(128,80);
		mRightHand->SetDiePos(mRightHand->GetRelativePos());

		mLeftHead = World->CreateGameObject<CLeftHead>("LeftHead").lock();
		mLeftHead->SetRelativePos(-120, 197);
		mLeftHead->SetDiePos(mLeftHead->GetRelativePos());

		mRightHead = World->CreateGameObject<CRightHead>("RightHead").lock();
		mRightHead->SetRelativePos(120,197);
		mRightHead->SetDiePos(mRightHead->GetRelativePos());

		mMiddleHead = World->CreateGameObject<CMiddleHead>("MiddleHead").lock();
		mMiddleHead->SetRelativePos(0, 257);
		mMiddleHead->SetInheritScale(false);
		mMiddleHead->SetDiePos(mMiddleHead->GetRelativePos());

	}

	SetHornTailHP(300000.f);
	mHPMax = 0.f;
	mHPMax += mLeftHead->GetHPMax();
	mHPMax += mRightHead->GetHPMax();
	mHPMax += mMiddleHead->GetHPMax();
	mHPMax += mLeftHand->GetHPMax();
	mHPMax += mRightHand->GetHPMax();
	mHPMax += mLeg->GetHPMax();
	mHPMax += mTail->GetHPMax();



	if (Mesh)
	{
		Mesh->SetWorldScale(1, 1, 1);
		Mesh->AddChild(mTail->GetRootComponent());
		Mesh->AddChild(mLeg->GetRootComponent());
		Mesh->AddChild(mLeftHand->GetRootComponent());
		Mesh->AddChild(mRightHand->GetRootComponent());
		Mesh->AddChild(mLeftHead->GetRootComponent());
		Mesh->AddChild(mMiddleHead->GetRootComponent());
		Mesh->AddChild(mRightHead->GetRootComponent());
		Mesh->AddChild(mWing->GetRootComponent());
	}

	mMonsterHPWidgetFunc = std::bind(&CMonsterHPWidget::SetMonsterHP, mMonsterHPWidget.lock().get(),
		std::placeholders::_1, std::placeholders::_2);
	SetRegionVector();


	return true;
}

void CHornTail::SetRegionVector()
{
	mHeadArray.push_back(mLeftHead);
	mHeadArray.push_back(mMiddleHead);
	mHeadArray.push_back(mRightHead);

	mBodyArray.push_back(mLeftHand);
	mBodyArray.push_back(mRightHand);
	
	mTailArray.push_back(mTail);
	mTailArray.push_back(mLeg);

	mFullArray.push_back(mLeftHead);
	mFullArray.push_back(mMiddleHead);
	mFullArray.push_back(mRightHead);
	mFullArray.push_back(mLeftHand);
	mFullArray.push_back(mRightHand);
	mFullArray.push_back(mTail);
	mFullArray.push_back(mLeg);
}

void CHornTail::CheckDieMonster()
{
	auto iter = mHeadArray.begin();
	for (; iter != mHeadArray.end();)
	{
		if ((*iter)->GetIsDead())
		{
			iter = mHeadArray.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	iter = mBodyArray.begin();

	for (; iter != mBodyArray.end();)
	{
		if ((*iter)->GetIsDead())
		{
			iter = mBodyArray.erase(iter);
		}
		else
		{
			iter++;
		}
	}


	iter = mTailArray.begin();

	for (; iter != mTailArray.end();)
	{
		if ((*iter)->GetIsDead())
		{
			iter = mTailArray.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	iter = mFullArray.begin();

	for (; iter != mFullArray.end();)
	{
		if ((*iter)->GetIsDead())
		{
			iter = mFullArray.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void CHornTail::DestroyMonster()
{
	mTail->Destroy();
	mLeg->Destroy();
	mLeftHand->Destroy();
	mRightHand->Destroy();
	mLeftHead->Destroy();
	mMiddleHead->Destroy();
	mRightHead->Destroy();
	mWing->Destroy();
	Destroy();
}
void CHornTail::SetHornTailHP(float HPMax)
{
	mTail->SetHPMax(HPMax);
	mLeg->SetHPMax(HPMax);
	mLeftHand->SetHPMax(HPMax);
	mRightHand->SetHPMax(HPMax);
	mLeftHead->SetHPMax(HPMax);
	mMiddleHead->SetHPMax(HPMax);
	mRightHead->SetHPMax(HPMax);
	mWing->SetHPMax(HPMax);
}
void CHornTail::ProcessSkill(float DeltaTime)
{
	if (HeadSkillDelay <= 0 && !mHeadArray.empty())
	{
		HeadSkillDelay = 10.f;
		int idx = rand() % mHeadArray.size();
		if (!mHeadArray[idx]->GetIsDead())
			mHeadArray[idx]->ChangeAttack();
	}
	if (BodySkillDelay <= 0 && !mBodyArray.empty())
	{
		BodySkillDelay = 13.f;
		int idx = rand() % mBodyArray.size();
		if (!mBodyArray[idx]->GetIsDead())
			mBodyArray[idx]->ChangeAttack();
	}
	if (TailSkillDelay <= 0 && !mTailArray.empty())
	{
		TailSkillDelay = 5.f;
		int idx = rand() % mTailArray.size();
		if (!mTailArray[idx]->GetIsDead())
			mTailArray[idx]->ChangeAttack();

	}
	if (WingSkillDelay <= 0)
	{
		WingSkillDelay = 30.f;
		int Size = (int)mFullArray.size();
		mWing->ChangeAttack();
		for (int i = 0; i < Size; i++)
		{
			if (!mFullArray[i]->GetIsDead() && mWing)
				mFullArray[i]->AddHP(mFullArray[i]->GetHPMax() * 0.1f);
		}
	}
}
void CHornTail::SetHPWidget()
{
	if (mMonsterHPWidgetFunc)
	{
		float HP = 0.f;
		if (mTail && !mTail->GetIsDead())
			HP += mTail->GetHP();
		if (mLeg && !mLeg->GetIsDead())
			HP += mLeg->GetHP();
		if (mLeftHand && !mLeftHand->GetIsDead())
			HP += mLeftHand->GetHP();
		if (mRightHand && !mRightHand->GetIsDead())
			HP += mRightHand->GetHP();
		if (mLeftHead && !mLeftHead->GetIsDead())
			HP += mLeftHead->GetHP();
		if (mMiddleHead && !mMiddleHead->GetIsDead())
			HP += mMiddleHead->GetHP();
		if (mRightHead && !mRightHead->GetIsDead())
			HP += mRightHead->GetHP();
		mMonsterHPWidgetFunc(HP, mHPMax);
	}
}
void CHornTail::DropItems()
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
		mDropResultArray[i]->SetWorldPos(GetWorldPos().x, GetWorldPos().y);
		mDropResultArray[i]->StartDropArray(speedX, speedY);
	}
}
void CHornTail::CreatePortal()
{
	std::weak_ptr<CPortal> Portal = mWorld.lock()->CreateGameObject<CPortal>("Portal");
	auto _Portal = Portal.lock();
	if (_Portal)
	{
		_Portal->SetName("Portal");
		_Portal->SetWorldPos(100, -428, 0);
		_Portal->SetLoadType(EWorldType::Ending);
	}

}
void CHornTail::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);


	HeadSkillDelay -= DeltaTime;
	BodySkillDelay -= DeltaTime;
	TailSkillDelay -= DeltaTime;
	WingSkillDelay -= DeltaTime;

	CheckDieMonster();

	if (mFullArray.empty() && !mIsEnd && mWing)
	{
		mWing->ChangeDie();
		mIsEnd = true;
	}
	if (mWing->GetIsDead())
	{
		CreatePortal();
		DropItems();
		DestroyMonster();

	}
	ProcessSkill(DeltaTime);
	SetHPWidget();
}

CHornTail* CHornTail::Clone()
{
	return new CHornTail(*this);
}

