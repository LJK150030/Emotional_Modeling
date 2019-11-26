#include "Game/EmotionalState.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/EngineCommon.hpp"

#include "ThirdParty/imGUI/imgui_plot.h"
#include "Engine/Core/StringUtils.hpp"


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

//creates a CSV file, and adds it to the log.txt
//TODO: need to be abel to have seperate files rather than one large txt file
void EmotionalState::LogEmotionalState()
{
	FILE* file = fopen( "Data/Log/EmotionalState.csv", "w" ); 

	std::string header = "instance,";

	for(int name_idx = 0; name_idx < NUM_EMOTIONS; ++name_idx)
	{
		header.append(Emotion::m_emotionName[name_idx]);

		if(name_idx != NUM_EMOTIONS - 1)
		{
			header.append(",");
		}
		else
		{
			header.append("\n");
		}
	}

	fwrite( header.c_str(), 1, header.length(), file); 

	for(int emotional_history = 0; emotional_history < m_emotionalHistory.size(); ++emotional_history)
	{
		std::string emotion_instance = Stringf("%i,", emotional_history);
				
		for(int emotion_idx = 0; emotion_idx < NUM_EMOTIONS; ++emotion_idx)
		{
			emotion_instance.append(std::to_string(m_emotionalHistory[emotional_history][emotion_idx]));

			if(emotion_idx != NUM_EMOTIONS - 1)
			{
				emotion_instance.append(",");
			}
			else
			{
				emotion_instance.append("\n");
			}
		}

		fwrite( emotion_instance.c_str(), 1, emotion_instance.length(), file); 
	}
	
	fclose(file);
}
