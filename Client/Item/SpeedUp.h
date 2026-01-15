#pragma once

#include "Item.h"

class CSpeedUp :
	public CItem
{
	friend class CWorld;
	friend class CObject;
protected:
	CSpeedUp();
	CSpeedUp(const CSpeedUp& ref);
	CSpeedUp(CSpeedUp&& ref)	noexcept;
public:
	virtual ~CSpeedUp();

	virtual bool Init();
	virtual void GetItem(std::weak_ptr<CPlayer> Player);
	virtual void Update(float DeltaTime);
};