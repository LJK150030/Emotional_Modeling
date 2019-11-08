#include "Game/Entity.hpp"
#include "Game/Game.hpp"

//will own entity (PlayerShip, Asteroid, Bullet)

Entity::Entity(Game* the_game, std::string& name): m_game(the_game), m_name(name)
{
}

Entity::~Entity()
{
}

Game* Entity::GetGame() const
{
	return m_game;
}

bool Entity::IsDead() const
{
	return m_isDead;
}

bool Entity::IsGarbage() const
{
	return m_isGarbage;
}

std::string Entity::GetName() const
{
	return m_name;
}

