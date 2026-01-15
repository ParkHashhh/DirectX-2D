#pragma once
#include "Item.h"

class CArrowForce:
	public CItem
{
	friend class CWorld;
	friend class CObject;
protected:
	CArrowForce();
	CArrowForce(const CArrowForce& ref);
	CArrowForce(CArrowForce&& ref)	noexcept;
public:
	virtual ~CArrowForce();

	virtual bool Init();
	virtual void GetItem(std::weak_ptr<CPlayer> Player);
	virtual void Update(float DeltaTime);
};

