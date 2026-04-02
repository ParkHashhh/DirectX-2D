#include "EndingWorld.h"
#include "../UI/StartWidget.h"
#include "World/WorldUIManager.h"
#include "EndingObject.h"
CEndingWorld::CEndingWorld()
{
}

CEndingWorld::~CEndingWorld()
{
}

bool CEndingWorld::Init()
{
	CWorld::Init();

	LoadAnimation2D();
	LoadSound();
	std::weak_ptr<CEndingObject> BoundsObj = CreateGameObject<CEndingObject>("EndingObject");
	return true;
}

void CEndingWorld::LoadAnimation2D()
{
	mWorldAssetManager->CreateFrameAnimation(
		"EndingCredit", "EndingCredit", 1, 223, "Ending/",
		"EndingCredit_", "png", 2392,1838);
}

void CEndingWorld::LoadSound()
{

	mWorldAssetManager->LoadSound("Ending", "BGM", true,
		"Ending.mp3");
	mWorldAssetManager->SoundPlay("Ending");
}

void CEndingWorld::CreateUI()
{
}

