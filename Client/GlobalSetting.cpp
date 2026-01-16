#include "GlobalSetting.h"
#include "CollisionInfoManager.h"

CGlobalSetting::CGlobalSetting()
{
}

CGlobalSetting::~CGlobalSetting()
{
}

bool CGlobalSetting::Init()
{

	CCollisionInfoManager::GetInst()->CreateChannel("Static");
	CCollisionInfoManager::GetInst()->CreateChannel("Player");
	CCollisionInfoManager::GetInst()->CreateChannel("Monster");
	CCollisionInfoManager::GetInst()->CreateChannel("PlayerAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("MonsterAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("PlayerInvincible");
	CCollisionInfoManager::GetInst()->CreateChannel("PlayerShield");
	CCollisionInfoManager::GetInst()->CreateChannel("Wall");
	CCollisionInfoManager::GetInst()->CreateChannel("Item");
	CCollisionInfoManager::GetInst()->CreateChannel("HitBox");

	CCollisionInfoManager::GetInst()->CreateProfile("Static", "Static", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Player", "Player", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Monster", "Monster", true);
	CCollisionInfoManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("PlayerInvincible", "PlayerInvincible", true);
	CCollisionInfoManager::GetInst()->CreateProfile("PlayerShield", "PlayerShield", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Wall", "Wall", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Item", "Item", true);
	CCollisionInfoManager::GetInst()->CreateProfile("HitBox", "HitBox", true);



	// 히트박스
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "Player",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "HitBox",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "PlayerAttack",
		ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "HitBox",
		ECollisionInteraction::Block);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "HitBox",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "MonsterAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "HitBox",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "PlayerShield",
		ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerShield", "HitBox",
		ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "Wall",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "HitBox",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"HitBox", "HitBox",
		ECollisionInteraction::Ignore);


	//아이템
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Player",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Item",
		ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "PlayerAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Item",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "PlayerShield",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerShield", "Item",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Item",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "MonsterAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Item",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Wall",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "Item",
		ECollisionInteraction::Ignore);



	//벽
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Wall",
		ECollisionInteraction::Ignore);
	
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "Player",
		ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Wall",
		ECollisionInteraction::Block);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "PlayerAttack",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Wall",
		ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "MonsterAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Wall",
		ECollisionInteraction::Ignore);



	// 쉴드
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerShield", "MonsterAttack",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "PlayerShield",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerShield", "Monster",
		ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "PlayerShield",
		ECollisionInteraction::Block);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "PlayerShield",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerShield", "PlayerAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "PlayerShield",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerShield", "Player",
		ECollisionInteraction::Ignore);


	// 무적상태
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerInvincible", "MonsterAttack",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "PlayerInvincible",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerInvincible", "Monster",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "PlayerInvincible",
		ECollisionInteraction::Overlap);



	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "PlayerAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Player",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "MonsterAttack",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "MonsterAttack",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Player",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Monster",
		ECollisionInteraction::Ignore);


	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "MonsterAttack",
		ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Player",
		ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Monster",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "PlayerAttack",
		ECollisionInteraction::Overlap);


	return true;
}
