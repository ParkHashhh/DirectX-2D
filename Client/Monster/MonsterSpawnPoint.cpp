#include "MonsterSpawnPoint.h"
#include "Component/SceneComponent.h"
#include "Monster.h"
#include "World/World.h"
#include "Goblin.h"
#include "Ogre.h"
#include "Orc.h"

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
	mLevelTime += DeltaTime;
	if (mLevelTime >= 30.f)
	{
		if (mLevel < MAX_LEVEL)
		{
			mLevelTime -= 30.f;
			mLevel++;
			mSpawnTime -= 0.2f;
		}
	}

	// 플레이어 죽으면 끝
	auto World = mWorld.lock();
	if (World)
	{
		if (World->GetPlayerIsDead())
			return;
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


	if ((int)mSpawnMonsterList.size() < 20)
	{	
		mTime += DeltaTime;

		if (mTime >= mSpawnTime)
		{
			mTime = 0.f;


			int MonsterType = rand() % mLevel;

			auto	World = mWorld.lock();
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
				int Sidepos = rand() % 4;
				int RandWidth = rand() % 1200 - 600;
				int RandHeight = rand() % 640 - 320;


				FVector3 SpawnPos = FVector3::Zero;
				// 생성위치
				switch (Sidepos)
				{
				case SpawnPos::Top:
					SpawnPos.x = RandWidth;
					SpawnPos.y = 320;
					break;
				case SpawnPos::Bottom:
					SpawnPos.x = RandWidth;
					SpawnPos.y = -320;
					break;
				case SpawnPos::Left:
					SpawnPos.y = RandHeight;
					SpawnPos.x = -600;
					break;
				case SpawnPos::Right:
					SpawnPos.x = 600;
					SpawnPos.y = RandHeight;
					break;
				}
				// 캐릭터쪽 따라가기
				auto Target = Monster->GetTargetObject().lock();
				FVector3	TargetPos = Target->GetWorldPos();
				FVector3 DirToTarget = TargetPos - SpawnPos;
				DirToTarget.Normalize();
				float Angle = SpawnPos.GetViewTargetAngle2D(TargetPos, EAxis::Y);
				Monster->SetWorldRotationZ(Angle);

				Monster->SetWorldPos(SpawnPos);
				Monster->SetWorldRotationZ(Angle);
				mSpawnMonsterList.push_back(SpawnMonster);
			}
		}
	}
}