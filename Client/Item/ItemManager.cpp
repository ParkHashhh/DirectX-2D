#include "ItemManager.h"
CItemManager* CItemManager::mInst = nullptr;

CItemManager::CItemManager()
{

}

CItemManager::~CItemManager()
{
}
bool CItemManager::Init()
{

	if (!LoadFile("ItemData.txt"))
		return false;
		
	return true;
}
