#pragma once

#include "UI/WidgetContainer.h"

class CNpcWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;

protected:
    CNpcWidget();

public:
    virtual ~CNpcWidget();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

private:
    void OkClick();
    void CancleClick();
};

