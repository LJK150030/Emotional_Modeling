#pragma once
#include "Game/Entity.hpp"

class Object final : public Entity
{
	explicit Object(Game* the_game, std::string name);
	virtual ~Object();

	void Update(float delta_seconds) override;
	void Render() override;
	void Die() override;
	void DrawProfile() override;
	bool DestroyEntity() override;
	bool PopulateFromXml(std::string& file_dir) override;
};
