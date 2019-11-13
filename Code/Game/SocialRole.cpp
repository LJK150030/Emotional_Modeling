#include "SocialRole.hpp"
#include "Engine/Math/MathUtils.hpp"

RelationshipType::RelationshipType() = default;

RelationshipType::RelationshipType(const std::string& name, const float liking, const float dominance, const float solidarity, const float familiarity)
	: m_name(name), m_relationshipMakeup{liking, dominance, solidarity, familiarity} { }

RelationshipType::~RelationshipType() = default;

STATIC RelationshipType RelationshipType::s_stranger("stranger", 0.5f, 0.5f, 0.5f, 0.0f);

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
			new_social_role[sol_asp_idx] = MIN_UNIT_VALUE;
		}
	}

	return new_social_role;
}

SocialRole::SocialRole() = default;


SocialRole::SocialRole(Actor* origin, Actor* towards, float init_liking, 
	float init_dominance, float init_solidarity, float init_familiarity):
	m_relationshipMakeup{init_liking, init_dominance, init_solidarity, init_familiarity},
	m_origin(origin), m_towards(towards) { }

SocialRole::SocialRole(Actor* origin, Actor* towards, float social_role_vector[NUM_SOCIAL_ASPECT]):
	m_origin(origin), m_towards(towards)
{
	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		m_relationshipMakeup[soc_asp] = social_role_vector[soc_asp];
		m_relationshipMakeup[soc_asp] = ClampFloat(
			m_relationshipMakeup[soc_asp],
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}
}

SocialRole::SocialRole(Actor* origin, Actor* towards, RelationshipType relationship_type):
	m_origin(origin), m_towards(towards)
{
	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		m_relationshipMakeup[soc_asp] = relationship_type.m_relationshipMakeup[soc_asp];
		m_relationshipMakeup[soc_asp] = ClampFloat(
			m_relationshipMakeup[soc_asp],
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}
}

SocialRole::~SocialRole() = default;


std::pair<Actor*, Actor*> SocialRole::GetConnection() const
{
	return std::pair<Actor*, Actor*>(m_origin, m_towards);
}

RelationshipType* SocialRole::GetClosestRelationshipType()
{
	std::vector<float> distance_factor;
	int closest_relation_idx = -1;
	//use our current relationship as a point
	//for each relationship type, use their point and calculate the length
	//want to return the RelationshipType that has the smallest length
	Vec4 current_relationship(m_relationshipMakeup[0], m_relationshipMakeup[1],
		m_relationshipMakeup[2], m_relationshipMakeup[3]);
	
	for(int rel_idx = 0; rel_idx < g_validRelationships.size(); ++rel_idx)
	{
		Vec4 relationship_type(
			g_validRelationships[rel_idx]->m_relationshipMakeup[0], 
			g_validRelationships[rel_idx]->m_relationshipMakeup[1],
			g_validRelationships[rel_idx]->m_relationshipMakeup[2], 
			g_validRelationships[rel_idx]->m_relationshipMakeup[3]
		);

		const Vec4 direction = relationship_type - current_relationship;
		distance_factor.push_back(direction.GetLength());

		if(closest_relation_idx == -1)
		{
			closest_relation_idx = 0;
		}
		else
		{
			if(distance_factor[closest_relation_idx] > distance_factor.back())
			{
				closest_relation_idx = rel_idx;
			}
		}
	}

	return g_validRelationships[closest_relation_idx];
}

float SocialRole::CertaintyOfRelationshipType(RelationshipType* relationship)
{
	// we know that our vectors will have a length between 0.0f and 1.0f
	
	//create vectors from the origin to the point for
	//	current relationship
	//	relationship type

	Vec4 current_relationship(
		m_relationshipMakeup[0], 
		m_relationshipMakeup[1], 
		m_relationshipMakeup[2], 
		m_relationshipMakeup[3]
		);

	Vec4 relationship_type(
		relationship->m_relationshipMakeup[0], 
		relationship->m_relationshipMakeup[1], 
		relationship->m_relationshipMakeup[2], 
		relationship->m_relationshipMakeup[3]
	);

	// calculate the projected vector from current to type
	
	float a_dot_b = current_relationship.x * relationship_type.x +
		current_relationship.y * relationship_type.y +
		current_relationship.z * relationship_type.z +
		current_relationship.w * relationship_type.w;

	float b_length = relationship_type.GetLength();

	float scalar_projection = a_dot_b/b_length;
	
	Vec4 projected_vector = relationship_type.GetNormalize();
	projected_vector *= scalar_projection;

	// we want the length of the projected vector, and a percent of how much it
	// overlaps the type vector.

	float projected_length = projected_vector.GetLength();
	float projected_length_ratio = projected_length/b_length;

	// this vector will never go in the opposite direction of the type,
	// but the overlap can be longer than the length of the type,
	// so, we want the value to be (100% - (overlap_ratio - 100%))
	// however, now it can have a negative percentage, so clamp to 0%

	if(projected_length_ratio > 1.0f)
	{
		projected_length_ratio = 1.0f - (projected_length_ratio - 1.0f);
	}

	projected_length_ratio = ClampFloat(projected_length_ratio, 0.0f, 1.0f);

	// calculate the vector rejection by current_relationship - projected_vector
	Vec4 vector_rejection = current_relationship - projected_vector;

	// we want the length of the rejection vector, and a percent of a unit vector
	// this percentage also needs to be the reciprocal, the closer we are, the similar we are
	float vector_rejection_length = vector_rejection.GetLength();
	float vector_rejection_ratio = 1.0f - vector_rejection_length;
	projected_length_ratio = ClampFloat(vector_rejection_ratio, 0.0f, 1.0f);

	
	return projected_length_ratio * projected_length_ratio;
}

SocialRole SocialRole::operator+(SocialRole& social_role_vec) const
{
	SocialRole new_role;

	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		new_role[soc_asp] = this->m_relationshipMakeup[soc_asp] + social_role_vec[soc_asp];
		new_role[soc_asp] = ClampFloat(new_role[soc_asp], 
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
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
			MIN_UNIT_VALUE, MAX_UNIT_VALUE);
	}
}

bool SocialRole::operator==(SocialRole& social_role_vec) const
{
	for(uint soc_asp = 0; soc_asp < NUM_SOCIAL_ASPECT; ++soc_asp)
	{
		if(m_relationshipMakeup[soc_asp] != social_role_vec.m_relationshipMakeup[soc_asp])
			return false;
	}

	return true;
}

float& SocialRole::operator[](int idx)
{
	return m_relationshipMakeup[idx];
}

float& SocialRole::operator[](SocialAspect& idx)
{
	return m_relationshipMakeup[static_cast<int>(idx)];
}
