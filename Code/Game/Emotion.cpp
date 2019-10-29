#include "Game/Emotion.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

STATIC std::string Emotion::m_emotionName[NUM_EMOTIONS] = {"Positive",
	"Negative", "Pleased", "Displeased", "Hope", "Fear", "Joy", "Distress",
	"Satisfaction", "Fears Confirmed", "Relief", "Disappointment",
	"Happy For", "Resentment", "Gloating", "Pity", "Approving",
	"Disapproving", "Pride", "Shame", "Admiration", "Reproach",
	"Gratification", "Remorse", "Gratitude", "Anger", "Liking",
	"Disliking", "Love", "Hate", "Interest", "Disgust"};

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
			new_emotion[emo_idx] = 0.0f;
		}
	}

	return new_emotion;
}


Emotion::Emotion(): m_emotions{0.0f}
{
	
}


Emotion::~Emotion()
{
}


float& Emotion::operator[](const int idx)
{
	return m_emotions[idx];
}


float& Emotion::operator[](const EmotionType idx)
{
	return m_emotions[idx];
}


Emotion Emotion::operator+(Emotion& emotion)
{
	Emotion new_emotion;
	
	for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
	{
		new_emotion[emo_idx] = this->m_emotions[emo_idx] + emotion[emo_idx];
		new_emotion[emo_idx] = ClampFloat(new_emotion[emo_idx], 0.0f, 1.0f);
	}

	return new_emotion;
}
