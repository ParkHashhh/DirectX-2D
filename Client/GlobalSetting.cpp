#include "GlobalSetting.h"
#include "CollisionInfoManager.h"
#include "Render/RenderManager.h"
#include "UI/MouseWidget.h"

CGlobalSetting::CGlobalSetting()
{
}

CGlobalSetting::~CGlobalSetting()
{
}

bool CGlobalSetting::Init()
{
	CCollisionInfoManager::GetInst()->CreateChannel("Player");
	CCollisionInfoManager::GetInst()->CreateChannel("Monster");
	CCollisionInfoManager::GetInst()->CreateChannel("BossGround");
	CCollisionInfoManager::GetInst()->CreateChannel("Item");
	CCollisionInfoManager::GetInst()->CreateChannel("PlayerAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("NearAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("MonsterAttack");
	CCollisionInfoManager::GetInst()->CreateChannel("Wall");
	CCollisionInfoManager::GetInst()->CreateChannel("Gravity");
	CCollisionInfoManager::GetInst()->CreateChannel("Portal");
	CCollisionInfoManager::GetInst()->CreateChannel("Ray");


	CCollisionInfoManager::GetInst()->CreateProfile("Player", "Player", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Monster", "Monster", true);
	CCollisionInfoManager::GetInst()->CreateProfile("BossGround", "BossGround", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Item", "Item", true);
	CCollisionInfoManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("NearAttack", "NearAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
	CCollisionInfoManager::GetInst()->CreateProfile("PlayerInvincible", "PlayerInvincible", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Wall", "Wall", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Gravity", "Gravity", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Portal", "Portal", true);
	CCollisionInfoManager::GetInst()->CreateProfile("Ray", "Ray", true);



	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Player", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Item", ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "PlayerAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Gravity", ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "Item", ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Monster", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "MonsterAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Portal", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "Ray", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Item", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Item", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Item", ECollisionInteraction::Ignore);




	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "BossGround", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "BossGround", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "PlayerAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "BossGround", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "Gravity", ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "BossGround", ECollisionInteraction::Block);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "Monster", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "BossGround", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "MonsterAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "BossGround", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "Portal", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "BossGround", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"BossGround", "Ray", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "BossGround", ECollisionInteraction::Overlap);












	//NearAttack

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "PlayerAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Wall", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Gravity", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Portal", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "NearAttack", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "MonsterAttack", ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "NearAttack", ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Monster", ECollisionInteraction::Overlap);


	//Ray
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Player", ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Ray",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Gravity",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "Ray",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "PlayerAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Ray",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Ray",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "MonsterAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Ray",
		ECollisionInteraction::Ignore);


	// Portal
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "Player",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Portal",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "PlayerAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Portal",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Portal",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "MonsterAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Portal",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "Ray",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Portal",
		ECollisionInteraction::Ignore);
		CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Portal", "Ray",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Portal",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Wall", "Ray",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "Wall",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"NearAttack", "Ray",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Ray", "NearAttack",
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
		"Gravity", "Player",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Player", "Gravity",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "PlayerAttack",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"PlayerAttack", "Gravity",
		ECollisionInteraction::Ignore);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "MonsterAttack",
		ECollisionInteraction::Overlap);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"MonsterAttack", "Gravity",
		ECollisionInteraction::Overlap);






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

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Gravity", "Monster",
		ECollisionInteraction::Ignore);
	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Gravity",
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
		"Player", "Monster",
		ECollisionInteraction::Overlap);

	CCollisionInfoManager::GetInst()->SetProfileInteraction(
		"Monster", "Player",
		ECollisionInteraction::Overlap);


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

	LoadMouseCursor();
	return true;
}

void CGlobalSetting::LoadMouseCursor()
{
	auto MouseWidget = CRenderManager::GetInst()->SetMouseWidget<CMouseWidget>(
		EMouseState::Normal, "MouseNormal").lock();

	TCHAR* FileName = new TCHAR[MAX_PATH];
	memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
	wsprintf(FileName, TEXT("Mouse/Default/DefaultCursor.png"));
	MouseWidget->SetSize(32.f, 32.f);
	MouseWidget->SetTexture("MouseNormal", FileName);
	MouseWidget->SetBrushAnimation(false);
	SAFE_DELETE(FileName);

	auto MouseGrabWidget = CRenderManager::GetInst()->SetMouseWidget<CMouseWidget>(
		EMouseState::Grab, "MouseGrab").lock();
	std::vector<const TCHAR*>	TextureFileName;
	for (int i = 0; i <= 2; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName,
			TEXT("Mouse/Grab/Grab_%03d.png"), i);
		TextureFileName.push_back(FileName);
	}

	MouseGrabWidget->SetSize(32.f, 32.f);
	MouseGrabWidget->SetTexture("MouseGrab", TextureFileName);

	MouseGrabWidget->AddBrushFrame(0.f, 0.f, 32.f, 32.f, 3);
	MouseGrabWidget->SetBrushAnimation(true);     


	for (int i = 0; i <= 2; ++i)
	{
		delete[] TextureFileName[i];
	}
	TextureFileName.clear();

	CRenderManager::GetInst()->SetMouseState(EMouseState::Normal);

}
