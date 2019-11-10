#pragma once
#include "Game/Entity.hpp"

class Action final : public Entity
{
public:
	explicit Action(Game* the_game, std::string name);
	explicit Action(Game* the_game, std::string name, float communal_effect);
	virtual ~Action();

	void Update(float delta_seconds) override;
	void Render() override;
	void Die() override;
	void DrawProfile() override;
	bool DestroyEntity() override;
	bool PopulateFromXml(std::string& file_dir) override;

	float m_communalEffect = 0.0f; // value between 0.0 and 1.0
};


