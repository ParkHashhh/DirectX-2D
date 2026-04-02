#pragma once
#include "../Monster.h"

class CScareCrow :
    public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CScareCrow();
	CScareCrow(const CScareCrow& ref);
	CScareCrow(CScareCrow&& ref)	noexcept;

public:
	virtual ~CScareCrow();

protected:
	virtual CScareCrow* Clone();

public:
	virtual void OnHit(const FVector3& HitPoint,
		class CCollider* Dest);

public:
	virtual bool Init();
	virtual void HitFinish();
};

