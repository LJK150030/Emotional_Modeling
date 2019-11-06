#include "Game/Attitude.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

STATIC const char* Attitude::m_attitudeAspectName[NUM_ATTITUDE_ASPECTS] = {"Attribute",
	"Familiarity"};

STATIC float Attitude::s_unfamiliarObject[NUM_ATTITUDE_ASPECTS] = { 0.5f, 0.0f };

Attitude::Attitude() = default;

Attitude::Attitude(Actor* origin, Object* towards, float init_attribute, float init_familiarity):
	m_attitudeVector{init_attribute, init_familiarity}, m_origin(origin), m_towards(towards) { }

Attitude::Attitude(Actor* origin, Object* towards, float attitude_vector[NUM_ATTITUDE_ASPECTS]):
	m_attitudeVector{attitude_vector[0], attitude_vector[1]}, m_origin(origin), m_towards(towards) { }

Attitude::~Attitude() = default;

std::pair<Actor*, Object*> Attitude::GetConnection() const
{
	return std::pair<Actor*, Object*>(m_origin, m_towards);
}

Attitude Attitude::operator+(Attitude& attitude_vec) const
{
	Attitude new_role;

	for(uint att_asp = 0; att_asp < NUM_ATTITUDE_ASPECTS; ++att_asp)
	{
		new_role[att_asp] = this->m_attitudeVector[att_asp] + attitude_vec[att_asp];
		new_role[att_asp] = ClampFloat(new_role[att_asp], MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}

	return new_role;
}


void Attitude::operator+=(Attitude& attitude_vec)
{
	for(uint att_asp = 0; att_asp < NUM_ATTITUDE_ASPECTS; ++att_asp)
	{
		m_attitudeVector[att_asp] += attitude_vec[att_asp];
		m_attitudeVector[att_asp] = ClampFloat(
			m_attitudeVector[att_asp],
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}
}

bool Attitude::operator==(Attitude& attitude_vec) const
{
	for(uint att_asp = 0; att_asp < NUM_ATTITUDE_ASPECTS; ++att_asp)
	{
		if(m_attitudeVector[att_asp] != attitude_vec.m_attitudeVector[att_asp])
			return false;
	}

	return true;
}

float& Attitude::operator[](int idx)
{
	return m_attitudeVector[idx];
}

float& Attitude::operator[](AttitudeAspect& idx)
{
	return m_attitudeVector[static_cast<int>(idx)];
}