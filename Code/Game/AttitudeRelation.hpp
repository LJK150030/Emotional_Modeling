#pragma once
#include "Game/SocialRole.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Attitude.hpp"
#include <map>

class Actor;
class Object;

struct AttitudeAspectHistory
{ 
	float history[MAX_HISTORY] = {MIN_UNIT_VALUE}; 
};

class AttitudeRelation
{
public:
	std::map<std::pair<Actor*, Object*>, std::vector<Attitude>>	m_attitudeMap;

public:
	AttitudeRelation();
	~AttitudeRelation();

	//mutators
	Attitude AttitudeTowards(Actor* me, Object* thing); // will add unfamiliar if we can't find it
	void AddAttitude(Attitude& role);

	//accessors 
	bool DoIKnowThisObject(Actor* me, Object* thing) const;
	AttitudeAspectHistory* GetHistory(Actor* me, Object* thing, AttitudeAspect attitude_aspect, int start_instance, int end_instance);
	std::vector<std::pair<Actor*, Object*>> GetConnectionList();

	//debug helpers
	void DrawImguiGraph();
};

