#pragma once
#include "Game/Entity.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/RenderContext.hpp"

class Personality;
class EmotionalState;

class Actor: public Entity
{
public: // shared public variables
	static ID3D11ShaderResourceView* s_characterSheet;

	
public: // member public functions
	explicit Actor(Game* the_game, std::string& name, IntVec2& portrait_coord);
	virtual ~Actor();

	void Update(float delta_seconds) override;
	void Render() override;
	void Die() override;
	void DrawProfile() override;
	bool DestroyEntity() override;
	bool PopulateFromXml(std::string& file_dir) override;

	
public: // shared public functions
	static void InitCharacterSheet( uint num_port_x, uint num_port_y, uint num_pixels_x, uint num_pixels_y );
	static void DeinitCharacterSheet();

	
private: // member private functions
	Vec4		CalcUvPortraitCoord();
	void		DrawPortrait();
	void		DrawPersonalityInfo();
	void		DrawEmotionalState();

	
private: // member private variables
	// game data
	Personality* m_personality;
	EmotionalState* m_emotionalState;
	
	//profile data
	bool m_displayVerboseProfile = true;
	Vec2 m_profileSize = Vec2(50, 50);
	
	//portrait data
	IntVec2 m_portraitCoord = IntVec2::ZERO;
	Vec4 m_portraitUV = Vec4::ZERO;
	
private: // shared private variables
	static IntVec2 s_charSheetPixels;
	static IntVec2 s_charSheetPortraits;
	static Vec2 s_charSheetUVStep;
	static float s_charPortraitSize;
	static float s_charPortraitBoarderOffset;
};
