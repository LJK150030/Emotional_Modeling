#include "SocialRole.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

STATIC float SocialRole::s_stranger[NUM_SOCIAL_ASPECT] = {0.1f, 0.1f, 0.1f, 0.0f};

STATIC const char* SocialRole::m_socialAspectName[NUM_SOCIAL_ASPECT] = {
	"Liking",
	"Dominance",
	"Solidarity",
	"Familiarity"
};

STATIC ImColor SocialRole::m_socialAspectColor[NUM_SOCIAL_ASPECT] ={
	ImColor(255, 255, 0, 255),
	ImColor(255, 0, 255, 255),
	ImColor(0, 255, 255, 255),
	ImColor(255, 255, 255, 255),
};

STATIC SocialRole SocialRole::GenerateRandomSocialRoleInit()
{
	SocialRole new_social_role;

	for(uint sol_asp_idx = 0; sol_asp_idx < NUM_SOCIAL_ASPECT; ++sol_asp_idx)
	{
		new_social_role[sol_asp_idx] = g_randomNumberGenerator.GetRandomFloatZeroToOne();
	}

	return new_social_role;
}


STATIC SocialRole SocialRole::GenerateRandomSocialRole()
{
	SocialRole new_social_role;

	for(uint sol_asp_idx = 0; sol_asp_idx < NUM_SOCIAL_ASPECT; ++sol_asp_idx)
	{
		float coin_flip = g_randomNumberGenerator.GetRandomFloatZeroToOne();
		if(coin_flip >= 0.5f)
		{	
			new_social_role[sol_asp_idx] = g_randomNumberGenerator.GetRandomFloatInRange(-0.05f, 0.05f);
		}
		else
		{
			new_social_role[sol_asp_idx] = 0.0f;
		}
	}

	return new_social_role;
}

SocialRole::SocialRole() : m_relationshipMakeup{0.0f} {}


SocialRole::SocialRole(Actor* origin, Actor* towards, float init_liking, 
	float init_dominance, float init_solidarity, float init_familiarity):
	m_origin(origin), m_towards(towards), m_relationshipMakeup{init_liking,
	init_dominance, init_solidarity, init_familiarity} { }

SocialRole::SocialRole(Actor* origin, Actor* towards, float social_role_vector[NUM_SOCIAL_ASPECT]):
	m_origin(origin), m_towards(towards)
{
	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		m_relationshipMakeup[soc_asp] = social_role_vector[soc_asp];
		m_relationshipMakeup[soc_asp] = ClampFloat(
			m_relationshipMakeup[soc_asp],
			0.0f, 1.0f);
	}
}

SocialRole::~SocialRole() = default;


std::pair<Actor*, Actor*> SocialRole::GetConnection() const
{
	return std::pair<Actor*, Actor*>(m_origin, m_towards);
}

SocialRole SocialRole::operator+(SocialRole& social_role_vec) const
{
	SocialRole new_role;

	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		new_role[soc_asp] = this->m_relationshipMakeup[soc_asp] + social_role_vec[soc_asp];
		new_role[soc_asp] = ClampFloat(new_role[soc_asp], 0.0f, 1.0f);
	}

	return new_role;
}

void SocialRole::operator+=(SocialRole& social_role_vec)
{
	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		m_relationshipMakeup[soc_asp] += social_role_vec[soc_asp];
		m_relationshipMakeup[soc_asp] = ClampFloat(
			m_relationshipMakeup[soc_asp],
			0.0f, 1.0f);
	}
}

bool SocialRole::operator==(SocialRole& social_role_vec) const
{
	return (
		m_relationshipMakeup[SOCIAL_ASPECT_LIKING] == social_role_vec.m_relationshipMakeup[SOCIAL_ASPECT_LIKING] &&
		m_relationshipMakeup[SOCIAL_ASPECT_DOMINANCE] == social_role_vec.m_relationshipMakeup[SOCIAL_ASPECT_DOMINANCE] &&
		m_relationshipMakeup[SOCIAL_ASPECT_SOLIDARITY] == social_role_vec.m_relationshipMakeup[SOCIAL_ASPECT_SOLIDARITY] &&
		m_relationshipMakeup[SOCIAL_ASPECT_FAMILIARITY] == social_role_vec.m_relationshipMakeup[SOCIAL_ASPECT_FAMILIARITY]
		);
}

float& SocialRole::operator[](int idx)
{
	return m_relationshipMakeup[idx];
}

float& SocialRole::operator[](SocialAspect& idx)
{
	return m_relationshipMakeup[static_cast<int>(idx)];
}
