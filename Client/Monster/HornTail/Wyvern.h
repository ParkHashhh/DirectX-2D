#pragma once
#include "../Monster.h"

namespace ColorIndex
{
	enum Index
	{
		Black,
		Red,
		Blue
	};
}

class CWyvern :
	public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CWyvern();
	CWyvern(const CWyvern& ref);
	CWyvern(CWyvern&& ref)	noexcept;

private:
	ColorIndex::Index mIndex = ColorIndex::Black;
	std::string mColor;
	float mSpeed = 200.f;
	bool mIsDead = false;
	float mFloatingTime = 0.f;
public:
	virtual ~CWyvern();

protected:
	virtual CWyvern* Clone();

public:
	virtual void OnHit(const FVector3& HitPoint,
		class CCollider* Dest);


public:
	virtual bool Init();
	virtual void Damage(float Damage);
	virtual void Update(float DeltaTime);
	virtual void DieFinish();
};

