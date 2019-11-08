#pragma once
#include "Game/SocialRole.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Praise.hpp"
#include <map>

class Actor;
class Action;

struct PraiseAspectHistory
{ 
	float history[MAX_HISTORY] = {MIN_UNIT_VALUE}; 
};

class PraiseRelation
{
public:
	std::map<std::pair<Actor*, Action*>, std::vector<Praise>>	m_praiseMap;

public:
	PraiseRelation();
	~PraiseRelation();

	//mutators
	Praise PraiseTowards(Actor* me, Action* action); // will add unfamiliar if we can't find it
	void AddPraise(Praise& role);

	//accessors 
	bool DoIKnowThisAction(Actor* me, Action* action) const;
	PraiseAspectHistory* GetHistory(Actor* me, Action* action, PraiseAspect praise_aspect, int start_instance, int end_instance);
	std::vector<std::pair<Actor*, Action*>> GetConnectionList();

	//debug helpers
	void DrawImguiGraph();
};

