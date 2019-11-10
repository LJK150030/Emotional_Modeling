#include "Game/Object.hpp"
#include "Engine/EngineCommon.hpp"

Object::Object(Game* the_game, std::string name) :	Entity(the_game, name)
{
	
}

Object::~Object()
{
}

void Object::Update(float delta_seconds)
{
	UNUSED(delta_seconds)
}

void Object::Render()
{
}

void Object::Die()
{
}

void Object::DrawProfile()
{
}

bool Object::DestroyEntity()
{
	return false;
}

bool Object::PopulateFromXml(std::string& file_dir)
{
	UNUSED(file_dir)
	return false;
}
