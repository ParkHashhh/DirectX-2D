#pragma once

#include "World/World.h"

class CEndingWorld :
	public CWorld
{
public:
	CEndingWorld();
	virtual ~CEndingWorld();

public:
	virtual bool Init();

private:
	void LoadAnimation2D();
	void LoadSound();
	void CreateUI();
};

