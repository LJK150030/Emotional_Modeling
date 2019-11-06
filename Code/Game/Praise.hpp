#pragma once
#include "Game/GameCommon.hpp"


class Actor;
class Action;

//Attitude is how an Actor feels about an object
enum PraiseAspect
{
	PRAISE_ASPECT_UNKNOWN = -1,
	PRAISE_ASPECT_ATTRIBUTE,
	NUM_PRAISE_ASPECTS
};

class Praise
{
public:
	float m_praiseVector[NUM_PRAISE_ASPECTS] = {MIN_UNIT_VALUE};
	Actor* m_origin = nullptr;
	Action* m_towards = nullptr;

public:
	static const char* m_praiseAspectName[NUM_PRAISE_ASPECTS];
	static float s_unfamiliarAction[NUM_PRAISE_ASPECTS];

public:
	Praise();
	Praise( Actor* origin, Action* towards, float init_attribute );
	Praise( Actor* origin, Action* towards, float praise_vector[NUM_PRAISE_ASPECTS] );
	~Praise();
	
	std::pair<Actor*, Action*> GetConnection() const;

	Praise operator+(Praise& praise_vec) const; 
	void operator+=(Praise& praise_vec); 
	bool operator==(Praise& praise_vec) const;
	float& operator[] (int idx); 
	float& operator[] (PraiseAspect& idx);

private:
};