#pragma once

#include "../Monster.h"



class CHornTail :
	public CMonster
{
	friend class CWorld;
	friend class CObject;
protected:
	CHornTail();
	CHornTail(const CHornTail& ref);
	CHornTail(CHornTail&& ref) noexcept;
public:
	virtual ~CHornTail();

private:
	std::shared_ptr<class CLeftHead> mLeftHead;
	std::shared_ptr<class CRightHead> mRightHead;
	std::shared_ptr<class CMiddleHead> mMiddleHead;
	std::shared_ptr<class CLeftHand> mLeftHand;
	std::shared_ptr<class CRightHand> mRightHand;
	std::shared_ptr<class CLeg> mLeg;
	std::shared_ptr<class CTail> mTail;
	std::shared_ptr<class CWing> mWing;

	
	std::vector<std::shared_ptr<class CHornTailPart>> mHeadArray;
	std::vector<std::shared_ptr<class CHornTailPart>> mBodyArray;
	std::vector<std::shared_ptr<class CHornTailPart>> mTailArray;
	std::vector<std::shared_ptr<class CHornTailPart>> mFullArray;

	std::weak_ptr<class CMonsterHPWidget> mMonsterHPWidget;
	std::function<void(float, float)>	mMonsterHPWidgetFunc;

	float mHPMax = 0.f;

	float HeadSkillDelay = 10.f;
	float BodySkillDelay = 13.f;
	float TailSkillDelay = 5.f;
	float WingSkillDelay = 30.f;

	bool mIsEnd = false;


protected:
	virtual CHornTail* Clone();

private:
	void SetRegionVector();
	void CheckDieMonster();
	void DestroyMonster();
	void SetHornTailHP(float HPMax);
	void ProcessSkill(float DeltaTime);
	void SetHPWidget();
	void DropItems();
	void CreatePortal();
public:
	
	virtual bool Init();
	virtual void Update(float DeltaTime);
};