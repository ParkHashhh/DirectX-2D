#include "StartWorld.h"
#include "../UI/StartWidget.h"
#include "World/WorldUIManager.h"
CStartWorld::CStartWorld()
{
}

CStartWorld::~CStartWorld()
{
}

bool CStartWorld::Init()
{
	CWorld::Init();

	LoadAnimation2D();

	LoadSound();

	CreateUI();

	return true;
}

void CStartWorld::LoadAnimation2D()
{
}

void CStartWorld::LoadSound()
{
	mWorldAssetManager->LoadSound("Login", "BGM", true,
		"Login.mp3");


	mWorldAssetManager->LoadSound("Login", "BGM", true,
		"Login.mp3");

	mWorldAssetManager->SoundPlay("Login");
}

void CStartWorld::CreateUI()
{
	std::weak_ptr<CStartWidget>	MainWidget =
		mUIManager->CreateWidget<CStartWidget>("StartWidget");

}

