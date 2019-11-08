#include "Game/AttitudeRelation.hpp"
#include "Game/Object.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "ThirdParty/imGUI/imgui_plot.h"


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

float GetAttitudeValFromHistory(const void* data, int idx)
{
	AttitudeAspectHistory* container = (AttitudeAspectHistory*) data;
	return container->history[idx];
}

void AttitudeRelation::DrawImguiGraph()
{
	if(ImGui::TreeNode("Attitude Relationships"))
	{
		AttitudeAspectHistory* all_values[NUM_ATTITUDE_ASPECTS];
		std::vector<std::pair<Actor*, Object*>> connection_list = GetConnectionList();

		for(auto connect_it = connection_list.begin(); connect_it != connection_list.end(); ++connect_it)
		{
			if (ImGui::TreeNode(connect_it->second->GetName().c_str()))
			{
				for(uint att_asp_idx = 0; att_asp_idx < NUM_SOCIAL_ASPECT; ++att_asp_idx)
				{
					all_values[att_asp_idx] = GetHistory(
						connect_it->first, connect_it->second, 
						static_cast<AttitudeAspect>(att_asp_idx), 0, g_numActionsEdTook - 1);
				}

				const ImVec2 graph_size(1200.0f, 300.0f);

				ImGui::PlotMultiLines(
					"", 
					NUM_ATTITUDE_ASPECTS,
					Attitude::m_attitudeAspectName, 
					Attitude::m_attitudeAspectColor, 
					GetAttitudeValFromHistory, 
					reinterpret_cast<const void* const*>(all_values), 
					g_numActionsEdTook, 
					MIN_UNIT_VALUE, 
					MAX_UNIT_VALUE, 
					graph_size);

				for(uint att_asp_idx = 0; att_asp_idx < NUM_ATTITUDE_ASPECTS; ++att_asp_idx)
				{
					delete all_values[att_asp_idx];
					all_values[att_asp_idx]  = nullptr;
				}
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
