#pragma once

#include "EngineInfo.h"
#include "../ClientInfo.h"

class CItemManager
{
private:
	CItemManager();
	~CItemManager();

private:
	static CItemManager* mInst;

public:
	static CItemManager* GetInst()
	{
		if (!mInst)
			mInst = new CItemManager;
		return mInst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(mInst);
	}

public:
	std::unordered_map<std::string, std::vector<FDropItemData>> mDropMap;


	
public:
	bool Init();

	bool FindDropItemData(const std::string& MonsterName, std::vector<FDropItemData>& ItemArray)
	{
		auto iter = mDropMap.find(MonsterName);
		if (iter != mDropMap.end())
		{
			ItemArray = iter->second;
			return true;
		}
		return false;
	}

	bool LoadFile(const std::string& Path)
	{
		std::ifstream File(Path);
		if (!File.is_open())
			return false;

		std::string Line;
		std::getline(File, Line);
		while (std::getline(File, Line))
		{	
			std::stringstream ss(Line);
			std::string MonsterName, ItemName,IconPath,ItemType, DropPercent, Min, Max;

			std::getline(ss, MonsterName, ',');
			std::getline(ss, ItemName, ',');
			std::getline(ss, IconPath, ',');
			std::getline(ss, ItemType, ',');
			std::getline(ss, DropPercent, ',');
			std::getline(ss, Min, ',');
			std::getline(ss, Max, ',');

			FDropItemData Data;
			Data.Name = ItemName;
			std::wstring wstr(IconPath.begin(), IconPath.end());
			Data.IconPath = wstr;
			/*
			Equip,
			Consume,
			Install,
			Etc,
			Cash,
			*/
			if (ItemType == "Equip")
				Data.Type = EItemType::Equip;
			else if (ItemType == "Consume")
				Data.Type = EItemType::Consume;
			else if (ItemType == "Install")
				Data.Type = EItemType::Install;
			else if (ItemType == "Etc")
				Data.Type = EItemType::Etc;
			else if (ItemType == "Cash")
				Data.Type = EItemType::Cash;
			else 
				Data.Type = EItemType::Etc;
			Data.DropPercent = std::stof(DropPercent);
			Data.MinCount = std::stoi(Min);      
			Data.MaxCount = std::stoi(Max);

			mDropMap[MonsterName].push_back(Data);
		}
		return true;
	}
};

