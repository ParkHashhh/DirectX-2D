#pragma once

#include "World/World.h"

class CMainWorld :
    public CWorld
{
public:
	CMainWorld();
	virtual ~CMainWorld();

public:
	virtual bool Init();
private:
	std::weak_ptr<class CColliderBox2D>	mBody;
	std::weak_ptr<class CMeshComponent>	mMeshComponent;
private:
	void LoadAnimation2D();
};

