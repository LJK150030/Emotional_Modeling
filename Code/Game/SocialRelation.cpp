#include "Game/SocialRelation.hpp"
#include "Game/Actor.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

SocialRelation::SocialRelation()
{
	m_socialMap = std::map<std::pair<Actor*, Actor*>, std::vector<SocialRole>>();
}


SocialRelation::~SocialRelation() = default;


bool SocialRelation::DoIKnowThisActor(Actor* me, Actor* them) const
{
	const auto result = m_socialMap.find(std::pair<Actor*, Actor*>(me, them));
	return result != m_socialMap.end();
}


// this creates a new SocialAspectHistory object, make sure to delete
SocialAspectHistory* SocialRelation::GetHistory(Actor* me, Actor* them, SocialAspect social_aspect, int start_instance, int end_instance)
{
	int size = end_instance - start_instance + 1;
	std::pair<Actor*, Actor*> connection(me, them);
	
	ASSERT_OR_DIE(size < MAX_HISTORY, "Attempting to get a history larger than buffer");
	ASSERT_OR_DIE(start_instance < static_cast<int>(m_socialMap[connection].size()), "Starting instance is larger than m_socialMap");
	ASSERT_OR_DIE(end_instance < static_cast<int>(m_socialMap[connection].size()), "Endding instance is larger than m_socialMap");

	SocialAspectHistory* social_aspect_history = new SocialAspectHistory(); 

	for(int history_idx = 0; history_idx < size; ++history_idx)
	{
		SocialRole social_aspect_at_instance = m_socialMap[connection][start_instance + history_idx]; 
		social_aspect_history->history[history_idx] = social_aspect_at_instance[social_aspect];
	}

	return social_aspect_history; 
}


std::vector<std::pair<Actor*, Actor*>> SocialRelation::GetConnectionList()
{
	std::vector<std::pair<Actor*, Actor*>> connection_list;
	for(auto social_map_it = m_socialMap.begin(); social_map_it != m_socialMap.end(); ++social_map_it) {
		connection_list.push_back(social_map_it->first);
	}
	return connection_list;
}


SocialRole SocialRelation::GetTheirRelationshipToMe(Actor* me, Actor* them)
{
	const auto result = m_socialMap.find(std::pair<Actor*, Actor*>(me, them));

	//If I cannot find the relationship, then this is a stranger
	if(result == m_socialMap.end())
	{
		m_socialMap.emplace(
			std::pair<Actor*, Actor*>(me, them),
			std::vector<SocialRole>());
		m_socialMap[std::pair<Actor*, Actor*>(me, them)].emplace_back(me, them, SocialRole::s_stranger);
		return m_socialMap[std::pair<Actor*, Actor*>(me, them)].back();
	}

	//else
	return result->second.back();
}


void SocialRelation::AddSocialRole(SocialRole& role)
{
	if(DoIKnowThisActor(role.m_origin, role.m_towards))
	{
		m_socialMap[role.GetConnection()].emplace_back(role);
	}
	else
	{
		m_socialMap.emplace(role.GetConnection(), 
			std::vector<SocialRole>());

		m_socialMap[role.GetConnection()].emplace_back(role);
	}
}

