#pragma once
#include <vector>

enum PersonalityIdex
{
	PERSONALITY_UNKNOWN = -1,
	PERSONALITY_OPENNESS,
	PERSONALITY_CONSCIENTIOUSNESS,
	PERSONALITY_EXTROVERSION,
	PERSONALITY_AGREEABLENESS,
	PERSONALITY_NEUROTICISM,
	NUM_PERSONALITY_VALS
};

class Personality
{
public:
	Personality(float o, float c, float e, float a, float n);
	~Personality();

	std::vector<float> GetPersonality();
	float GetOpenness();
	float GetConscientiousness();
	float GetExtroversion();
	float GetAgreeableness();
	float GetNeuroticism();
	float &operator[] (int idx); 
private:
	std::vector<float> m_personality;
};
