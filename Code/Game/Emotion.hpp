#pragma once
#include <string>
#include "Game/GameCommon.hpp"
#include "ThirdParty/imGUI/imgui.h"

enum EmotionType
{
	EMOTION_UNKNOWN = -1,
	EMOTION_POSITIVE, 
	EMOTION_NEGATIVE,
	EMOTION_PLEASED,
	EMOTION_DISPLEASED,
	EMOTION_HOPE,
	EMOTION_FEAR,
	EMOTION_JOY,
	EMOTION_DISTRESS,
	EMOTION_SATISFACTION,
	EMOTION_FEARS_CONFIRMED,
	EMOTION_RELIEF,
	EMOTION_DISAPPOINTMENT,
	EMOTION_HAPPY_FOR,
	EMOTION_RESENTMENT,
	EMOTION_GLOATING,
	EMOTION_PITY,
	EMOTION_APPROVING,
	EMOTION_DISAPPROVING,
	EMOTION_PRIDE,
	EMOTION_SHAME,
	EMOTION_ADMIRATION,
	EMOTION_REPROACH,
	EMOTION_GRATIFICATION,
	EMOTION_REMORSE,
	EMOTION_GRATITUDE,
	EMOTION_ANGER,
	EMOTION_LIKING,
	EMOTION_DISLIKING,
	EMOTION_LOVE,
	EMOTION_HATE,
	EMOTION_INTEREST,
	EMOTION_DISGUST,
	NUM_EMOTIONS

};

class Emotion
{
public:
	float m_emotions[NUM_EMOTIONS] = {MIN_UNIT_VALUE};

public:
	Emotion();
	~Emotion();

	//debug functions
	static Emotion GenerateRandomEmotionInit();
	static Emotion GenerateRandomEmotion();

	float& operator[] (int idx); 
	float& operator[] (EmotionType idx);
	Emotion operator+ (Emotion& emotion);
	void operator+=(Emotion& emotion); 


public:
	static const char* m_emotionName[NUM_EMOTIONS];
	static ImColor m_emotionColor[NUM_EMOTIONS];
	
};