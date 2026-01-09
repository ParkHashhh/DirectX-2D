#include "MonsterSpawnPoint.h"
#include "Component/SceneComponent.h"
#include "Monster.h"
#include "World/World.h"

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

	// 만약 생성한 몬스터가 제가 되었거나 처음 시작한다면
	// 몬스터를 생성한다.
	auto begin = mSpawnMonsterList.begin();
	auto end = mSpawnMonsterList.end();
	for (; begin != end;)
	{
		if (begin->expired())
			begin = mSpawnMonsterList.erase(begin);
		else
			begin++;
	}
	if ((int)mSpawnMonsterList.size() < 20)
	{	
		mTime += DeltaTime;

		if (mTime >= mSpawnTime)
		{
			mTime = 0.f;

			std::weak_ptr<CObject> Origin = CObject::FindCDO(mSpawnClass);

			auto OriginMonster = std::dynamic_pointer_cast<CMonster>(Origin.lock());

			auto	World = mWorld.lock();

			std::weak_ptr<CMonster> SpawnMonster = World->CreateCloneGameObject<CMonster>("Monster", OriginMonster);

			auto	Monster = std::dynamic_pointer_cast<CMonster>(SpawnMonster.lock());
			int Sidepos = rand() % 4;
			int RandWidth = rand() % 1200 - 600;
			int RandHeight = rand() % 640 - 320;


			FVector3 SpawnPos = FVector3::Zero;

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