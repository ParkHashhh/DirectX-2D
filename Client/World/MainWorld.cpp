#include "MainWorld.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterSpawnPoint.h"
#include "../Monster/ScareCrow/ScareCrow.h"
#include "Asset/AssetManager.h"
#include "Render/RenderManager.h"
#include "Asset/Animation2D/Animation2DManager.h"
#include "Component/MeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderLine2D.h"
#include "../Portal/Portal.h"
#include "../Monster/Zakum/BossGround.h"
#include "../UI/NpcWidget.h"
#include "../Npc/Npc.h"

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
	LoadSound();
	std::weak_ptr<CGameObject> BoundsObj = CreateGameObject<CGameObject>("ScreenBounds");

	auto BackObj = CreateGameObject<CGameObject>("Background").lock();
	auto Mesh = BackObj->CreateComponent<CMeshComponent>("BackMesh").lock();
	auto mWorldAssetManager = GetWorldAssetManager().lock();
	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D"); 
		Mesh->SetRelativeScale(1440, 991);
		Mesh->SetRenderLayer("Map");
		Mesh->SetRelativePos(80, -80, 0);
		auto Tex = mWorldAssetManager->FindTexture("Background");
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, FVector4(1, 1, 1, 1));
	}


	auto MapLine = BoundsObj.lock();
	MapLine->SetWorldPos(0, 0, 0);
	MapLine->SetInheritScale(false);
	MapLine->SetInheritRot(false);
	MapLine->SetName("Wall");
	if (MapLine)
	{
		auto LineTop = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineTop->SetCollisionProfile("Wall");
		LineTop->SetLineDistance(1280.f);
		LineTop->AddWorldRotationZ(-90);
		LineTop->SetWorldPos(-640,360);
		LineTop->SetInheritScale(false);
		LineTop->SetDebugDraw(false);
		LineTop->SetStatic(true);


		auto LineLeft = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineLeft->SetCollisionProfile("Wall");
		LineLeft->SetLineDistance(720.f);
		LineLeft->AddWorldRotationZ(0);
		LineLeft->SetWorldPos(-640, -360);
		LineLeft->SetInheritScale(false);
		LineLeft->SetDebugDraw(false);


		auto LineRight = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineRight->SetCollisionProfile("Wall");
		LineRight->SetLineDistance(720.f);
		LineRight->AddWorldRotationZ(0);
		LineRight->SetWorldPos(790, -360);
		LineRight->SetInheritScale(false);
		LineRight->SetDebugDraw(false);

		auto CheckGravityBlock = MapLine->CreateComponent<CColliderBox2D>("Box2D").lock();
		CheckGravityBlock->SetCollisionProfile("Gravity");
		CheckGravityBlock->SetName("Bottom");
		CheckGravityBlock->AddWorldPos(110, -460);
		CheckGravityBlock->SetBoxSize(1500, 200);
		CheckGravityBlock->SetWorldRotationZ(180);
		CheckGravityBlock->SetDebugDraw(false);


		CheckGravityBlock = MapLine->CreateComponent<CColliderBox2D>("Box2D1").lock();
		CheckGravityBlock->SetCollisionProfile("Gravity");
		CheckGravityBlock->SetName("Bottom");
		CheckGravityBlock->SetWorldPos(235, -325);
		CheckGravityBlock->SetBoxSize(900, 65);
		CheckGravityBlock->AddWorldRotationZ(180);
		CheckGravityBlock->SetDebugDraw(false);
		CheckGravityBlock->SetStatic(true);

		CheckGravityBlock = MapLine->CreateComponent<CColliderBox2D>("Box2D2").lock();
		CheckGravityBlock->SetCollisionProfile("Gravity");
		CheckGravityBlock->SetName("Bottom");
		CheckGravityBlock->SetWorldPos(330, -265);
		CheckGravityBlock->SetBoxSize(900, 65);
		CheckGravityBlock->AddWorldRotationZ(180);
		CheckGravityBlock->SetDebugDraw(false);
		CheckGravityBlock->SetStatic(true);


		CheckGravityBlock = MapLine->CreateComponent<CColliderBox2D>("Box2D3").lock();
		CheckGravityBlock->SetCollisionProfile("Gravity");
		CheckGravityBlock->SetWorldPos(-145, -75);
		CheckGravityBlock->SetBoxSize(140, 40);
		CheckGravityBlock->AddWorldRotationZ(180);
		CheckGravityBlock->SetDebugDraw(false);
	}

	std::weak_ptr<CPlayer>	Player = CreateGameObject<CPlayer>("Player");
	Player.lock()->SetZakumEquip();



	std::weak_ptr<CScareCrow>	Monster = CreateGameObject<CScareCrow>("Monster");
	Monster.lock()->SetWorldPos(200,-180,0);
	std::weak_ptr<CPortal> Portal = CreateGameObject<CPortal>("Portal");
	auto _Portal = Portal.lock();
	std::weak_ptr<CNpc>	Npc = CreateGameObject<CNpc>("Npc");
	auto _Npc = Npc.lock();
	if (_Npc)
	{
		_Npc->SetWorldPos(-430.f, -200.f);
	}
	if (_Portal)
	{
		_Portal->SetName("Portal");
		_Portal->SetWorldPos(630, -180, 0);
		_Portal->SetLoadType(EWorldType::Zakum);
	}


	return true;
}


void CMainWorld::LoadSound()
{
	mWorldAssetManager->LoadSound("FloralLife", "BGM", true,
		"FloralLife.mp3");

	mWorldAssetManager->SoundPlay("FloralLife");
	mWorldAssetManager->LoadSound("MP", "Effect", false,
		"MP.mp3");
	mWorldAssetManager->LoadSound("Jump", "Effect", false,
		"Jump.mp3");
	mWorldAssetManager->LoadSound("Heist", "Effect", false,
		"Heist.mp3");
	mWorldAssetManager->LoadSound("Shadow", "Effect", false,
		"Shadow.mp3");
	mWorldAssetManager->LoadSound("Hit", "Effect", false,
		"Hit.mp3");
	mWorldAssetManager->LoadSound("Boost", "Effect", false,
		"Boost.mp3");
	mWorldAssetManager->LoadSound("Will", "Effect", false,
		"Will.mp3");
	mWorldAssetManager->LoadSound("Avenger", "Effect", false,
		"Avenger.mp3");
	mWorldAssetManager->LoadSound("HP", "Effect", false,
		"HP.mp3");
	mWorldAssetManager->LoadSound("MP", "Effect", false,
		"MP.mp3");
	mWorldAssetManager->LoadSound("Portal", "Effect", false,
		"Portal.mp3");
	mWorldAssetManager->LoadSound("TripleThrow", "Effect", false,
		"TripleThrow.mp3");

	mWorldAssetManager->LoadSound("FlashJump", "Effect", false,
		"FlashJump.mp3");
}

void CMainWorld::LoadAnimation2D()
{
	// LoadTexture
	mWorldAssetManager->LoadTextureFullPath("Background", L"../Binary/Asset/Texture/Background/MainBackground.png");
	mWorldAssetManager->LoadTextureFullPath("Hit", L"../Binary/Asset/Texture/Damage/Hit.png");
	mWorldAssetManager->LoadTextureFullPath("BossBackground", L"../Binary/Asset/Texture/Background/ZakumBackground.png");
	mWorldAssetManager->LoadTextureFullPath("HorntailBackground", L"../Binary/Asset/Texture/Background/HornTailBackground.png");
	mWorldAssetManager->LoadTextureFullPath("Miss", L"../Binary/Asset/Texture/Damage/Miss.png");
	mWorldAssetManager->LoadTextureFullPath("Dew", L"../Binary/Asset/Texture/Slot/Dew.png");
	mWorldAssetManager->LoadTextureFullPath("Milk", L"../Binary/Asset/Texture/Slot/Milk.png");
	mWorldAssetManager->LoadTextureFullPath("Elixir", L"../Binary/Asset/Texture/Slot/Elixir.png");
	mWorldAssetManager->LoadTextureFullPath("PowerElixir", L"../Binary/Asset/Texture/Slot/PowerElixir.png");
	mWorldAssetManager->LoadTextureFullPath("ZakumHead", L"../Binary/Asset/Texture/Equip/ZakumHead.png");
	mWorldAssetManager->LoadTextureFullPath("ZakumEtc", L"../Binary/Asset/Texture/Slot/ZakumEtc.png");
	mWorldAssetManager->LoadTextureFullPath("ZakumHork", L"../Binary/Asset/Texture/Equip/ZakumHork.png");
	mWorldAssetManager->LoadTextureFullPath("ZakumArrow", L"../Binary/Asset/Texture/Equip/ZakumArrow.png");
	mWorldAssetManager->LoadTextureFullPath("ZakumWand", L"../Binary/Asset/Texture/Equip/ZakumWand.png");
	mWorldAssetManager->LoadTextureFullPath("Brownbamboo", L"../Binary/Asset/Texture/Equip/Brownbamboo.png");
	mWorldAssetManager->LoadTextureFullPath("BlueGown", L"../Binary/Asset/Texture/Equip/BlueGown.png");
	mWorldAssetManager->LoadTextureFullPath("BlackIgen", L"../Binary/Asset/Texture/Equip/BlackIgen.png");
	mWorldAssetManager->LoadTextureFullPath("MokGlove", L"../Binary/Asset/Texture/Equip/MokGlove.png");
	mWorldAssetManager->LoadTextureFullPath("ShabbyCloak", L"../Binary/Asset/Texture/Equip/ShabbyCloak.png");
	mWorldAssetManager->LoadTextureFullPath("MapleKandeo", L"../Binary/Asset/Texture/Equip/MapleKandeo.png");

	// Item
	mWorldAssetManager->LoadTextureFullPath("HornTailNecklace", L"../Binary/Asset/Texture/Equip/HornTailNecklace.png");
	mWorldAssetManager->LoadTextureFullPath("HornTailEgg", L"../Binary/Asset/Texture/Slot/HornTailEgg.png");
	mWorldAssetManager->LoadTextureFullPath("DragonSlave", L"../Binary/Asset/Texture/Equip/DragonSlave.png");
	mWorldAssetManager->LoadTextureFullPath("DragonStaff", L"../Binary/Asset/Texture/Equip/DragonStaff.png");
	mWorldAssetManager->LoadTextureFullPath("DragonSword", L"../Binary/Asset/Texture/Equip/DragonSword.png");


	mWorldAssetManager->LoadTextureFullPath("ZakumHead", L"../Binary/Asset/Texture/Equip/ZakumHead.png");
	mWorldAssetManager->LoadTextureFullPath("DarkPirate", L"../Binary/Asset/Texture/Equip/DarkPirate.png");
	mWorldAssetManager->LoadTextureFullPath("DarkPirateBottom", L"../Binary/Asset/Texture/Equip/DarkPirateBottom.png");
	mWorldAssetManager->LoadTextureFullPath("DarkGatina", L"../Binary/Asset/Texture/Equip/DarkGatina.png");
	mWorldAssetManager->LoadTextureFullPath("IcarusCloak", L"../Binary/Asset/Texture/Equip/IcarusCloak.png");
	mWorldAssetManager->LoadTextureFullPath("Crackedglasses", L"../Binary/Asset/Texture/Equip/Crackedglasses.png");
	mWorldAssetManager->LoadTextureFullPath("BloodRover", L"../Binary/Asset/Texture/Equip/BloodRover.png");
	mWorldAssetManager->LoadTextureFullPath("MapleScanda", L"../Binary/Asset/Texture/Equip/MapleScanda.png");
	
	// CreateShurikenAnim
	mWorldAssetManager->CreateFrameAnimation(
		"Shuriken", "Shuriken", 1, 16, "Player/Item/",
		"Shuriken_", "png", 48, 9);


	// CreateHitEffectAnim
	mWorldAssetManager->CreateFrameAnimation(
		"HitEffect", "HitEffect", 1, 4, "Effect/HitEffect/",
		"HitEffect_", "png", 320, 240);
	// CreateTripleThrowEffectAnim
	mWorldAssetManager->CreateFrameAnimation(
		"TripleThrowEffect", "TripleThrowEffect", 1, 6, "Effect/TripleThrowEffect/",
		"TripleThrowEffect_", "png", 320, 240);

	// CreateSturnDebuffAnim
	mWorldAssetManager->CreateFrameAnimation(
		"SturnDebuff", "SturnDebuff", 1, 3, "Effect/SturnDebuff/",
		"SturnDebuff_", "png", 66, 18);

	// CreateDarkDebuffAnim
	mWorldAssetManager->CreateFrameAnimation(
		"DarkDebuff", "DarkDebuff", 1, 7, "Effect/DarkDebuff/",
		"DarkDebuff_", "png", 48, 55);

	mWorldAssetManager->CreateFrameAnimation(
		"WillEffect", "WillEffect", 1, 23, "Effect/WillEffect/",
		"WillEffect_", "png", 250, 422);
	// CreateAvengerAnim
	mWorldAssetManager->CreateFrameAnimation(
		"Avenger", "Avenger", 1, 12, "Player/Item/",
		"Avenger_", "png", 121,72);

	// CreatePlayerIdleAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerIdle", "PlayerIdle",1, 3, "Player/Idle/",
		"Idle_", "png", 64, 69);

	// CreatePlayerWalkAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerWalk", "PlayerWalk", 1,12, "Player/Walk/",
		"Walk_", "png", 64, 69);

	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack", "PlayerAttack", 1,5	, "Player/Attack/",
		"Attack_", "png", 77, 69);
	// CreatePlayerAttack1Anim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack1", "PlayerAttack1", 1, 5, "Player/Attack/",
		"Attack1_", "png", 87, 65);
	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack2", "PlayerAttack2", 1, 5, "Player/Attack/",
		"Attack2_", "png", 92, 68);
	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack3", "PlayerAttack3", 1, 5, "Player/Attack/",
		"Attack3_", "png", 87, 65);

	// CreatePlayerJumpAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerJump", "PlayerJump", 1,1, "Player/Jump/",
		"Jump_", "png", 64, 67);

	// CreateDownAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerDown", "PlayerDown", 1, 1, "Player/Down/",
		"Down_", "png", 72, 42);

	// CreatePlayerDeadAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerDead", "PlayerDead", 1, 1, "Player/Dead/",
		"Dead_", "png", 64, 66);

	// CreatePlayerHitAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerHit", "PlayerHit", 1, 3, "Player/Hit/",
		"Hit_", "png", 64, 68);
	// CreatePlayerNearAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerNearAttack", "PlayerNearAttack", 1, 80, "Player/NearAttack/",
		"NearAttack_", "png", 83, 66);




	// CreateSturnDebuffAnim
	mWorldAssetManager->CreateFrameAnimation(
		"SturnDebuff", "SturnDebuff", 1, 3, "Effect/SturnDebuff/",
		"SturnDebuff_", "png", 66, 18);

	// CreateDarkDebuffAnim
	mWorldAssetManager->CreateFrameAnimation(
		"DarkDebuff", "DarkDebuff", 1, 7, "Effect/DarkDebuff/",
		"DarkDebuff_", "png", 48, 55);

	// CreateShadowIdleAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowIdle", "ShadowIdle", 1, 3, "Shadow/Idle/",
		"Shadow_Idle_", "png", 49, 69);

	// CreateShadowAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowAttack", "ShadowAttack", 1, 5, "Shadow/Attack/",
		"Shadow_Attack_", "png", 62, 69);

	// CreateShadowAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowAttack1", "ShadowAttack1", 1, 5, "Shadow/Attack/",
		"Shadow_Attack1_", "png", 78, 65);

	// CreateShadowAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowAttack2", "ShadowAttack2", 1, 5, "Shadow/Attack/",
		"Shadow_Attack2_", "png", 78, 68);

	// CreateShadowAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowAttack3", "ShadowAttack3", 1, 5, "Shadow/Attack/",
		"Shadow_Attack3_", "png", 78, 65);


	// CreateShadowWalkAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowWalk", "ShadowWalk", 1, 4, "Shadow/Walk/",
		"Shadow_Walk_", "png", 49, 69);

	// CreateShadowJumpAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowJump", "ShadowJump", 1, 1, "Shadow/Jump/",
		"Shadow_Jump_", "png", 49, 63);
	// CreateShadowDownAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ShadowDown", "ShadowDown", 1, 1, "Shadow/Down/",
		"Shadow_Down_", "png", 65, 37);


	// CreateScareCrowStandAnim
	mWorldAssetManager->CreateFrameAnimation("ScareCrowStand", "ScareCrowStand",
		1, 13, "Monster/ScareCrow/Stand/",
		"ScareCrow_Stand_", "png", 81, 86);
	// CreateScareCrowStandAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ScareCrowHit", "ScareCrowHit", 1, 1, "Monster/ScareCrow/Hit/",
		"ScareCrow_Hit_", "png", 82, 90);

	mWorldAssetManager->CreateFrameAnimation(
		"JumpEffect", "JumpEffect", 1, 6, "Effect/JumpEffect/",
		"JumpEffect_", "png", 167, 129);

	mWorldAssetManager->CreateFrameAnimation(
		"ShadowEffect", "ShadowEffect", 1, 16, "Effect/ShadowEffect/",
		"ShadowEffect_", "png", 195, 180);

	mWorldAssetManager->CreateFrameAnimation(
		"ShadowEffect2", "ShadowEffect2", 1, 31, "Effect/ShadowEffect2/",
		"ShadowEffect2_", "png", 158, 131);

	mWorldAssetManager->CreateFrameAnimation(
		"HeistEffect", "HeistEffect", 1, 9, "Effect/HeistEffect/",
		"HeistEffect_", "png", 122, 64);

	mWorldAssetManager->CreateFrameAnimation(
		"BoostEffect", "BoostEffect", 1, 9, "Effect/BoostEffect/",
		"BoostEffect_", "png", 79, 34);

	mWorldAssetManager->CreateFrameAnimation(
		"Portal", "Portal", 1, 8, "Effect/Portal/",
		"Portal_", "png", 104, 142);
	
}
	