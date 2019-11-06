#pragma once
#include "Engine/Core/StringUtils.hpp"

struct Rgba;
struct Vec2;
struct Vertex_PCU;
class Game;

class Entity
{
public:
	explicit Entity(Game* the_game, std::string& name);
	virtual ~Entity();
	
	virtual void Update(float delta_seconds)			= 0;
	virtual void Render()								= 0;
	virtual void Die()									= 0;
	virtual void DrawProfile()							= 0;
	virtual bool DestroyEntity()						= 0;
	virtual bool PopulateFromXml(std::string& file_dir)	= 0;
	
	Game*	GetGame()		const;
	bool	IsDead()		const;
	bool	IsGarbage()		const;
	
protected:
	Game*			m_game			= nullptr;
	std::string		m_name			= "";
	bool			m_isDead		= false; // whether the Entity should [not] participate in game logic
	bool			m_isGarbage		= false; // whether the Entity should be deleted at the end of Game::Update()
};
