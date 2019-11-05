#pragma once
#include "Engine/Math/Vec4.hpp"
#include "ThirdParty/imGUI/imgui.h"
#include <utility>

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

class SocialRole
{
public:
	float m_relationshipMakeup[NUM_SOCIAL_ASPECT];
	Actor* m_origin = nullptr;
	Actor* m_towards = nullptr;

public:
	static float s_stranger[NUM_SOCIAL_ASPECT];
	static const char* m_socialAspectName[NUM_SOCIAL_ASPECT];
	static ImColor m_socialAspectColor[NUM_SOCIAL_ASPECT];
	
public:
	SocialRole();
	SocialRole(Actor* origin, Actor* towards, float init_liking, float init_dominance, 
		float init_solidarity, float init_familiarity);
	SocialRole(Actor* origin, Actor* towards, float social_role_vector[NUM_SOCIAL_ASPECT]);
	~SocialRole();
	
	//accessors 
	std::pair<Actor*, Actor*>	GetConnection() const;

	
	//mutators
	SocialRole operator+(SocialRole& social_role_vec) const; 
	void operator+=(SocialRole& social_role_vec); 
	bool operator==(SocialRole& social_role_vec) const;
	float& operator[] (int idx); 
	float& operator[] (SocialAspect& idx);

public:
	static SocialRole GenerateRandomSocialRoleInit();
	static SocialRole GenerateRandomSocialRole();
};
