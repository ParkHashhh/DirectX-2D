#include "MainWorld.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterSpawnPoint.h"
#include "../Monster/Goblin.h"
#include "Asset/AssetManager.h"
#include "Asset/Animation2D/Animation2DManager.h"
#include "Component/MeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderLine2D.h"


CMainWorld::CMainWorld()
{
}

CMainWorld::~CMainWorld()
{
}

bool CMainWorld::Init()
{
	CWorld::Init();
	
	LoadAnimation2D();
	std::weak_ptr<CPlayer>	Player = CreateGameObject<CPlayer>("Player");
	std::weak_ptr<CMonsterSpawnPoint>	SpawnPoint1 = CreateGameObject<CMonsterSpawnPoint>("SpawnPoint");
	std::weak_ptr<CGameObject> BoundsObj = CreateGameObject<CGameObject>("ScreenBounds");
	auto Map = BoundsObj.lock();
	//Line2D->SetWorldRotation(GetWorldRot());
	//Line2D->SetWorldPos(GetWorldPos() + GetAxis(EAxis::Y) * 50.f);
	//Line2D->SetEnable(true);
	//Line2D->SetDebugDraw(true);

	/*Line2D->SetDebugDraw(false);
	Line2D->SetInheritScale(false);
	Line2D->SetEnable(false);
	*/
	if (Map)
	{
		auto LineTop = Map->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineTop->SetCollisionProfile("Monster");
		LineTop->SetLineDistance(1280.f);
		// 가로
		LineTop->AddWorldRotationZ(-90);
		LineTop->SetWorldPos(-640,360);
		LineTop->SetInheritScale(false);
		LineTop->SetEnable(true);
		LineTop->SetDebugDraw(true);

		auto LineBottom = Map->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineBottom->SetCollisionProfile("Monster");
		LineBottom->SetLineDistance(1280.f);
		// 가로
		LineBottom->AddWorldRotationZ(-90);
		LineBottom->SetWorldPos(-640, -360);
		LineBottom->SetInheritScale(false);
		LineBottom->SetEnable(true);
		LineBottom->SetDebugDraw(true);

		auto LineLeft = Map->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineLeft->SetCollisionProfile("Monster");
		LineLeft->SetLineDistance(720.f);
		// 가로
		LineLeft->AddWorldRotationZ(180);
		LineLeft->SetWorldPos(-640, 360);
		LineLeft->SetInheritScale(false);
		LineLeft->SetEnable(true);
		LineLeft->SetDebugDraw(true);

		auto LineRight = Map->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineRight->SetCollisionProfile("Monster");
		LineRight->SetLineDistance(720.f);
		// 가로
		LineRight->AddWorldRotationZ(180);
		LineRight->SetWorldPos(640, 360);
		LineRight->SetInheritScale(false);
		LineRight->SetEnable(true);
		LineRight->SetDebugDraw(true);

	}

	std::shared_ptr<CMonsterSpawnPoint>	Point = SpawnPoint1.lock();
	if (Point)
	{
		Point->SetSpawnClass<CGoblin>();
		Point->SetSpawnTime(1.f);
	}

	return true;
}

void CMainWorld::LoadAnimation2D()
{
	// LoadArrowTex
	mWorldAssetManager->LoadTextureFullPath("Arrow", L"../Binary/Asset/Texture/Player/Item/Arrow.png");
	
	// CreateIdleAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerIdle", "PlayerIdle",0, 17, "Player/Idle/",
		"0_Forest_Ranger_Idle_", "png", 900, 900);

	// CreateRunAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerRun", "PlayerRun", 0,11, "Player/Running/",
		"0_Forest_Ranger_Running_", "png", 900, 900);

	// CreateAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerShoot", "PlayerShoot", 0,8, "Player/Shooting/",
		"0_Forest_Ranger_Shooting_", "png", 900, 900);
	// CreateRunShootAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerRunShoot", "PlayerRunShoot", 0,11, "Player/Run_Shooting/",
		"0_Forest_Ranger_Run_Shooting_", "png", 900, 900);

	// CreateSlideAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerSlide", "PlayerSlide", 0, 5, "Player/Sliding/",
		"0_Forest_Ranger_Sliding_", "png", 900, 900);
	// CreateDeadAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerDead", "PlayerDead", 0, 14, "Player/Dead/",
		"0_Forest_Ranger_Dying_", "png", 900, 900);

	// CreateGoblinRunAnim

	if (!mWorldAssetManager->CreateFrameAnimation(
		"GoblinRun", "GoblinRun", 0, 11, "Monster/Goblin/Running/",
		"0_Goblin_Running_", "png", 900, 900))
		return;
	// CreateGoblinAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"GoblinAttack", "GoblinAttack", 0, 11, "Monster/Goblin/Attack/",
		"0_Goblin_Attack_", "png", 900, 900);

	// CreateOgreRunAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OgreRun", "OgreRun", 0, 11, "Monster/Ogre/Running/",
		"0_Ogre_Running_", "png", 900, 900);
	// CreateOgreAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OgreAttack", "OgreAttack", 0, 11, "Monster/Ogre/Attack/",
		"0_Ogre_Slashing_", "png", 900, 900);

	// CreateOrcRunAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OrcRun", "OrcRun", 0, 11, "Monster/Orc/Running/",
		"0_Orc_Running_", "png", 900, 900);
	// CreateOgreAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OrcAttack", "OrcAttack", 0, 11, "Monster/Orc/Attack/",
		"0_Orc_Slashing_", "png", 900, 900);


	mWorldAssetManager->CreateFrameAnimation(
		"Explosion", "Explosion",1,89, "Explosion/",
		"Explosion", "png", 320.f, 240.f,false);


	
}
