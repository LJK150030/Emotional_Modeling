#include "Game/PraiseRelation.hpp"
#include "Game/Action.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "ThirdParty/imGUI/imgui_plot.h"


PraiseRelation::PraiseRelation()
{
	m_praiseMap = std::map<std::pair<Actor*, Action*>, std::vector<Praise>>();
}


PraiseRelation::~PraiseRelation() = default;


Praise PraiseRelation::PraiseTowards(Actor* me, Action* action)
{
	const auto result = m_praiseMap.find(std::pair<Actor*, Action*>(me, action));

	//If I cannot find the relationship, then this is a stranger
	if(result == m_praiseMap.end())
	{
		m_praiseMap.emplace(
			std::pair<Actor*, Action*>(me, action),
			std::vector<Praise>());
		m_praiseMap[std::pair<Actor*, Action*>(me, action)].emplace_back(me, action, 
			Praise::s_unfamiliarAction);
		return m_praiseMap[std::pair<Actor*, Action*>(me, action)].back();
	}

	//else
	return result->second.back();
}


void PraiseRelation::AddPraise(Praise& role)
{
	if(DoIKnowThisAction(role.m_origin, role.m_towards))
	{
		m_praiseMap[role.GetConnection()].emplace_back(role);
	}
	else
	{
		m_praiseMap.emplace(role.GetConnection(), 
			std::vector<Praise>());

		m_praiseMap[role.GetConnection()].emplace_back(role);
	}
}


bool PraiseRelation::DoIKnowThisAction(Actor* me, Action* action) const
{
	const auto result = m_praiseMap.find(std::pair<Actor*, Action*>(me, action));
	return result != m_praiseMap.end();
}

// this creates a new PraiseAspectHistory object, make sure to delete
PraiseAspectHistory* PraiseRelation::GetHistory(Actor* me, Action* action, PraiseAspect praise_aspect,
	int start_instance, int end_instance)
{
	int size = end_instance - start_instance + 1;
	std::pair<Actor*, Action*> connection(me, action);

	ASSERT_OR_DIE(size < MAX_HISTORY, "Attempting to get a history larger than buffer");
	ASSERT_OR_DIE(start_instance < static_cast<int>(m_praiseMap[connection].size()), "Starting instance is larger than m_praiseMap");
	ASSERT_OR_DIE(end_instance < static_cast<int>(m_praiseMap[connection].size()), "Endding instance is larger than m_praiseMap");

	PraiseAspectHistory* praise_aspect_history = new PraiseAspectHistory(); 

	for(int history_idx = 0; history_idx < size; ++history_idx)
	{
		Praise attitude_aspect_at_instance = m_praiseMap[connection][start_instance + history_idx]; 
		praise_aspect_history->history[history_idx] = attitude_aspect_at_instance[praise_aspect];
	}

	return praise_aspect_history; 
}


std::vector<std::pair<Actor*, Action*>> PraiseRelation::GetConnectionList()
{
	std::vector<std::pair<Actor*, Action*>> connection_list;
	for(auto praise_map_it = m_praiseMap.begin(); praise_map_it != m_praiseMap.end(); ++praise_map_it) {
		connection_list.push_back(praise_map_it->first);
	}
	return connection_list;
}

float GetPraiseValFromHistory(const void* data, int idx)
{
	PraiseAspectHistory* container = (PraiseAspectHistory*) data;
	return container->history[idx];
}

void PraiseRelation::DrawImguiGraph()
{
	if(ImGui::TreeNode("Praise Relationships"))
	{
		PraiseAspectHistory* all_values[NUM_PRAISE_ASPECTS];
		std::vector<std::pair<Actor*, Action*>> connection_list = GetConnectionList();

		for(auto connect_it = connection_list.begin(); connect_it != connection_list.end(); ++connect_it)
		{
			if (ImGui::TreeNode(connect_it->second->GetName().c_str()))
			{
				for(uint pra_asp_idx = 0; pra_asp_idx < NUM_SOCIAL_ASPECT; ++pra_asp_idx)
				{
					all_values[pra_asp_idx] = GetHistory(
						connect_it->first, connect_it->second, 
						static_cast<PraiseAspect>(pra_asp_idx), 0, g_numActionsEdTook - 1);
				}

				const ImVec2 graph_size(1200.0f, 300.0f);

				ImGui::PlotMultiLines(
					"", 
					NUM_PRAISE_ASPECTS,
					Praise::m_praiseAspectName, 
					Praise::m_praiseAspectColor, 
					GetPraiseValFromHistory, 
					reinterpret_cast<const void* const*>(all_values), 
					g_numActionsEdTook, 
					MIN_UNIT_VALUE, 
					MAX_UNIT_VALUE, 
					graph_size);

				for(uint pra_asp_idx = 0; pra_asp_idx < NUM_PRAISE_ASPECTS; ++pra_asp_idx)
				{
					delete all_values[pra_asp_idx];
					all_values[pra_asp_idx]  = nullptr;
				}
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
