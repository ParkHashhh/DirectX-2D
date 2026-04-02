#include "ZakumWorld.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterSpawnPoint.h"
#include "../Monster/ScareCrow/ScareCrow.h"
#include "../Monster/Zakum/Zakum.h"
#include "Asset/AssetManager.h"
#include "Render/RenderManager.h"
#include "Asset/Animation2D/Animation2DManager.h"
#include "Component/MeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderLine2D.h"
#include "../Monster/Zakum/BossGround.h"

CZakumWorld::CZakumWorld()
{
}

CZakumWorld::~CZakumWorld()
{
}

bool CZakumWorld::Init()
{	
	CWorld::Init();
	LoadAnimation2D();
	LoadSound();
	CreateMapCollider();
	CreateMapFootHold();
	std::weak_ptr<CPlayer>	Player = CreateGameObject<CPlayer>("Player");
	Player.lock()->SetZakumEquip();
	Player.lock()->AddFireEye();
	//std::weak_ptr<CZakum>	Zakum = CreateGameObject<CZakum>("Zakum");
	std::weak_ptr<CBossGround> Ground = CreateGameObject<CBossGround>("Ground");
	Ground.lock()->SetWorldPos(10, -390, 0);
	return true;
}

void CZakumWorld::LoadAnimation2D()
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

	// CreateHitEffectAnim
	mWorldAssetManager->CreateFrameAnimation(
		"HitEffect", "HitEffect", 1, 4, "Effect/HitEffect/",
		"HitEffect_", "png", 320, 240);
	// CreateTripleThrowEffectAnim
	mWorldAssetManager->CreateFrameAnimation(
		"TripleThrowEffect", "TripleThrowEffect", 1, 6, "Effect/TripleThrowEffect/",
		"TripleThrowEffect_", "png", 320, 240);
	// CreateShurikenAnim
	mWorldAssetManager->CreateFrameAnimation(
		"Shuriken", "Shuriken", 1, 16, "Player/Item/",
		"Shuriken_", "png", 48, 9);

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
		"Avenger_", "png", 121, 72);

	// CreatePlayerIdleAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerIdle", "PlayerIdle", 1, 3, "Player/Idle/",
		"Idle_", "png", 64, 69);

	// CreatePlayerWalkAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerWalk", "PlayerWalk", 1, 12, "Player/Walk/",
		"Walk_", "png", 64, 69);

	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack", "PlayerAttack", 1, 5, "Player/Attack/",
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
		"PlayerJump", "PlayerJump", 1, 1, "Player/Jump/",
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



	
	// CreateZakumAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumBody_Idle", "ZakumBody_Idle", 1, 8, "Monster/Zakum/Body/Idle/",
		"ZakumBody_Idle_", "png", 385, 388);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumBody_Die", "ZakumBody_Die", 1, 10, "Monster/Zakum/Body/Die/",
		"ZakumBody_Die_", "png", 385, 381);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumBody_Skill", "ZakumBody_Skill", 1, 225, "Monster/Zakum/Body/Skill/",
		"ZakumBody_Skill_", "png", 385, 414);

	// CreateZakumAnim
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm1_Idle", "ZakumBody1_Idle", 1, 67, "Monster/Zakum/Arm/1/Idle/",
		"ZakumArm1_Idle_", "png", 240, 393);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm1_Die", "ZakumArm1_Die", 1, 17, "Monster/Zakum/Arm/1/Die/",
		"ZakumArm1_Die_", "png", 283, 455);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm2_Idle", "ZakumArm2_Idle", 1, 67, "Monster/Zakum/Arm/2/Idle/",
		"ZakumArm2_Idle_", "png", 275, 339);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm2_Die", "ZakumArm2_Die", 1, 17, "Monster/Zakum/Arm/2/Die/",
		"ZakumArm2_Die_", "png", 351, 425);
	
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm3_Idle", "ZakumBody3_Idle", 1, 67, "Monster/Zakum/Arm/3/Idle/",
		"ZakumArm3_Idle_", "png", 299, 285);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm3_Die", "ZakumArm3_Die", 1, 17, "Monster/Zakum/Arm/3/Die/",
		"ZakumArm3_Die_", "png", 388, 398);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm4_Idle", "ZakumArm4_Idle", 1, 67, "Monster/Zakum/Arm/4/Idle/",
		"ZakumArm4_Idle_", "png", 325, 215);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm4_Die", "ZakumArm4_Die", 1, 17, "Monster/Zakum/Arm/4/Die/",
		"ZakumArm4_Die_", "png", 398, 386);

	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm5_Idle", "ZakumBody5_Idle", 1, 67, "Monster/Zakum/Arm/5/Idle/",
		"ZakumArm5_Idle_", "png", 248, 396);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm5_Die", "ZakumArm5_Die", 1, 17, "Monster/Zakum/Arm/5/Die/",
		"ZakumArm5_Die_", "png", 283, 458);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm6_Idle", "ZakumArm6_Idle", 1, 67, "Monster/Zakum/Arm/6/Idle/",
		"ZakumArm6_Idle_", "png", 294, 341);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm6_Die", "ZakumArm6_Die", 1, 17, "Monster/Zakum/Arm/6/Die/",
		"ZakumArm6_Die_", "png", 354, 428);

	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm7_Idle", "ZakumBody7_Idle", 1, 67, "Monster/Zakum/Arm/7/Idle/",
		"ZakumArm7_Idle_", "png", 316, 289);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm7_Die", "ZakumArm7_Die", 1, 17, "Monster/Zakum/Arm/7/Die/",
		"ZakumArm7_Die_", "png", 388, 398);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm8_Idle", "ZakumArm8_Idle", 1, 67, "Monster/Zakum/Arm/8/Idle/",
		"ZakumArm8_Idle_", "png", 339, 214);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm8_Die", "ZakumArm8_Die", 1, 17, "Monster/Zakum/Arm/8/Die/",
		"ZakumArm8_Die_", "png", 398, 368);

	mWorldAssetManager->CreateFrameAnimation(
		"Grave", "Grave", 1,1, "Player/Dead/",
		"Grave_", "png", 40, 43);

	mWorldAssetManager->CreateFrameAnimation(
		"FallingStone", "FallingStone", 69, 101, "Monster/Zakum/Skill/FallingStone/",
		"Stone_", "png", 281, 703);

	mWorldAssetManager->CreateFrameAnimation(
		"DestinationStone", "DestinationStone", 1, 68, "Monster/Zakum/Skill/DestinationStone/",
		"Stone_", "png", 281, 703);
	mWorldAssetManager->CreateFrameAnimation(
		"ZakumArm_Skill", "ZakumArm_Skill", 1, 96, "Monster/Zakum/Skill/ArmSkill/",
		"ZakumArm_Skill_", "png", 640, 444);
}	
void CZakumWorld::LoadSound()
{
	mWorldAssetManager->LoadSound("ZakumBgm", "BGM", true,
		"ZakumBgm.mp3");
	mWorldAssetManager->SoundPlay("ZakumBgm");

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

void CZakumWorld::CreateUI()
{
}

void CZakumWorld::CreateMapCollider()
{

	std::weak_ptr<CGameObject> BoundsObj = CreateGameObject<CGameObject>("ScreenBounds");

	auto BackObj = CreateGameObject<CGameObject>("Background").lock();
	auto Mesh = BackObj->CreateComponent<CMeshComponent>("BackMesh").lock();
	auto AssetMng = GetWorldAssetManager().lock();
	if (Mesh)
	{
		Mesh->SetMesh("CenterRectTex");
		Mesh->SetShader("DefaultTexture2D");
		Mesh->SetRelativeScale(1500.f, 1000.f);
		Mesh->SetRenderLayer("Map");
		Mesh->SetRelativePos(100, -123, 0);
		auto Tex = AssetMng->FindTexture("BossBackground");
		Mesh->SetTexture(0, 0, Tex);
		Mesh->SetMaterialBaseColor(0, FVector4(1, 1, 1, 1));
	}


	auto MapLine = BoundsObj.lock();

	if (MapLine)
	{

		MapLine->SetWorldPos(0, 0, 0);
		MapLine->SetInheritScale(false);
		MapLine->SetInheritRot(false);
		MapLine->SetName("Wall");


		auto LineTop = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineTop->SetCollisionProfile("Wall");
		LineTop->SetLineDistance(1280.f);
		LineTop->AddWorldRotationZ(-90);
		LineTop->SetWorldPos(-640, 360);
		LineTop->SetInheritScale(false);
		LineTop->SetDebugDraw(false);
		LineTop->SetStatic(true);


		auto LineLeft = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineLeft->SetCollisionProfile("Wall");
		LineLeft->SetLineDistance(920.f);
		LineLeft->AddWorldRotationZ(0);
		LineLeft->SetWorldPos(-640, -460);
		LineLeft->SetInheritScale(false);
		LineLeft->SetDebugDraw(false);


		auto LineRight = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineRight->SetCollisionProfile("Wall");
		LineRight->SetLineDistance(920.f);
		LineRight->AddWorldRotationZ(0);
		LineRight->SetWorldPos(840, -460);
		LineRight->SetInheritScale(false);
		LineRight->SetDebugDraw(false);

		auto CheckGravityBlock = MapLine->CreateComponent<CColliderBox2D>("Box2D").lock();
		CheckGravityBlock->SetCollisionProfile("Gravity");
		CheckGravityBlock->SetName("Bottom");
		CheckGravityBlock->SetWorldPos(100, -529);
		CheckGravityBlock->SetBoxSize(1480, 200);
		CheckGravityBlock->AddWorldRotationZ(180);
		CheckGravityBlock->SetDebugDraw(false);
		CheckGravityBlock->SetStatic(true);

	}

}

void CZakumWorld::CreateMapFootHold()
{
	std::weak_ptr<CGameObject> BoundsObj = CreateGameObject<CGameObject>("FootHold");

	auto Bound = BoundsObj.lock();
	if (Bound)
	{
		Bound->SetWorldPos(0.f, 0.f, 0.f);

		Bound->SetInheritScale(false);
		Bound->SetInheritRot(false);

		auto FootHold1 = Bound->CreateComponent<CColliderBox2D>("FootHold1").lock();
		FootHold1->SetCollisionProfile("Gravity");
		FootHold1->SetName("FootHold1");
		FootHold1->SetWorldPos(-400, -310);
		FootHold1->SetBoxSize(280, 30);
		FootHold1->SetDebugDraw(false);
		FootHold1->SetStatic(true);

		auto FootHold2 = Bound->CreateComponent<CColliderBox2D>("FootHold2").lock();
		FootHold2->SetCollisionProfile("Gravity");
		FootHold2->SetName("FootHold2");

		FootHold2->SetWorldPos(-400, -200);
		FootHold2->SetBoxSize(280, 30);
		FootHold2->SetDebugDraw(false);
		FootHold2->SetStatic(true);

		auto FootHold3 = Bound->CreateComponent<CColliderBox2D>("FootHold3").lock();
		FootHold3->SetCollisionProfile("Gravity");
		FootHold3->SetName("FootHold3");
		FootHold3->SetWorldPos(-350, -97);
		FootHold3->SetBoxSize(280, 30);
		FootHold3->SetDebugDraw(false);
		FootHold3->SetStatic(true);

		auto FootHold4 = Bound->CreateComponent<CColliderBox2D>("FootHold4").lock();
		FootHold4->SetCollisionProfile("Gravity");
		FootHold4->SetName("FootHold4");

		FootHold4->SetWorldPos(570, -310);
		FootHold4->SetBoxSize(280, 30);
		FootHold4->SetDebugDraw(false);
		FootHold4->SetStatic(true);

		auto FootHold5 = Bound->CreateComponent<CColliderBox2D>("FootHold5").lock();
		FootHold5->SetCollisionProfile("Gravity");
		FootHold5->SetName("FootHold5");

		FootHold5->SetWorldPos(600, -200);
		FootHold5->SetBoxSize(280, 30);
		FootHold5->SetDebugDraw(false);
		FootHold5->SetStatic(true);

		auto FootHold6 = Bound->CreateComponent<CColliderBox2D>("FootHold6").lock();
		FootHold6->SetCollisionProfile("Gravity");
		FootHold6->SetName("FootHold6");

		FootHold6->SetWorldPos(560, -95);
		FootHold6->SetBoxSize(280, 30);
		FootHold6->SetDebugDraw(false);
		FootHold6->SetStatic(true);
	}
}
