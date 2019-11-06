#include "Game/Action.hpp"
#include "Engine/EngineCommon.hpp"

Action::Action(Game* the_game, std::string& name) :	Entity(the_game, name)
{
}

Action::~Action()
{
}

void Action::Update(float delta_seconds)
{
	UNUSED(delta_seconds);
}

void Action::Render()
{
}

void Action::Die()
{
}

void Action::DrawProfile()
{
}

bool Action::DestroyEntity()
{
	return false;
}

bool Action::PopulateFromXml(std::string& file_dir)
{
	UNUSED(file_dir);
	return false;
}
