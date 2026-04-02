#pragma once

#include "UI/WidgetContainer.h"

class CDamageUI :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetComponent;
    friend class CWidgetContainer;

protected:
    CDamageUI();

public:
    virtual ~CDamageUI();

private:
    std::weak_ptr<class CNumberWidget> mNumberWidget;
    std::weak_ptr<class CMeshComponent>	mMeshComponent;
    std::shared_ptr<class CGameObject> mObject;
    std::shared_ptr<class CImage> mMissImage;
    float mLifeTime = 0.f;
    float mMaxLifeTime = 0.6f;
public:
    void SetNumber(int Number);
    void SetPos(FVector3 Pos);
    void SetEnable(bool Enable);
    void SetHitPos(float X, float Y);


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();
};

