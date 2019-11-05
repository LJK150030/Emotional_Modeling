#pragma once
#include <vector>
#include "Game/Emotion.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "GameCommon.hpp"

struct EmotionHistory { 
	float history[MAX_HISTORY]; 
};

class EmotionalState
{
public:
	std::vector<Emotion> m_emotionalHistory;
	
public:
	EmotionHistory* GetHistory(EmotionType emotion, int start_instance, int end_instance);

public:
	EmotionalState();
	~EmotionalState();
	Emotion& GetCurrentEmotion();
	void AddEmotion(Emotion emo);
};



