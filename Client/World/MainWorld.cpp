#include "MainWorld.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterSpawnPoint.h"
#include "../Monster/Goblin.h"
#include "Asset/AssetManager.h"
#include "Render/RenderManager.h"
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
	std::weak_ptr<CMonsterSpawnPoint>	SpawnPoint1 = CreateGameObject<CMonsterSpawnPoint>("SpawnPoint");
	std::weak_ptr<CGameObject> BoundsObj = CreateGameObject<CGameObject>("ScreenBounds");

	auto BackObj = CreateGameObject<CGameObject>("Background").lock();
	auto Mesh = BackObj->CreateComponent<CMeshComponent>("BackMesh").lock();
	auto AssetMng = GetWorldAssetManager().lock();
	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D"); 
		Mesh->SetRelativeScale(1280.f, 720.f);
		Mesh->SetRelativePos(0, 0, 0);
		Mesh->SetRenderType(Mesh, ERenderListSort::None);
		auto Tex = AssetMng->FindTexture("Background");
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, FVector4(0.35, 0.35, 0.35, 0));
	}
	auto MapLine = BoundsObj.lock();
	if (MapLine)
	{
		auto LineTop = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineTop->SetCollisionProfile("Wall");
		LineTop->SetLineDistance(1280.f);
		// 가로
		LineTop->AddWorldRotationZ(-90);
		LineTop->SetWorldPos(-640,360);
		LineTop->SetInheritScale(false);
		LineTop->SetEnable(true);
		LineTop->SetDebugDraw(true);

		auto LineBottom = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineBottom->SetCollisionProfile("Wall");
		LineBottom->SetLineDistance(1280.f);
		// 가로
		LineBottom->AddWorldRotationZ(-90);
		LineBottom->SetWorldPos(-640, -360);
		LineBottom->SetInheritScale(false);
		LineBottom->SetEnable(true);
		LineBottom->SetDebugDraw(true);

		auto LineLeft = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineLeft->SetCollisionProfile("Wall");
		LineLeft->SetLineDistance(720.f);
		// 가로
		LineLeft->AddWorldRotationZ(180);
		LineLeft->SetWorldPos(-640, 360);
		LineLeft->SetInheritScale(false);
		LineLeft->SetEnable(true);
		LineLeft->SetDebugDraw(true);

		auto LineRight = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineRight->SetCollisionProfile("Wall");
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
	std::weak_ptr<CPlayer>	Player = CreateGameObject<CPlayer>("Player");

	return true;
}

void CMainWorld::LoadAnimation2D()
{
	// LoadTexture
	mWorldAssetManager->LoadTextureFullPath("Arrow", L"../Binary/Asset/Texture/Player/Item/Arrow.png");
	mWorldAssetManager->LoadTextureFullPath("Background", L"../Binary/Asset/Texture/Background/Background.png");
	mWorldAssetManager->LoadTextureFullPath("ArrowForce", L"../Binary/Asset/Texture/Item/ArrowForce.png");
	mWorldAssetManager->LoadTextureFullPath("HpPotion", L"../Binary/Asset/Texture/Item/HpPotion.png");
	mWorldAssetManager->LoadTextureFullPath("SpeedUp", L"../Binary/Asset/Texture/Item/SpeedUp.png");
	
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

	// CreateDeadAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerShield", "PlayerShield", 0, 17, "Player/Shield/",
		"0_Forest_Ranger_Shield_", "png", 900, 900);
	// CreateClearAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerClear", "PlayerClear", 1, 29, "Player/Clear/",
		"Player_Clear_frame_", "png", 900, 900);

	// CreateGoblinRunAnim
	if (!mWorldAssetManager->CreateFrameAnimation(
		"GoblinRun", "GoblinRun", 0, 11, "Monster/Goblin/Running/",
		"0_Goblin_Running_", "png", 900, 900))
		return;
	// CreateGoblinAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"GoblinAttack", "GoblinAttack", 0, 11, "Monster/Goblin/Attack/",
		"0_Goblin_Attack_", "png", 900, 900);
	// CreateGoblinSturnAnim
	mWorldAssetManager->CreateFrameAnimation(
		"GoblinSturn", "GoblinSturn", 0, 11, "Monster/Goblin/Sturn/",
		"0_Goblin_Sturn_", "png", 900, 900);


	// CreateOgreRunAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OgreRun", "OgreRun", 0, 11, "Monster/Ogre/Running/",
		"0_Ogre_Running_", "png", 900, 900);
	// CreateOgreAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OgreAttack", "OgreAttack", 0, 11, "Monster/Ogre/Attack/",
		"0_Ogre_Slashing_", "png", 900, 900);
	// CreateOgreSturnAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OgreSturn", "OgreSturn", 0, 11, "Monster/Ogre/Sturn/",
		"0_Ogre_Sturn_", "png", 900, 900);


	// CreateOrcRunAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OrcRun", "OrcRun", 0, 11, "Monster/Orc/Running/",
		"0_Orc_Running_", "png", 900, 900);
	// CreateOrcAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"OrcAttack", "OrcAttack", 0, 11, "Monster/Orc/Attack/",
		"0_Orc_Slashing_", "png", 900, 900);

	// CreateOrcFireBallAnim
	mWorldAssetManager->CreateFrameAnimation(
		"FireBall", "FireBall", 0, 7, "Monster/Orc/Attack/",
		"FireBall_Frame_", "png", 640, 360);

	// CreateOrcFireEffectBallAnim
	mWorldAssetManager->CreateFrameAnimation(
		"FireBallEffect", "FireBallEffect", 0, 25, "Effect/Fireball/",
		"FireBall_Frame_Effect_", "png", 547, 483);

	// CreateBallockDeadAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockDead", "BallockDead", 1, 4, "Monster/Ballock/Dead/",
		"Ballock_Dead_frame_", "png", 240, 162);

	// CreateBallockHitAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockHit", "BallockHit", 1, 30, "Monster/Ballock/Hit/",
		"Ballock_Hit_frame_", "png", 172, 160);

	// CreateBallockMoveAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockMove", "BallockMove", 1, 15, "Monster/Ballock/Move/",
		"Ballock_Move_frame_", "png", 185, 170);

	// CreateBallockSlashAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockSlash", "BallockSlash", 1, 10, "Monster/Ballock/Slash/",
		"Ballock_Slash_frame_", "png", 203, 171);

	// CreateBallockThrowAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockThrow", "BallockThrow", 1, 10, "Monster/Ballock/Throw/",
		"Ballock_Throw_frame_", "png", 201, 171);

	// CreateBallockSturnAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockSturn", "BallockSturn", 1, 10, "Monster/Ballock/Sturn/",
		"Ballock_Sturn_frame_", "png", 172, 160);
	// CreateBallockSkillAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockSkill", "BallockSkill", 1, 45, "Monster/Ballock/Skill/",
		"Ballock_Skill_frame_", "png", 600,540);
	// CreateBallockBressAnim
	mWorldAssetManager->CreateFrameAnimation(
		"BallockBress", "BallockBress", 1, 12, "Monster/Ballock/Bress/",
		"Ballock_Bress_frame_", "png", 600, 540);

	//Effect
	mWorldAssetManager->CreateFrameAnimation(
		"Explosion", "Explosion",1,89, "Effect/Explosion/",
		"Explosion", "png", 320.f, 240.f,false);	
	mWorldAssetManager->CreateFrameAnimation(
		"Spark", "Spark", 0, 10, "Effect/Spark/",
		"Spark_Frame_Effect_", "png", 1280, 720);
	mWorldAssetManager->CreateFrameAnimation(
		"BallockEffect", "BallockEffect", 1, 8, "Effect/BallockEffect/",
		"Ballock_Skill_Effect_frame_", "png", 114, 111);
	mWorldAssetManager->CreateFrameAnimation(
		"BressEffect", "BressEffect", 1, 50, "Effect/BressEffect/",
		"Bress_Effect_frame_", "png", 500, 500);


	
}
	