#include "Game/Personality.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Personality::Personality(float o, float c, float e, float a, float n)
{
	m_personality = std::vector<float>();

	m_personality.push_back(o);
	m_personality.push_back(c);
	m_personality.push_back(e);
	m_personality.push_back(a);
	m_personality.push_back(n);
}

Personality::~Personality()
{
}

std::vector<float> Personality::GetPersonality()
{
	return m_personality;
}

float Personality::GetOpenness()
{
	return m_personality[PERSONALITY_OPENNESS];
}

float Personality::GetConscientiousness()
{
	return m_personality[PERSONALITY_CONSCIENTIOUSNESS];
}

float Personality::GetExtroversion()
{
	return m_personality[PERSONALITY_EXTROVERSION];
}

float Personality::GetAgreeableness()
{
	return m_personality[PERSONALITY_AGREEABLENESS];
}

float Personality::GetNeuroticism()
{
	return m_personality[PERSONALITY_NEUROTICISM];
}

float& Personality::operator[](int idx)
{
	ASSERT_OR_DIE(idx < NUM_PERSONALITY_VALS, "out of bounds for personality");

	return m_personality[idx];
}
