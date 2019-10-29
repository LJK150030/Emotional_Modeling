#include "Game/EmotionalState.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/EngineCommon.hpp"

EmotionalState::EmotionalState()
{
	m_emotionalHistory = std::vector<Emotion>();
	m_individualEmotionHistory = std::vector<std::vector<float>>(32);
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

	for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
	{	
		m_individualEmotionHistory[emo_idx].push_back(emo[emo_idx]);
	}
}
