
#include "Engine.h"
#include "resource.h"
#include "World/WorldManager.h"
#include "World/MainWorld.h"
#include "World/StartWorld.h"
#include "World/ZakumWorld.h"
#include "World/EndingWorld.h"
#include "World/HornTailWorld.h"
#include "Player/Player.h"
#include "Player/Shuriken.h"
#include "Monster/Monster.h"
#include "Monster/MonsterSpawnPoint.h"
#include "Monster/ScareCrow/ScareCrow.h"
#include "Item/ItemManager.h"
#include "Component/StateComponent.h"
#include "GlobalSetting.h"
#include "UI/InventoryManager.h"
#ifdef _DEBUG
// 라이브러리 링크를 걸어준다.
#pragma comment(lib, "GameEngine_Debug.lib")

#else

#pragma comment(lib, "GameEngine.lib")

#endif // _DEBUG


// HINSTANCE : 이 프로그램의 식별번호이다.
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    CEngine::GetInst()->CreateEngineSetting<CGlobalSetting>();

    if (!CEngine::GetInst()->Init(hInstance, TEXT("GameClient"), IDI_ICON1, IDI_ICON1,
        1280, 720, true))
    {
        CEngine::DestroyInst();
        return 0;
    }

    CEngine::CreateCDO<CPlayer>();
    CEngine::CreateCDO<CShuriken>();
    CEngine::CreateCDO<CMonster>();
    CEngine::CreateCDO<CScareCrow>();
    CEngine::CreateCDO<CStateComponent>();

    //아이템 데이터 불러오기
    CItemManager::GetInst()->Init();
    CInventoryManager::GetInst()->Init();
    // 시작 월드를 지정한다.
    CWorldManager::GetInst()->CreateWorld<CStartWorld>(false);
    int Ret = CEngine::GetInst()->Run();

    CEngine::DestroyInst();

    return Ret;
}
