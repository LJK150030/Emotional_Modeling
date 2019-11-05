#pragma once
#include "Actor.hpp"
#include "SocialRole.hpp"
#include "GameCommon.hpp"
#include <map>

struct SocialAspectHistory { 
	float history[MAX_HISTORY]; 
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

};

