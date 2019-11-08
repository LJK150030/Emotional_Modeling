#pragma once
#include "Game/GameCommon.hpp"
#include "ThirdParty/imGUI/imgui.h"

class Actor;
class Object;

//Attitude is how an Actor feels about an object
enum AttitudeAspect
{
	ATTITUDE_ASPECT_UNKNOWN = -1,
	ATTITUDE_ASPECT_ATTRIBUTE,
	ATTITUDE_ASPECT_FAMILIARITY,
	NUM_ATTITUDE_ASPECTS

};

class Attitude
{
public:
	float m_attitudeVector[NUM_ATTITUDE_ASPECTS] = {MIN_UNIT_VALUE};
	Actor* m_origin = nullptr;
	Object* m_towards = nullptr;

public:
	static const char* m_attitudeAspectName[NUM_ATTITUDE_ASPECTS];
	static ImColor m_attitudeAspectColor[NUM_ATTITUDE_ASPECTS];
	static float s_unfamiliarObject[NUM_ATTITUDE_ASPECTS];

public:
	Attitude();
	Attitude( Actor* origin, Object* towards, float init_attribute, float init_familiarity );
	Attitude( Actor* origin, Object* towards, float attitude_vector[NUM_ATTITUDE_ASPECTS] );
	~Attitude();

	std::pair<Actor*, Object*>	GetConnection() const;
	
	Attitude operator+(Attitude& attitude_vec) const; 
	void operator+=(Attitude& attitude_vec); 
	bool operator==(Attitude& attitude_vec) const;
	float& operator[] (int idx); 
	float& operator[] (AttitudeAspect& idx);

private:
};