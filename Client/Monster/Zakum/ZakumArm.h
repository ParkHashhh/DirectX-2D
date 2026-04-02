#pragma once
#include "../Monster.h"

namespace ArmIndex
{
	enum Index
	{
		Start,
		LT,
		LMT,
		LMB,
		LB,
		RT,
		RMT,
		RMB,
		RB,
		End
	};
}

class CZakumArm :
	public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CZakumArm();
	CZakumArm(const CZakumArm& ref);
	CZakumArm(CZakumArm&& ref)	noexcept;
	
private:
	ArmIndex::Index mIndex = ArmIndex::End;

public:
	virtual ~CZakumArm();

protected:
	virtual CZakumArm* Clone();

private:
	float mSkillCoolTime = 7.5f;
	float XPos = 0.f;
	std::vector <std::weak_ptr<CCollider>> mGravityList;

public:
	float GetHPMax();
	float GetHP();

	void SetMeshEnable(bool Enable);
	void SetBodyEnable(bool Enable);
	virtual void OnHit(const FVector3& HitPoint,
		class CCollider* Dest);
	void SetIndex(ArmIndex::Index Index);

	std::weak_ptr<class CColliderBox2D>GetBody()
	{
		return mBody;
	}

public:
	void SetXPos();
	void ShuffleXPosList();
	void GetGravityList();
	virtual bool Init();
	virtual void Damage(float Damage);
	virtual void Update(float DeltaTime);
	virtual void DieFinish();
	void SkillProcess();
};

