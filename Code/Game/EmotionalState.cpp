#include "Game/EmotionalState.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/EngineCommon.hpp"

#include "ThirdParty/imGUI/imgui_plot.h"


EmotionalState::EmotionalState()
{
	m_emotionalHistory = std::vector<Emotion>();
}



EmotionalState::~EmotionalState()
{
}


EmotionHistory* EmotionalState::GetHistory(EmotionType emotion, int start_instance, int end_instance)
{

	int size = end_instance - start_instance + 1;
	ASSERT_OR_DIE(size < MAX_HISTORY, "Attempting to get a history larger than buffer");
	ASSERT_OR_DIE(start_instance < static_cast<int>(m_emotionalHistory.size()), "Starting instance is larger than m_emotionalHistory");
	ASSERT_OR_DIE(end_instance < static_cast<int>(m_emotionalHistory.size()), "Endding instance is larger than m_emotionalHistory");

	EmotionHistory* emotion_history = new EmotionHistory(); 

	for(int history_idx = 0; history_idx < size; ++history_idx)
	{
		Emotion emotion_at_instance = m_emotionalHistory[start_instance + history_idx]; 
		emotion_history->history[history_idx] = emotion_at_instance[emotion];
	}

	return emotion_history; 
}



Emotion& EmotionalState::GetCurrentEmotion()
{
	ASSERT_OR_DIE(!m_emotionalHistory.empty(), "Trying to get an empty history")

	return  m_emotionalHistory.back();
}



void EmotionalState::AddEmotion(Emotion emo)
{
	m_emotionalHistory.push_back(emo);
}

float GetEmotionValFromHistory(const void* data, int idx)
{
	EmotionHistory* container = (EmotionHistory*) data;
	return container->history[idx];
}

void EmotionalState::DrawImguiGraph()
{
	if(ImGui::TreeNode("Emotional State"))
	{
		EmotionHistory* all_values[NUM_EMOTIONS];

		for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
		{
			all_values[emo_idx] = GetHistory(static_cast<EmotionType>(emo_idx), 0, g_numActionsEdTook - 1 );
		}

		ImVec2 graph_size(1200.0f, 300.0f);

		ImGui::PlotMultiLines(
			"", 
			NUM_EMOTIONS,
			Emotion::m_emotionName, 
			Emotion::m_emotionColor, 
			GetEmotionValFromHistory, 
			reinterpret_cast<const void* const*>(all_values), 
			g_numActionsEdTook, 
			MIN_UNIT_VALUE, 
			MAX_UNIT_VALUE, 
			graph_size);

		for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
		{
			delete all_values[emo_idx];
			all_values[emo_idx]  = nullptr;
		}

		ImGui::TreePop();
	}
}
