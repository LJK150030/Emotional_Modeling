#pragma once
#include "Game/SocialRole.hpp"
#include "Game/GameCommon.hpp"
#include <map>

class Actor;

struct SocialAspectHistory
{ 
	float history[MAX_HISTORY] = {MIN_UNIT_VALUE}; 
};

class SocialRelation
{
public:
	std::map<std::pair<Actor*, Actor*>, std::vector<SocialRole>>	m_socialMap;
	
public:
	SocialRelation();
	~SocialRelation();

	//mutators
	SocialRole GetTheirRelationshipToMe(Actor* me, Actor* them); // will add stranger if we can't find them
	void AddSocialRole(SocialRole& role);
	
	//accessors 
	bool DoIKnowThisActor(Actor* me, Actor* them) const;
	SocialAspectHistory* GetHistory(Actor* me, Actor* them, SocialAspect social_aspect, int start_instance, int end_instance);
	std::vector<std::pair<Actor*, Actor*>> GetConnectionList();

	//debug helpers
	void DrawImguiGraph();
	void LogSocialRelation();
};

