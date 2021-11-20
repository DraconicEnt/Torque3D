#include <algorithm>

#include "platform/platform.h"
#include "BOL/teamManager.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "core/util/safeDelete.h"
#include "console/sim.h"
#include "console/simSet.h"
#include "scene/sceneManager.h"
#include "materials/materialManager.h"
#include "materials/sceneData.h"
#include "T3D/gameBase/gameConnection.h"
#include "console/engineAPI.h"

TeamManager* TeamManager::smActiveTeamManager = NULL;

MODULE_BEGIN( Team )
    MODULE_INIT_BEFORE( Scene )
    MODULE_SHUTDOWN_AFTER( Scene )

    MODULE_INIT
    {
        TeamManager::smActiveTeamManager = new TeamManager();
    }

    MODULE_SHUTDOWN
    {
        delete TeamManager::smActiveTeamManager;
    }
MODULE_END;


TeamManager::TeamManager( )
{

}

TeamManager::~TeamManager()
{

}

void TeamManager::removeObject(GameBase* object)
{
    auto teamVectorSearch = mTeamOwnerships.find(object->getTeam());
    if (teamVectorSearch != mTeamOwnerships.end())
    {
        std::vector<GameBase*> &teamVector = (*teamVectorSearch).second;

        auto objectSearch = std::find(teamVector.begin(), teamVector.end(), object);
        if (objectSearch != teamVector.end())
        {
            teamVector.erase(objectSearch);
        }
    }
}

void TeamManager::setTeamAssociation(GameBase* object, const U32 team)
{
    // Clear from existing team
    removeObject(object);

    // Add to new team
    mTeamOwnerships[team].push_back(object);
}

bool TeamManager::isTeamRegistered(const U32 team)
{
    auto search = mTeamOwnerships.find(team);

    if (search != mTeamOwnerships.end())
    {
        return true;
    }
    return false;
}

const std::vector<GameBase*>& TeamManager::getTeam(const U32 team)
{
    return mTeamOwnerships[team];
}

DefineEngineFunction( getTeamObjectCount, S32, ( U32 team ),,
                      "Retrieves the number of objects associated with the specified team.\n"
                      "@ingroup Team\n" )
{
    return TeamManager::smActiveTeamManager->getTeam(team).size();
}

DefineEngineFunction( getTeamObject, S32, ( U32 team, U32 object ),,
                      "Retrieves the number of objects associated with the specified team.\n"
                      "@ingroup Team\n" )
{
    const std::vector<GameBase*>& teamVector = TeamManager::smActiveTeamManager->getTeam(team);

    if (object >= teamVector.size())
    {
        return -1;
    }

    return teamVector[object]->getId();
}
