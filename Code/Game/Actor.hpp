#pragma once
#include "Game/Entity.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/RenderContext.hpp"

class Actor;
class Action;
class Emotion;
class Personality;
class EmotionalState;
class SocialRelation;
class SocialRole;
struct RelationshipType;
class AttitudeRelation;
class Attitude;
class PraiseRelation;
class Praise;

struct ExperiencedActions
{
	Actor* actor = nullptr;
	Action* action = nullptr;
	Actor* patient = nullptr;
	float certainty = 0.0f;
};

class Actor final : public Entity
{
public: // shared public variables
	static ID3D11ShaderResourceView* s_characterSheet;

	
public: // member public functions
	explicit Actor(Game* the_game, std::string name, IntVec2& portrait_coord);
	virtual ~Actor();

	void Update(float delta_seconds) override;
	void Render() override;
	void Die() override;
	void DrawProfile() override;
	bool DestroyEntity() override;
	bool PopulateFromXml(std::string& file_dir) override;

	Emotion GenerateEmotionFromAction(Action& action, Actor& from);
	SocialRole GenerateSocialRoleFromEmotion(Emotion& felt_emotion, Actor& from);
	//applying emotion from an aciton
	void ReactToAction(Action& action, Actor& from);
	
	//debugging
	void ApplyEmotion(Emotion& emotion);
	void AddRelationship(SocialRole& social_role);
	void UpdateRelationship(SocialRole& social_role);
	void DetermineRelationshipWith(Actor* relations_with);

	void LogData( Actor* relations_with );
	void LogActionsExperienced();
	
public: // shared public functions
	static void InitCharacterSheet( uint num_port_x, uint num_port_y, uint num_pixels_x, uint num_pixels_y );
	static void DeinitCharacterSheet();

	
private: // member private functions
	Vec4	CalcUvPortraitCoord();
	void	DrawPortrait();

	
private: // member private variables
	// game data
	Personality* m_personality;
	EmotionalState* m_emotionalState;
	SocialRelation* m_perceivedSocialRelation;
	AttitudeRelation* m_attitudeRelation;
	PraiseRelation* m_praiseRelation;

	//TODO: this is only for one person!!! this needs to be in the social roles class
	RelationshipType* m_closestRelationType; 

	//profile data
	bool m_displayVerboseProfile = true;
	Vec2 m_profileSize = Vec2(50, 50);
	
	//portrait data
	IntVec2 m_portraitCoord = IntVec2::ZERO;
	Vec4 m_portraitUV = Vec4::ZERO;

	std::vector<ExperiencedActions> m_actionsExperienced;
	
private: // shared private variables
	static IntVec2 s_charSheetPixels;
	static IntVec2 s_charSheetPortraits;
	static Vec2 s_charSheetUVStep;
	static float s_charPortraitSize;
	static float s_charPortraitBoarderOffset;
};
