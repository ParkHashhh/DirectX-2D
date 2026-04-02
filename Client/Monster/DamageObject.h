#pragma once
#include "Object/GameObject.h"
class CDamageObject :
    public CGameObject
{
		friend class CWorld;
		friend class CObject;

	protected:
		CDamageObject();
		CDamageObject(const CDamageObject& ref);
		CDamageObject(CDamageObject&& ref)	noexcept;

	public:
		virtual ~CDamageObject();

	private:
		std::weak_ptr<class CWidgetComponent>	mWidgetComponent;
		std::shared_ptr<class CDamageUI> mDamageUI;

	public:
		void SetHitWorldPos(float x, float y);
		virtual bool Init();
		virtual void Update(float DeltaTime);
		void SetDamage(int Damage);

	protected:
		virtual CDamageObject* Clone();

};

