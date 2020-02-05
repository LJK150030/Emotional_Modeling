#include "Game/GameCommon.hpp"
#include "Engine/Tools/Profiler.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Game/App.hpp"


App*					g_theApp = nullptr;

//NamedStrings g_gameConfigBlackboard;
float g_startTime = 0.0f;
int g_numActionsEdTook = 1;
const char* g_socialRoleEdPicked = "";
bool g_socialRoleSelectedThisTurn = false;
std::vector<Action*> g_validActions = std::vector<Action*>();
std::vector<RelationshipType*> g_validRelationships = std::vector<RelationshipType*>();

//--------------------------------------------------------------
