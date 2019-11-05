#include "Game/EmotionalState.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/EngineCommon.hpp"

EmotionalState::EmotionalState()
{
	m_emotionalHistory = std::vector<Emotion>();
}



EmotionalState::~EmotionalState()
{
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
