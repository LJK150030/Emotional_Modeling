#pragma once
#include "Game/GameCommon.hpp"
#include "ThirdParty/imGUI/imgui.h"
#include <utility>
#include "Emotion.hpp"

class Actor;

enum SocialAspect
{
	SOCIAL_ASPECT_UNKNOWN = -1,
	SOCIAL_ASPECT_LIKING, 
	SOCIAL_ASPECT_DOMINANCE, 
	SOCIAL_ASPECT_SOLIDARITY,
	SOCIAL_ASPECT_FAMILIARITY,
	NUM_SOCIAL_ASPECT
};


struct RelationshipType
{
	RelationshipType();
	RelationshipType(const std::string& name, float liking, float dominance, float solidarity, float familiarity);
	~RelationshipType();

	const char* GetDebugName() const;
	
	//set default values to the stranger value
	std::string m_name = "stranger";
	float m_relationshipMakeup[NUM_SOCIAL_ASPECT] = {0.5f, 0.5f, 0.5f, 0.0f};

	static RelationshipType s_stranger;
	
};


class SocialRole
{
public:
	const float FAMILIARITY_GROWTH = 0.02f;
	const float FAMILIARITY_DECAY = 0.02f; //this is not used because we are constantly talking to our test actor
	float m_relationshipMakeup[NUM_SOCIAL_ASPECT] = {MIN_UNIT_VALUE};
	Actor* m_origin = nullptr;
	Actor* m_towards = nullptr;

public:
	static const char* m_socialAspectName[NUM_SOCIAL_ASPECT];
	static ImColor m_socialAspectColor[NUM_SOCIAL_ASPECT];
	
public:
	SocialRole();
	SocialRole(Actor* origin, Actor* towards, float init_liking, float init_dominance, 
		float init_solidarity, float init_familiarity);
	SocialRole(Actor* origin, Actor* towards, float social_role_vector[NUM_SOCIAL_ASPECT]);
	SocialRole(Actor* origin, Actor* towards, RelationshipType relationship_type);
	SocialRole(Actor* origin, Actor* towards, const Emotion& emotion);
	~SocialRole();
	
	//accessors 
	std::pair<Actor*, Actor*>	GetConnection() const;
	RelationshipType*			GetClosestRelationshipType();
	float						GetCertaintyOfRelationshipType( int rel_idx);
	float						CertRelatDot(RelationshipType* relationship);
	float						CertRelatLength(RelationshipType* relationship);

	
	//mutators
	SocialRole operator+(SocialRole& social_role_vec) const; 
	void operator+=(SocialRole& social_role_vec); 
	bool operator==(SocialRole& social_role_vec) const;
	float& operator[] (int idx); 
	float& operator[] (SocialAspect& idx);

	//hellper
	static SocialRole	GenerateRandomSocialRoleInit();
	static SocialRole	GenerateRandomSocialRole();
};

//helper functions

//length apart from the two social roles