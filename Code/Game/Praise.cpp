#include "Game/Praise.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

STATIC const char* Praise::m_praiseAspectName[NUM_PRAISE_ASPECTS] = {"Attribute"};
STATIC float Praise::s_unfamiliarAction[NUM_PRAISE_ASPECTS] = { 0.0f };

Praise::Praise() = default;

Praise::Praise(Actor* origin, Action* towards, float init_attribute):
	m_praiseVector{init_attribute}, m_origin(origin), m_towards(towards) { }

Praise::Praise(Actor* origin, Action* towards, float praise_vector[NUM_PRAISE_ASPECTS]):
	m_praiseVector{praise_vector[0]}, m_origin(origin), m_towards(towards) { }

Praise::~Praise() = default;

std::pair<Actor*, Action*> Praise::GetConnection() const
{
	return std::pair<Actor*, Action*>(m_origin, m_towards);
}

Praise Praise::operator+(Praise& praise_vec) const
{
	Praise new_role;

	for(uint pra_asp = 0; pra_asp < NUM_PRAISE_ASPECTS; ++pra_asp)
	{
		new_role[pra_asp] = this->m_praiseVector[pra_asp] + praise_vec[pra_asp];
		new_role[pra_asp] = ClampFloat(new_role[pra_asp], 
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}

	return new_role;
}

void Praise::operator+=(Praise& praise_vec)
{
	for(uint pra_asp = 0; pra_asp < NUM_PRAISE_ASPECTS; ++pra_asp)
	{
		m_praiseVector[pra_asp] += praise_vec[pra_asp];
		m_praiseVector[pra_asp] = ClampFloat(
			m_praiseVector[pra_asp],
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}
}

bool Praise::operator==(Praise& praise_vec) const
{
	for(uint att_asp = 0; att_asp < NUM_PRAISE_ASPECTS; ++att_asp)
	{
		if(m_praiseVector[att_asp] != praise_vec.m_praiseVector[att_asp])
			return false;
	}

	return true;
}

float& Praise::operator[](int idx)
{
	return m_praiseVector[idx];
}

float& Praise::operator[](PraiseAspect& idx)
{
	return m_praiseVector[static_cast<int>(idx)];
}