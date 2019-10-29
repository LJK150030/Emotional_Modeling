#pragma once
#include <vector>
#include "Game/Emotion.hpp"

class EmotionalState
{
public:
	std::vector<Emotion> m_emotionalHistory;
	std::vector<std::vector<float>> m_individualEmotionHistory;
	
public:
	EmotionalState();
	~EmotionalState();
	Emotion& GetCurrentEmotion();
	void AddEmotion(Emotion emo);
};
