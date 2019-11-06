#include "Game/AttitudeRelation.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


AttitudeRelation::AttitudeRelation()
{
	m_attitudeMap = std::map<std::pair<Actor*, Object*>, std::vector<Attitude>>();
}


AttitudeRelation::~AttitudeRelation() = default;


Attitude AttitudeRelation::AttitudeTowards(Actor* me, Object* thing)
{
	const auto result = m_attitudeMap.find(std::pair<Actor*, Object*>(me, thing));

	//If I cannot find the relationship, then this is a stranger
	if(result == m_attitudeMap.end())
	{
		m_attitudeMap.emplace(
			std::pair<Actor*, Object*>(me, thing),
			std::vector<Attitude>());
		m_attitudeMap[std::pair<Actor*, Object*>(me, thing)].emplace_back(me, thing, 
			Attitude::s_unfamiliarObject);
		return m_attitudeMap[std::pair<Actor*, Object*>(me, thing)].back();
	}

	//else
	return result->second.back();
}


void AttitudeRelation::AddAttitude(Attitude& role)
{
	if(DoIKnowThisObject(role.m_origin, role.m_towards))
	{
		m_attitudeMap[role.GetConnection()].emplace_back(role);
	}
	else
	{
		m_attitudeMap.emplace(role.GetConnection(), 
			std::vector<Attitude>());

		m_attitudeMap[role.GetConnection()].emplace_back(role);
	}
}


bool AttitudeRelation::DoIKnowThisObject(Actor* me, Object* thing) const
{
	const auto result = m_attitudeMap.find(std::pair<Actor*, Object*>(me, thing));
	return result != m_attitudeMap.end();
}

// this creates a new AttitudeAspectHistory object, make sure to delete
AttitudeAspectHistory* AttitudeRelation::GetHistory(Actor* me, Object* thing, AttitudeAspect attitude_aspect,
	int start_instance, int end_instance)
{
	int size = end_instance - start_instance + 1;
	std::pair<Actor*, Object*> connection(me, thing);

	ASSERT_OR_DIE(size < MAX_HISTORY, "Attempting to get a history larger than buffer");
	ASSERT_OR_DIE(start_instance < static_cast<int>(m_attitudeMap[connection].size()), "Starting instance is larger than m_attitudeMap");
	ASSERT_OR_DIE(end_instance < static_cast<int>(m_attitudeMap[connection].size()), "Endding instance is larger than m_attitudeMap");

	AttitudeAspectHistory* attitude_aspect_history = new AttitudeAspectHistory(); 

	for(int history_idx = 0; history_idx < size; ++history_idx)
	{
		Attitude attitude_aspect_at_instance = m_attitudeMap[connection][start_instance + history_idx]; 
		attitude_aspect_history->history[history_idx] = attitude_aspect_at_instance[attitude_aspect];
	}

	return attitude_aspect_history; 
}


std::vector<std::pair<Actor*, Object*>> AttitudeRelation::GetConnectionList()
{
	std::vector<std::pair<Actor*, Object*>> connection_list;
	for(auto attitude_map_it = m_attitudeMap.begin(); attitude_map_it != m_attitudeMap.end(); ++attitude_map_it) {
		connection_list.push_back(attitude_map_it->first);
	}
	return connection_list;
}
