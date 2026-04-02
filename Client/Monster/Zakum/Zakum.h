#pragma once
#include "../Monster.h"

class CZakum :
	public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CZakum();
	CZakum(const CZakum& ref);
	CZakum(CZakum&& ref)	noexcept;
	
private:
	std::weak_ptr<class CZakumArm> mArmList[8];

	std::weak_ptr<class CMonsterHPWidget> mMonsterHPWidget;
	std::function<void(float, float)>	mMonsterHPWidgetFunc;
	float mSumHP = 0.f;
	bool mIsDead = false;

public:
	virtual ~CZakum();

protected:
	virtual CZakum* Clone();



private:
	float mSkillCooltime = 10.f;
	std::vector <std::weak_ptr<CCollider>> mGravityList;
	std::vector<float> XPosList;
	float mBodyHp = 200000.f;
	bool mIsArmsDead = false;
public:
	virtual void OnHit(const FVector3& HitPoint,
		class CCollider* Dest);

	

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Damage(float Damage);
private:
	void SetXPosList();
	void GetGravityList();
	void Skill();
	void SkillEnd();
	virtual void DieFinish();
	void CreateArm();
	void SetArmPos();
	void ShuffleXPosList();
	void DropItems();
	void CreatePortal();
};

