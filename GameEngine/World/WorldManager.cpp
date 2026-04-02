#include "WorldManager.h"

CWorldManager* CWorldManager::mInst = nullptr;

CWorldManager::CWorldManager()
{
}

CWorldManager::~CWorldManager()
{
}

std::weak_ptr<class CWorld> CWorldManager::GetWorld() const
{
	return mWorld;
}

void CWorldManager::InputActive()
{
	mWorld->InputActive();
}

void CWorldManager::InputDeactive()
{
	mWorld->InputDeactive();
}

bool CWorldManager::Init()
{
	mWorld = std::make_shared<CWorld>();

	mWorld->SetSelf(mWorld);

	if (!mWorld->Init())
		return false;
	
	return true;
}

bool CWorldManager::Update(float DeltaTime)
{
	mWorld->Update(DeltaTime);

	return ChangeWorld();
}

bool CWorldManager::PostUpdate(float DeltaTime)
{
	mWorld->PostUpdate(DeltaTime);

	return ChangeWorld();
}

void CWorldManager::Render()
{
	mWorld->Render();
}

void CWorldManager::RenderUI()
{
	mWorld->RenderUI();
}

void CWorldManager::PostRender()
{
	mWorld->PostRender();
}

bool CWorldManager::ChangeWorld()
{
	if (mNextWorld)
	{
		// 애셋 매니저를 다음 월드로 추가해줄지 결정한다.
		if (mNextWorld->mKeepAsset)
		{
			mNextWorld->mWorldAssetManager->KeepAsset(
				mWorld->mWorldAssetManager);
		}
		else if (mWorld->mKeepAsset)
		{
			mNextWorld->mWorldAssetManager->KeepAsset(
				mWorld->mWorldAssetManager);
		}


		mWorld = mNextWorld;

		mNextWorld.reset();

		return true;
	}

	return false;
}
