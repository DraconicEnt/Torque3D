#pragma once

#include <vector>
#include <unordered_map>

#ifndef _TORQUE_STRING_H_
#include "core/util/str.h"
#endif
#ifndef _TSIGNAL_H_
#include "core/util/tSignal.h"
#endif
#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif
#ifndef _GAMEBASE_H_
#include "T3D/gameBase/gameBase.h"
#endif

class TeamManager
{
private:
    /// A map for tracking all objects owned by a given team.
    std::unordered_map<U32, std::vector<GameBase*>> mTeamOwnerships;

protected:
    friend class GameBase;

    void removeObject(GameBase* object);
    void setTeamAssociation(GameBase* object, const U32 team);

public:
    TeamManager( );
    virtual ~TeamManager();

    bool isTeamRegistered(const U32 team);
    const std::vector<GameBase*>& getTeam(const U32 team);

    /// The team manager instance.
    static TeamManager* smActiveTeamManager;
};
