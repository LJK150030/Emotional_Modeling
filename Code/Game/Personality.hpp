#pragma once
#include "Game/GameCommon.hpp"

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
	float m_personality[NUM_PERSONALITY_VALS] = {MIN_UNIT_VALUE};
	
public:
	Personality(float o, float c, float e, float a, float n);
	~Personality();

	float &operator[] (int idx); 
};
