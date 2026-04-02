#include "LoadingThread.h"
#include "World/WorldManager.h"
#include "../World/MainWorld.h"
#include "../World/StartWorld.h"
#include "../World/ZakumWorld.h"
#include "../World/HornTailWorld.h"
#include "../World/EndingWorld.h"

CLoadingThread::CLoadingThread()
{
}

CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::Exit()
{
	CThreadBase::Exit();
}

void CLoadingThread::Run()
{
	switch (mWorldType)
	{
	case EWorldType::Start:
		CWorldManager::GetInst()->CreateAsyncWorld<CStartWorld>();
		break;
	case EWorldType::Main:
		CWorldManager::GetInst()->CreateAsyncWorld<CMainWorld>();
		break;
	case EWorldType::Zakum:
		CWorldManager::GetInst()->CreateAsyncWorld<CZakumWorld>();
		break;
	case EWorldType::Horntail:
		CWorldManager::GetInst()->CreateAsyncWorld<CHornTailWorld>();
		break;
	case EWorldType::Ending:
		CWorldManager::GetInst()->CreateAsyncWorld<CEndingWorld>();
		break;
	}
}
