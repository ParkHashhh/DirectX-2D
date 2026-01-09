#include "MainWorld.h"
#include "../Player/Player.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterSpawnPoint.h"
#include "Asset/AssetManager.h"
#include "Asset/Animation2D/Animation2DManager.h"

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

	std::weak_ptr<CMonster>	Monster1 = CreateGameObject<CMonster>("Monster");

	std::shared_ptr<CMonster>	Monster = Monster1.lock();

	if (Monster)
	{
		Monster->SetWorldPos(-400.f, 300.f);
		Monster->SetWorldRotationZ(180);
	}
	std::weak_ptr<CMonsterSpawnPoint>	SpawnPoint1 = CreateGameObject<CMonsterSpawnPoint>("SpawnPoint");

	std::shared_ptr<CMonsterSpawnPoint>	Point = SpawnPoint1.lock();
	if (Point)
	{
		Point->SetSpawnClass<CMonster>();
		Point->SetSpawnTime(2.f);
	}
	/*Monster1 = CreateGameObject<CMonster>("Monster");

	Monster = Monster1.lock();

	if (Monster)
	{
		Monster->SetWorldPos(400.f, 300.f);
		Monster->SetWorldRotationZ(180.f);
	}

	std::weak_ptr<CMonsterSpawnPoint>	SpawnPoint1 = CreateGameObject<CMonsterSpawnPoint>("SpawnPoint");

	std::shared_ptr<CMonsterSpawnPoint>	Point = SpawnPoint1.lock();

	if (Point)
	{
		Point->SetWorldPos(-400.f, -300.f);
		Point->SetWorldRotationZ(20.f);
		Point->SetSpawnClass<CMonster>();
		Point->SetSpawnTime(5.f);
	}*/

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

	mWorldAssetManager->CreateFrameAnimation(
		"PlayerRunShoot", "PlayerRunShoot", 0,11, "Player/Run_Shooting/",
		"0_Forest_Ranger_Run_Shooting_", "png", 900, 900);

	mWorldAssetManager->CreateFrameAnimation(
		"PlayerSlide", "PlayerSlide", 0, 5, "Player/Sliding/",
		"0_Forest_Ranger_Sliding_", "png", 900, 900);

	mWorldAssetManager->CreateAnimation("MonsterIdle");
	mWorldAssetManager->SetAnimation2DTextureType("MonsterIdle",
		EAnimation2DTextureType::SpriteSheet);

	mWorldAssetManager->SetTexture("MonsterIdle", "MonsterSheet",
		TEXT("Monster.png"));

	for (int i = 0; i < 14; ++i)
	{
		mWorldAssetManager->AddFrame("MonsterIdle", i * 45.f, 60.f,
			45.f, 60.f);
	}


	mWorldAssetManager->CreateAnimation("MonsterAttack");
	mWorldAssetManager->SetAnimation2DTextureType("MonsterAttack",
		EAnimation2DTextureType::SpriteSheet);

	mWorldAssetManager->SetTexture("MonsterAttack", "MonsterSheet",
		TEXT("Monster.png"));

	for (int i = 0; i < 21; ++i)
	{
		mWorldAssetManager->AddFrame("MonsterAttack", i * 45.f,
			180.f, 45.f, 60.f);
	}

	mWorldAssetManager->CreateFrameAnimation(
		"Explosion", "Explosion",1,89, "Explosion/",
		"Explosion", "png", 320.f, 240.f,false);


	//mWorldAssetManager->CreateAnimation("Explosion");
	//mWorldAssetManager->SetAnimation2DTextureType("Explosion",
	//	EAnimation2DTextureType::Frame);

	//for (int i = 1; i <= 89; ++i)
	//{
	//	//TCHAR	FileName[MAX_PATH] = {};
	//	TCHAR* FileName = new TCHAR[MAX_PATH];
	//	memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
	//	wsprintf(FileName,
	//		TEXT("Explosion/Explosion%d.png"), i);
	//	TextureFileName.push_back(FileName);
	//}

	//mWorldAssetManager->SetTexture("Explosion", "Explosion",
	//	TextureFileName);

	//for (int i = 0; i < 89; ++i)
	//{
	//	delete[] TextureFileName[i];
	//}
	//TextureFileName.clear();

	//mWorldAssetManager->AddFrame("Explosion", 89, 0.f, 0.f, 320.f, 240.f);
}
