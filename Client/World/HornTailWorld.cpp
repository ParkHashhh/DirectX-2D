#include "HornTailWorld.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterSpawnPoint.h"
#include "../Monster/HornTail/HornTail.h"
#include "../Monster/HornTail/LeftHead.h"
#include "Asset/AssetManager.h"
#include "Render/RenderManager.h"
#include "Asset/Animation2D/Animation2DManager.h"
#include "Component/MeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderLine2D.h"
#include "../Portal/Portal.h"

CHornTailWorld::CHornTailWorld()
{
}

CHornTailWorld::~CHornTailWorld()
{
}

bool CHornTailWorld::Init()
{
	CWorld::Init();
	LoadAnimation2D();
	LoadSound();

	CreateMapCollider();
	CreateMapFootHold();
	std::weak_ptr<CPlayer>	Player = CreateGameObject<CPlayer>("Player");
	Player.lock()->SetHorntailEquip();
	Player.lock()->SetHornTailWorld(true);
	std::weak_ptr<CHornTail>	HornTail = CreateGameObject<CHornTail>("HornTailssss");
	auto _HornTail = HornTail.lock();
	if (_HornTail)
	{
		_HornTail->SetWorldPos(100, -370);
	}
	return true;
}

void CHornTailWorld::LoadAnimation2D()
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
		"Avenger_", "png", 121, 72);

	// CreatePosionAnim
	mWorldAssetManager->CreateFrameAnimation(
		"Poison", "Poison", 1, 21, "Effect/Poison/",
		"Poison_", "png", 836, 664);

	// CreateSturnDebuffAnim
	mWorldAssetManager->CreateFrameAnimation(
		"SturnDebuff", "SturnDebuff", 1, 3, "Effect/SturnDebuff/",
		"SturnDebuff_", "png", 66, 18);

	// CreatePlayerIdleAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerIdle", "PlayerIdle", 1, 15, "Player2/Idle/",
		"Idle_", "png", 101, 109);

	// CreatePlayerWalkAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerWalk", "PlayerWalk", 1, 12, "Player2/Walk/",
		"Walk_", "png", 101, 109);

	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack", "PlayerAttack", 1, 9, "Player2/Attack/",
		"Attack_", "png", 101, 109);

	// CreatePlayerAttack1Anim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack1", "PlayerAttack1", 1, 9, "Player2/Attack/",
		"Attack1_", "png", 110, 109);

	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack2", "PlayerAttack2", 1, 9, "Player2/Attack/",
		"Attack2_", "png", 113, 109);

	// CreatePlayerAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerAttack3", "PlayerAttack3", 1, 9, "Player2/Attack/",
		"Attack3_", "png", 110, 109);

	// CreatePlayerJumpAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerJump", "PlayerJump", 1, 1, "Player2/Jump/",
		"Jump_", "png", 100, 108);

	// CreatePlayerDownAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerDown", "PlayerDown", 1, 1, "Player2/Down/",
		"Down_", "png", 98, 108);

	// CreatePlayerDeadAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerDead", "PlayerDead", 1, 1, "Player2/Dead/",
		"Dead_", "png", 101, 110);

	// CreatePlayerHitAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerHit", "PlayerHit", 1, 15, "Player2/Hit/",
		"Hit_", "png", 101, 109);

	// CreatePlayerNearAttackAnim
	mWorldAssetManager->CreateFrameAnimation(
		"PlayerNearAttack", "PlayerNearAttack", 1, 9, "Player2/NearAttack/",
		"NearAttack_", "png", 102, 109);

	//HornTailTail
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailTail_Idle", "HornTailTail_Idle", 1, 1, "Monster/HornTail/Tail/Idle/",
		"HornTailTail_Idle_", "png", 406, 146);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailTail_Attack", "HornTailTail_Attack", 1, 59, "Monster/HornTail/Tail/Attack/",
		"HornTailTail_Attack_", "png", 435, 223);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailTail_Die", "HornTailTail_Die", 1, 67, "Monster/HornTail/Tail/Die/",
		"HornTailTail_Die_", "png", 427, 146);

	//HornTailLeg
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeg_Idle", "HornTailLeg_Idle", 1, 1, "Monster/HornTail/Leg/Idle/",
		"HornTailLeg_Idle_", "png", 514, 192);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeg_Attack", "HornTailLeg_Attack", 1, 17, "Monster/HornTail/Leg/Attack/",
		"HornTailLeg_Attack1_", "png", 590, 257);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeg_Die", "HornTailLeg_Die", 1, 173, "Monster/HornTail/Leg/Die/",
		"HornTailLeg_Die_", "png", 514, 197);

	//HornTailLeftHand
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHand_Idle", "HornTailLeftHand_Idle", 1, 36, "Monster/HornTail/LeftHand/Idle/",
		"HornTailLeftHand_Idle_", "png", 215, 324);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHand_Attack", "HornTailLeftHand_Attack", 1, 112, "Monster/HornTail/LeftHand/Attack/",
		"HornTailLeftHand_Attack_", "png", 316, 368);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHand_Die", "HornTailLeftHand_Die", 1, 200, "Monster/HornTail/LeftHand/Die/",
		"HornTailLeftHand_Die_", "png", 210, 321);


	//HornTailRightHand
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailRightHand_Idle", "HornTailRightHand_Idle", 1, 42, "Monster/HornTail/RightHand/Idle/",
		"HornTailRightHand_Idle_", "png", 215, 324);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailRightHand_Attack", "HornTailRightHand_Attack", 1, 112, "Monster/HornTail/RightHand/Attack/",
		"HornTailRightHand_Attack_", "png", 302, 395);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailRightHand_Die", "HornTailRightHand_Die", 1, 200, "Monster/HornTail/RightHand/Die/",
		"HornTailRightHand_Die_", "png", 210, 321);


	//HornTailLeftHead
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHead_Idle", "HornTailLeftHead_Idle", 1, 40, "Monster/HornTail/LeftHead/Idle/",
		"HornTailLeftHead_Idle_", "png", 261, 505);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHead_Attack", "HornTailLeftHead_Attack", 1, 17, "Monster/HornTail/LeftHead/Attack/",
		"HornTailLeftHead_Attack_", "png", 353, 505);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHead_Die", "HornTailLeftHead_Die", 1, 182, "Monster/HornTail/LeftHead/Die/",
		"HornTailLeftHead_Die_", "png", 253, 505);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailLeftHead_Object", "HornTailLeftHead_Object", 1, 34, "Monster/HornTail/Object/LeftHead/",
		"HornTailLeftHead_Object_", "png", 216, 242);

	//HornTailMiddleHead
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailMiddleHead_Idle", "HornTailMiddleHead_Idle", 1, 50, "Monster/HornTail/MiddleHead/Idle/",
		"HornTailMiddleHead_Idle_", "png", 161, 628);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailMiddleHead_Attack", "HornTailMiddleHead_Attack", 1, 26, "Monster/HornTail/MiddleHead/Attack/",
		"HornTailMiddleHead_Attack_", "png", 552, 777);

	mWorldAssetManager->CreateFrameAnimation(
		"HornTailMiddleHead_Die", "HornTailMiddleHead_Die", 1, 171, "Monster/HornTail/MiddleHead/Die/",
		"HornTailMiddleHead_Die_", "png", 161, 629);

	//HornTailRightHead
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailRightHead_Idle", "HornTailRightHead_Idle", 1, 60, "Monster/HornTail/RightHead/Idle/",
		"HornTailRightHead_Idle_", "png", 261, 505);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailRightHead_Attack", "HornTailRightHead_Attack", 1, 17, "Monster/HornTail/RightHead/Attack/",
		"HornTailRightHead_Attack_", "png", 349, 505);
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailRightHead_Die", "HornTailRightHead_Die", 1, 181, "Monster/HornTail/RightHead/Die/",
		"HornTailRightHead_Die_", "png", 253, 505);

	// Wyvern
	mWorldAssetManager->CreateFrameAnimation(
		"BlueWyvern_Idle","BlueWyvern_Idle", 1, 6, "Monster/HornTail/Wyvern/Blue/Idle/",
		"BlueWyvern_Idle_", "png", 188, 171);
	mWorldAssetManager->CreateFrameAnimation(
		"BlackWyvern_Idle", "BlackWyvern_Idle", 1,6, "Monster/HornTail/Wyvern/Black/Idle/",
		"BlackWyvern_Idle_", "png", 188, 171);
	mWorldAssetManager->CreateFrameAnimation(
		"RedWyvern_Idle", "RedWyvern_Idle", 1, 6, "Monster/HornTail/Wyvern/Red/Idle/",
		"RedWyvern_Idle_", "png", 188, 171);

	mWorldAssetManager->CreateFrameAnimation(
		"BlueWyvern_Die", "BlueWyvern_Die", 1, 34, "Monster/HornTail/Wyvern/Blue/Die/",
		"BlueWyvern_Die_", "png", 175, 191);
	mWorldAssetManager->CreateFrameAnimation(
		"BlackWyvern_Die", "BlackWyvern_Die", 1, 34, "Monster/HornTail/Wyvern/Black/Die/",
		"BlackWyvern_Die_", "png", 175, 191);
	mWorldAssetManager->CreateFrameAnimation(
		"RedWyvern_Die", "RedWyvern_Die", 1, 34, "Monster/HornTail/Wyvern/Red/Die/",
		"RedWyvern_Die_", "png", 175, 191);

	//HornTailWing
	mWorldAssetManager->CreateFrameAnimation(
		"HornTailWing_Idle", "HornTailWing_Idle", 1, 32, "Monster/HornTail/Wing/Idle/",
		"HornTailWing_Idle_", "png", 590, 364);

	mWorldAssetManager->CreateFrameAnimation(
		"HornTailWing_Attack", "HornTailWing_Attack", 1,18, "Monster/HornTail/Wing/Attack/",
		"HornTailWing_Attack_", "png", 1016, 626);

	mWorldAssetManager->CreateFrameAnimation(
		"HornTailWing_Die", "HornTailWing_Die", 1, 182, "Monster/HornTail/Wing/Die/",
		"HornTailWing_Die_", "png", 574, 364);


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
	mWorldAssetManager->CreateFrameAnimation(
		"Grave", "Grave", 1, 1, "Player/Dead/",
		"Grave_", "png", 40, 43);

}
void CHornTailWorld::LoadSound()
{

	mWorldAssetManager->LoadSound("HornTailBgm", "BGM", true,
		"HornTailBgm.mp3");
	mWorldAssetManager->SoundPlay("HornTailBgm");

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

void CHornTailWorld::CreateUI()
{
}

void CHornTailWorld::CreateMapCollider()
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
		auto Tex = AssetMng->FindTexture("HorntailBackground");
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
		LineTop->SetLineDistance(1500.f);
		LineTop->AddWorldRotationZ(-90);
		LineTop->SetWorldPos(-640, 360);
		LineTop->SetInheritScale(false);
		LineTop->SetDebugDraw(false);
		LineTop->SetStatic(true);


		auto LineLeft = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineLeft->SetCollisionProfile("Wall");
		LineLeft->SetLineDistance(920.f);
		LineLeft->AddWorldRotationZ(0);
		LineLeft->SetWorldPos(-640, -510);
		LineLeft->SetInheritScale(false);
		LineLeft->SetDebugDraw(false);


		auto LineRight = MapLine->CreateComponent<CColliderLine2D>("Line2D").lock();
		LineRight->SetCollisionProfile("Wall");
		LineRight->SetLineDistance(920.f);
		LineRight->AddWorldRotationZ(0);
		LineRight->SetWorldPos(840, -510);
		LineRight->SetInheritScale(false);
		LineRight->SetDebugDraw(false);

		auto CheckGravityBlock = MapLine->CreateComponent<CColliderBox2D>("Box2D").lock();
		CheckGravityBlock->SetCollisionProfile("Gravity");
		CheckGravityBlock->SetName("Bottom");
		CheckGravityBlock->SetWorldPos(100, -585);
		CheckGravityBlock->SetBoxSize(1480, 200);
		CheckGravityBlock->AddWorldRotationZ(180);
		CheckGravityBlock->SetDebugDraw(false);
		CheckGravityBlock->SetStatic(true);
	}

}

void CHornTailWorld::CreateMapFootHold()
{
	std::weak_ptr<CGameObject> BoundsObj = CreateGameObject<CGameObject>("FootHold");

	auto Bound = BoundsObj.lock();
	if (Bound)
	{
		Bound->SetWorldPos(0.f, 0.f, 0.f);

		Bound->SetInheritScale(false);
		Bound->SetInheritRot(false);

		//Left

		auto FootHold1 = Bound->CreateComponent<CColliderBox2D>("FootHold1").lock();
		FootHold1->SetCollisionProfile("Gravity");
		FootHold1->SetName("FootHold1");
		FootHold1->SetWorldPos(-130, 90);
		FootHold1->SetBoxSize(60, 10);
		FootHold1->SetDebugDraw(false);
		FootHold1->SetStatic(true);

		auto FootHold2 = Bound->CreateComponent<CColliderBox2D>("FootHold2").lock();
		FootHold2->SetCollisionProfile("Gravity");
		FootHold2->SetName("FootHold2");

		FootHold2->SetWorldPos(-222, 30);
		FootHold2->SetBoxSize(70, 10);
		FootHold2->SetDebugDraw(false);
		FootHold2->SetStatic(true);

		auto FootHold3 = Bound->CreateComponent<CColliderBox2D>("FootHold3").lock();
		FootHold3->SetCollisionProfile("Gravity");
		FootHold3->SetName("FootHold3");
		FootHold3->SetWorldPos(-460, -40);
		FootHold3->SetBoxSize(340, 10);
		FootHold3->SetDebugDraw(false);
		FootHold3->SetStatic(true);

		auto FootHold4 = Bound->CreateComponent<CColliderBox2D>("FootHold4").lock();
		FootHold4->SetCollisionProfile("Gravity");
		FootHold4->SetName("FootHold4");

		FootHold4->SetWorldPos(-510, -235);
		FootHold4->SetBoxSize(240, 10);
		FootHold4->SetDebugDraw(false);
		FootHold4->SetStatic(true);

		auto FootHold5 = Bound->CreateComponent<CColliderBox2D>("FootHold5").lock();
		FootHold5->SetCollisionProfile("Gravity");
		FootHold5->SetName("FootHold5");


		FootHold5->SetWorldPos(-320, -300);
		FootHold5->SetBoxSize(60, 10);
		FootHold5->SetDebugDraw(false);
		FootHold5->SetStatic(true);

		//Right 
		auto FootHold6 = Bound->CreateComponent<CColliderBox2D>("FootHold6").lock();
		FootHold6->SetCollisionProfile("Gravity");
		FootHold6->SetName("FootHold6");

		FootHold6->SetWorldPos(350, 90);
		FootHold6->SetBoxSize(60, 10);
		FootHold6->SetDebugDraw(false);
		FootHold6->SetStatic(true);


		auto FootHold7 = Bound->CreateComponent<CColliderBox2D>("FootHold7").lock();
		FootHold7->SetCollisionProfile("Gravity");
		FootHold7->SetName("FootHold7");

		FootHold7->SetWorldPos(540, 25);
		FootHold7->SetBoxSize(255, 10);
		FootHold7->SetDebugDraw(false);
		FootHold7->SetStatic(true);

		auto FootHold8 = Bound->CreateComponent<CColliderBox2D>("FootHold8").lock();
		FootHold8->SetCollisionProfile("Gravity");
		FootHold8->SetName("FootHold8");

		FootHold8->SetWorldPos(350, -40);
		FootHold8->SetBoxSize(60, 10);
		FootHold8->SetDebugDraw(false);
		FootHold8->SetStatic(true);

		auto FootHold9 = Bound->CreateComponent<CColliderBox2D>("FootHold9").lock();
		FootHold9->SetCollisionProfile("Gravity");
		FootHold9->SetName("FootHold9");

		FootHold9->SetWorldPos(445, -170);
		FootHold9->SetBoxSize(65, 10);
		FootHold9->SetDebugDraw(false);
		FootHold9->SetStatic(true);

		auto FootHold10 = Bound->CreateComponent<CColliderBox2D>("FootHold10").lock();
		FootHold10->SetCollisionProfile("Gravity");
		FootHold10->SetName("FootHold10");

		FootHold10->SetWorldPos(587, -230);
		FootHold10->SetBoxSize(165, 10);
		FootHold10->SetDebugDraw(false);
		FootHold10->SetStatic(true);

		auto FootHold11 = Bound->CreateComponent<CColliderBox2D>("FootHold11").lock();
		FootHold11->SetCollisionProfile("Gravity");
		FootHold11->SetName("FootHold11");

		FootHold11->SetWorldPos(445, -300);
		FootHold11->SetBoxSize(65, 10);
		FootHold11->SetDebugDraw(false);
		FootHold11->SetStatic(true);
	}
}
