#include "Game/Emotion.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"

STATIC const char* Emotion::m_emotionName[NUM_EMOTIONS] = {"Positive",
	"Negative", "Pleased", "Displeased", "Hope", "Fear", "Joy", "Distress",
	"Satisfaction", "Fears Confirmed", "Relief", "Disappointment",
	"Happy For", "Resentment", "Gloating", "Pity", "Approving",
	"Disapproving", "Pride", "Shame", "Admiration", "Reproach",
	"Gratification", "Remorse", "Gratitude", "Anger", "Liking",
	"Disliking", "Love", "Hate", "Interest", "Disgust"};

STATIC ImColor Emotion::m_emotionColor[NUM_EMOTIONS] ={
	ImColor(128, 0, 0, 255),		ImColor(128, 0, 85, 255),	ImColor(128, 0, 170, 255),	ImColor(128, 0, 255, 255),
	ImColor(128, 85, 0, 255),	ImColor(128, 85, 85, 255),	ImColor(128, 85, 170, 255),	ImColor(128, 85, 255, 255),
	ImColor(128, 170, 0, 255),	ImColor(128, 170, 85, 255),	ImColor(128, 170, 170, 255),	ImColor(128, 170, 255, 255),
	ImColor(128, 255, 0, 255),	ImColor(128, 255, 85, 255),	ImColor(128, 255, 170, 255),	ImColor(128, 255, 255, 255),
	ImColor(255, 0, 0, 255),		ImColor(255, 0, 85, 255),	ImColor(255, 0, 170, 255),	ImColor(255, 0, 255, 255),
	ImColor(255, 85, 0, 255),	ImColor(255, 85, 85, 255),	ImColor(255, 85, 170, 255),	ImColor(255, 85, 255, 255),
	ImColor(255, 170, 0, 255),	ImColor(255, 170, 85, 255),	ImColor(255, 170, 170, 255),	ImColor(255, 170, 255, 255),
	ImColor(255, 255, 0, 255),	ImColor(255, 255, 85, 255),	ImColor(255, 255, 170, 255),	ImColor(255, 255, 255, 255),
};

STATIC Emotion Emotion::GenerateRandomEmotionInit()
{
	Emotion new_emotion;

	for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
	{
		new_emotion[emo_idx] = g_randomNumberGenerator.GetRandomFloatZeroToOne();
	}

	return new_emotion;
}


STATIC Emotion Emotion::GenerateRandomEmotion()
{
	Emotion new_emotion;

	for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
	{
		float coin_flip = g_randomNumberGenerator.GetRandomFloatZeroToOne();
		if(coin_flip >= 0.5f)
		{	
			new_emotion[emo_idx] = g_randomNumberGenerator.GetRandomFloatInRange(-0.1f, 0.1f);
		}
		else
		{
			new_emotion[emo_idx] = MIN_UNIT_VALUE;
		}
	}

	return new_emotion;
}

STATIC Emotion Emotion::GenerateDecayEmotion()
{
	Emotion decay_emotion;

	for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
	{
		decay_emotion[emo_idx] = EMOTION_DECAY_VALUE;
	}

	return decay_emotion;
}


Emotion::Emotion() = default;


Emotion::~Emotion() = default;


float& Emotion::operator[](const int idx)
{
	return m_emotions[idx];
}


float& Emotion::operator[](const EmotionType idx)
{
	return m_emotions[static_cast<int>(idx)];
}


Emotion Emotion::operator+(Emotion& emotion)
{
	Emotion new_emotion;
	
	for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
	{
		new_emotion[emo_idx] = this->m_emotions[emo_idx] + emotion[emo_idx];
		new_emotion[emo_idx] = ClampFloat(new_emotion[emo_idx], MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}

	return new_emotion;
}

void Emotion::operator+=(Emotion& emotion)
{
	for(uint emo_asp = 0; emo_asp < NUM_EMOTIONS; ++emo_asp)
	{
		m_emotions[emo_asp] += emotion[emo_asp];
		m_emotions[emo_asp] = ClampFloat(
			m_emotions[emo_asp],
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}
}
