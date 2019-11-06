#include "Game/Personality.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Personality::Personality(float o, float c, float e, float a, float n) : m_personality{ o,
	c, e, a, n} { }

Personality::~Personality() = default;

float& Personality::operator[](int idx)
{
	return m_personality[idx];
}
