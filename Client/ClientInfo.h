#pragma once
#include <format>
enum class EWorldType
{
	None,
	Start,
	Loading,
	Main,
	Zakum,
	Horntail,
	Ending

};



namespace EItemType
{
	enum Type
	{
		Equip,
		Consume,
		Install,
		Etc,
		Cash,
		End
	};
}


struct FDropItemData
{
	std::string Name = "";
	std::wstring IconPath = TEXT("");
	EItemType::Type Type = EItemType::End;

	float DropPercent = 0.f;
	int MinCount = 0;
	int MaxCount = 0;
};
