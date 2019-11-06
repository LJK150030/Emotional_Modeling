#pragma once
#include "Game/Entity.hpp"

class Action final : public Entity
{
	explicit Action(Game* the_game, std::string& name);
	virtual ~Action();

	void Update(float delta_seconds) override;
	void Render() override;
	void Die() override;
	void DrawProfile() override;
	bool DestroyEntity() override;
	bool PopulateFromXml(std::string& file_dir) override;
};


