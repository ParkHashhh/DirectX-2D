#pragma once

#include "World/World.h"

class CHornTailWorld :
	public CWorld
{
public:
	CHornTailWorld();
	virtual ~CHornTailWorld();

public:
	virtual bool Init();
private:
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
	void CreateMapCollider();
	void CreateMapFootHold();
};

