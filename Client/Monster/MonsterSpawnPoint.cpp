#include "MonsterSpawnPoint.h"
#include "Component/SceneComponent.h"
#include "Monster.h"
#include "World/World.h"
#include "Goblin.h"
#include "Ogre.h"
#include "Orc.h"
#include "Ballock.h"





namespace SpawnPos
{
	enum Type : unsigned char
	{
		Top,
		Bottom,
		Left,
		Right
	};
}
namespace Monster
{
	enum Type : unsigned char
	{
		Goblin,
		Ogre,
		Orc
	};
}


CMonsterSpawnPoint::CMonsterSpawnPoint()
{
	SetClassType<CMonsterSpawnPoint>();
}

CMonsterSpawnPoint::CMonsterSpawnPoint(const CMonsterSpawnPoint& ref) :
	CGameObject(ref)
{
}

CMonsterSpawnPoint::CMonsterSpawnPoint(CMonsterSpawnPoint&& ref) noexcept :
	CGameObject(std::move(ref))
{
}

CMonsterSpawnPoint::~CMonsterSpawnPoint()
{
}

bool CMonsterSpawnPoint::Init()
{
	CGameObject::Init();
	CreateComponent<CSceneComponent>("Root");

	return true;
}

void CMonsterSpawnPoint::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	auto World = mWorld.lock();
	mLevelTime += DeltaTime;
	
	if (mLevelTime >= 10.f)
	{
		if (mLevel < MAX_LEVEL)
		{
			mLevelTime -= 30.f;
			mLevel++;
			mSpawnTime -= 0.2f;
		}
		if (mLevel == MAX_LEVEL)
		{

			std::weak_ptr<CObject> Origin = CObject::FindCDO(typeid(CBallock).hash_code());
			auto OriginMonster = std::dynamic_pointer_cast<CMonster>(Origin.lock());

			if (OriginMonster)
			{
				std::string MonsterName = "Monster_" + std::to_string(rand() % 10000);
				std::weak_ptr<CMonster> SpawnMonster = World->CreateCloneGameObject<CMonster>(MonsterName, OriginMonster);
				auto Monster = SpawnMonster.lock();
				Monster->SetMonsterData();
				mSpawnMonsterList.push_back(SpawnMonster);
				mBossMonster = SpawnMonster;
			}
			mLevel++;
			OutputDebugStringA("### BOSS BALLOCK SPAWNED! ###\n");
		}
	}

	// 플레이어
	if (World)
	{
		if (World->GetPlayerIsDead())
		{
			auto iter = mSpawnMonsterList.begin();
			auto iterEnd = mSpawnMonsterList.end();
			for (; iter != iterEnd; iter++)
			{
				auto Monster = iter->lock();
				if(Monster)
					Monster->Destroy();
			}
			mSpawnMonsterList.clear();
			return;
		}
		else if (World->GetBallockIsDead())
		{
			auto iter = mSpawnMonsterList.begin();
			auto iterEnd = mSpawnMonsterList.end();
			for (; iter != iterEnd; iter++)
			{
				auto Monster = iter->lock();
				if (Monster)
					Monster->Destroy();
			}
			mSpawnMonsterList.clear();
			return;
		}
	}


	// 죽은몬스터 제거
	auto iter = mSpawnMonsterList.begin();

	while (iter != mSpawnMonsterList.end())
	{
		if (iter->expired())
		{
			iter = mSpawnMonsterList.erase(iter);
		}
		else
		{
			++iter;
		}
	}


	if ((int)mSpawnMonsterList.size() < 10)
	{	
		mTime += DeltaTime;

		if (mTime >= mSpawnTime)
		{
			mTime = 0.f;


			int MonsterType = rand() % mLevel;

			// 몬스터 해쉬코드 받아오기
			switch (MonsterType)
			{
			case Monster::Goblin:
				mSpawnClass = typeid(CGoblin).hash_code();
				break;
			case Monster::Ogre:
				mSpawnClass = typeid(COgre).hash_code();
				break;
			case Monster::Orc:
				mSpawnClass = typeid(COrc).hash_code();
				break;
			}

			std::weak_ptr<CObject> Origin = CObject::FindCDO(mSpawnClass);
			auto OriginMonster = std::dynamic_pointer_cast<CMonster>(Origin.lock());

			if (OriginMonster)
			{
				std::string MonsterName = "Monster_" + std::to_string(rand() % 10000);
				std::weak_ptr<CMonster> SpawnMonster = World->CreateCloneGameObject<CMonster>(MonsterName, OriginMonster);
				auto Monster = SpawnMonster.lock();
				Monster->SetMonsterData();
				bool InRange = false;
				FVector3 SpawnPos = FVector3::Zero;
				int SafetyCount = 0;
				while (!InRange && SafetyCount < 10)
				{
					SafetyCount++;
					InRange = true;
					int Sidepos = rand() % 4;
					int RandWidth = rand() % 1400 - 700;
					int RandHeight = rand() % 800 - 400;



					// 생성위치
					switch (Sidepos)
					{
					case SpawnPos::Top:
						SpawnPos.x = RandWidth;
						SpawnPos.y = 400;
						break;
					case SpawnPos::Bottom:
						SpawnPos.x = RandWidth;
						SpawnPos.y = -400;
						break;
					case SpawnPos::Left:
						SpawnPos.y = RandHeight;
						SpawnPos.x = -700;
						break;
					case SpawnPos::Right:
						SpawnPos.x = 700;
						SpawnPos.y = RandHeight;
						break;
					}

					// 순회한다음 각 Dist안에 몬스터가 없으면 ㄱㄱ
					auto iter = mSpawnMonsterList.begin();
					auto iterEnd = mSpawnMonsterList.end();

					for (; iter != iterEnd; iter++)
					{
						float Dist = (iter->lock()->GetWorldPos() - SpawnPos).Length();
						if (Dist < mDist)
						{
							InRange = false;
							break;
						}
					}
				}
				
			

				// 캐릭터쪽 따라가기
				auto Target = Monster->GetTargetObject().lock();
				if (!Target)
					return;
				FVector3	TargetPos = Target->GetWorldPos();
				FVector3 DirToTarget = TargetPos - SpawnPos;
				DirToTarget.Normalize();
				float Angle = SpawnPos.GetViewTargetAngle2D(TargetPos, EAxis::Y);
				Monster->SetWorldRotationZ(Angle);

				Monster->SetWorldPos(SpawnPos);
				mSpawnMonsterList.push_back(SpawnMonster);
			}
		}
	}
}