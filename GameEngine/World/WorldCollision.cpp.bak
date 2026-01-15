#include "WorldCollision.h"
#include "../Component/Collider.h"
#include "../Component/ColliderSphere2D.h"
#include "../Object/GameObject.h"

CWorldCollision::CWorldCollision()
{
}

CWorldCollision::~CWorldCollision()
{
}

void CWorldCollision::AddCollider(
	const std::weak_ptr<class CCollider>& Collider)
{
	mColliderList.push_back(Collider);
}

bool CWorldCollision::Init()
{
	return true;
}

void CWorldCollision::Update(float DeltaTime)
{
	/*if (mInterval > 0.f)
	{
		mIntervalTime += DeltaTime;

		if (mIntervalTime < mInterval)
			return;

		mIntervalTime -= mInterval;
	}*/

	// 충돌체 정보를 모두 갱신한다.
	UpdateInfo();

	auto	iter = mColliderList.begin();
	auto	iterEnd = mColliderList.end();
	--iterEnd;

	auto	iterEnd1 = mColliderList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter).expired())
		{
			iter = mColliderList.erase(iter);
			iterEnd = mColliderList.end();
			iterEnd1 = iterEnd;
			--iterEnd;
			continue;
		}

		auto	SrcCollider = (*iter).lock();

		if (!SrcCollider->GetAlive())
		{
			iter = mColliderList.erase(iter);
			iterEnd = mColliderList.end();
			iterEnd1 = iterEnd;
			--iterEnd;
			continue;
		}

		else if (!SrcCollider->GetEnable())
		{
			++iter;
			continue;
		}

		FCollisionProfile* SrcProfile = SrcCollider->GetCollisionProfile();

		if (!SrcProfile->Enable)
		{
			++iter;
			continue;
		}

		auto	iter1 = iter;
		++iter1;

		for (; iter1 != iterEnd1;)
		{
			if ((*iter1).expired())
			{
				iter1 = mColliderList.erase(iter1);
				iterEnd = mColliderList.end();
				iterEnd1 = iterEnd;
				--iterEnd;
				continue;
			}

			auto	DestCollider = (*iter1).lock();

			if (!DestCollider->GetAlive())
			{
				iter1 = mColliderList.erase(iter1);
				iterEnd = mColliderList.end();
				iterEnd1 = iterEnd;
				--iterEnd;
				continue;
			}

			else if (!DestCollider->GetEnable())
			{
				++iter1;
				continue;
			}

			// 두 물체의 충돌처리를 진행한다.
			// 두 물체의 충돌 프로파일을 비교한다.
			FCollisionProfile* DestProfile = DestCollider->GetCollisionProfile();

			if (!DestProfile->Enable)
			{
				++iter1;
				continue;
			}

			// 상대방 채널에 대해 충돌이 무시일 경우 충돌처리를 건너뛴다.
			else if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Ignore ||
				DestProfile->Interaction[SrcProfile->Channel->Channel] == ECollisionInteraction::Ignore)
			{
				++iter1;
				continue;
			}

			// Overlap vs Block 처리 못함. 서로 같을 경우에만 처리.
			else if (SrcProfile->Interaction[DestProfile->Channel->Channel] !=
				DestProfile->Interaction[SrcProfile->Channel->Channel])
			{
				++iter1;
				continue;
			}

			// 실제 충돌처리를 진행한다.
			FVector3	HitPoint;

			// Collision함수의 반환값이 true일 경우 충돌 되었다는
			// 의미이다.
			if (SrcCollider->Collision(HitPoint, DestCollider))
			{
				// Hit 충돌의 경우 충돌 되었다가 떨어지는 것이 없기
				// 때문에 Hit일 경우를 체크하여 바로 진행한다.
				if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Block)
				{
					// 둘중 이동하고 있던 물체를 찾는다.
					// RootComponent를 기준으로 찾는다.
					// 만약 둘 다 이동하고 있던 물체라면 두 물체는
					// 겹치는 구간의 반씩 반대방향으로 이동한다.
					auto SrcObj = SrcCollider->GetOwner().lock();
					auto DestObj = DestCollider->GetOwner().lock();

					// 현재 프레임에 얼마나 움직였는지를 가지고 온다.
					FVector3	SrcVelocity = SrcObj->GetVelocity();
					FVector3	DestVelocity = DestObj->GetVelocity();

					// 둘다 움직이고 있을 경우
					if (!SrcVelocity.IsZero() &&
						!DestVelocity.IsZero())
					{
						// 서로의 x, y, z 절반씩 이동한다.
						// 속도 / -2.f 를 하면 이동한 양의 반을 반대방향
						// 으로 이동시키는데 여기에 1.01을 곱해서 1% 만큼
						// 더 반대 방향으로 이동하게 한다.
						SrcObj->AddWorldPos(SrcVelocity / -2.f * 1.01f);
						DestObj->AddWorldPos(DestVelocity / -2.f * 1.01f);

						SrcCollider->UpdateInfo();
						DestCollider->UpdateInfo();
					}
					
					// RigidBody 만들어야 회전까지 처리됨.
					// Src가 움직이고 있을 경우
					else if (!SrcVelocity.IsZero())
					{
						SrcObj->AddWorldPos(-(SrcVelocity * 1.01f));

						SrcCollider->UpdateInfo();
					}

					// Dest가 움직이고 있을 경우
					else if (!DestVelocity.IsZero())
					{
						DestObj->AddWorldPos(-(DestVelocity * 1.01f));

						DestCollider->UpdateInfo();
					}

					SrcObj->ClearPhysics();
					DestObj->ClearPhysics();


					SrcCollider->CallCollisionHit(HitPoint,
						*iter1);

					DestCollider->CallCollisionHit(HitPoint,
						*iter);
				}

				// Overlap일 경우
				else
				{
					if (!SrcCollider->CheckCollisionObject(
						DestCollider.get()))
					{
						SrcCollider->CallCollisionBegin(HitPoint,
							*iter1);

						DestCollider->CallCollisionBegin(HitPoint,
							*iter);
					}
				}
			}

			// 두 물체가 현재 프레임에는 부딪히지 않았지만 이전프레임 까지는
			// 부딪히고 있었을 경우는 이제 막 떨어지는 경우이다.
			else if (SrcCollider->CheckCollisionObject(
				DestCollider.get()))
			{
				if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Overlap)
				{
					SrcCollider->CallCollisionEnd(DestCollider.get());

					DestCollider->CallCollisionEnd(SrcCollider.get());
				}

				else
				{
				}
			}

			++iter1;
		}

		++iter;
	}
}

void CWorldCollision::UpdateInfo()
{
	auto	iter = mColliderList.begin();
	auto	iterEnd = mColliderList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter).expired())
		{
			iter = mColliderList.erase(iter);
			iterEnd = mColliderList.end();
			continue;
		}

		auto	Collider = (*iter).lock();

		if (!Collider->GetAlive())
		{
			iter = mColliderList.erase(iter);
			iterEnd = mColliderList.end();
			continue;
		}

		Collider->UpdateInfo();
		++iter;
	}
}
