#pragma once
#include "UI/WidgetContainer.h"
class CPlayerSkillStateWidget :
    public CWidgetContainer
{
    friend class CWorldUIManager;
    friend class CWidgetContainer;

protected:
    CPlayerSkillStateWidget();

public:
    virtual ~CPlayerSkillStateWidget();

private:
    std::vector<std::shared_ptr<class CSkillSlot>> mBuffList;
    std::weak_ptr<class CPlayer> mPlayer;
private:

    void ResetBuffPositions();

public:
    void SetPlayer(std::weak_ptr<class CPlayer> Player)
    {
        mPlayer = Player;
    }

    void CreateSkillSlot(const std::string& Name, const TCHAR* FileName, float MaxTime);
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render();

};

