#include "Game/Actor.hpp"
#include "Game/Action.hpp"
#include "Game/Personality.hpp"
#include "Game/EmotionalState.hpp"
#include "Game/Emotion.hpp"
#include "Game/SocialRelation.hpp"
#include "Game/AttitudeRelation.hpp"
#include "Game/PraiseRelation.hpp"
#include "Engine/EngineCommon.hpp"
#include "ThirdParty/imGUI/imgui.h"
#include "Engine/Renderer/RenderContext.hpp"

STATIC ID3D11ShaderResourceView* Actor::s_characterSheet = nullptr;
STATIC IntVec2 Actor::s_charSheetPixels = IntVec2::ZERO;
STATIC IntVec2 Actor::s_charSheetPortraits = IntVec2::ZERO;
STATIC Vec2 Actor::s_charSheetUVStep = Vec2::ZERO;
STATIC float Actor::s_charPortraitSize = 250.0f;
STATIC float Actor::s_charPortraitBoarderOffset = 25.0f;


STATIC void Actor::InitCharacterSheet( uint num_portrait_x, uint num_portrait_y,  
	uint num_pixels_x, uint num_pixels_y )
{
	s_charSheetPortraits = IntVec2(static_cast<int>(num_portrait_x), static_cast<int>(num_portrait_y));
	s_charSheetPixels = IntVec2(static_cast<int>(num_pixels_x), static_cast<int>(num_pixels_y));

	bool ret = g_theRenderer->LoadTextureFromFile("../Run/Data/Images/Characters_Sheet.png", &Actor::s_characterSheet, &Actor::s_charSheetPixels.x, &Actor::s_charSheetPixels.y);
	IM_ASSERT(ret);

	s_charSheetUVStep = Vec2(1.0f/num_portrait_x, 1.0f/num_portrait_y);
}


STATIC void Actor::DeinitCharacterSheet()
{
	DX_SAFE_RELEASE(Actor::s_characterSheet);
}


Actor::Actor(Game* the_game, std::string name, IntVec2& portrait_coord):
	Entity(the_game, name), m_portraitCoord(portrait_coord)
{
	//given the portrait_coord, need to calculate the image UVs
	m_portraitUV = CalcUvPortraitCoord();

	// temporary, expecting to read from XML file
	m_personality = new Personality(
		g_randomNumberGenerator.GetRandomFloatZeroToOne(), 
		g_randomNumberGenerator.GetRandomFloatZeroToOne(), 
		g_randomNumberGenerator.GetRandomFloatZeroToOne(), 
		g_randomNumberGenerator.GetRandomFloatZeroToOne(), 
		g_randomNumberGenerator.GetRandomFloatZeroToOne());
	
	m_emotionalState = new EmotionalState();
	m_perceivedSocialRelation = new SocialRelation();
	m_attitudeRelation = new AttitudeRelation();
	m_praiseRelation = new PraiseRelation();
	
	
	m_emotionalState->AddEmotion(Emotion::GenerateRandomEmotionInit());

	m_actionsExperienced = std::vector<ExperiencedActions>();

// 	for(int num = 0; num < DEMO_NUM_INTERACTIONS; ++num)
// 	{
// 		Emotion temp_emotion = Emotion::GenerateRandomEmotion();
// 		m_emotionalState->AddEmotion(m_emotionalState->GetCurrentEmotion() + temp_emotion);
// 	}
}


Actor::~Actor()
{
	delete m_praiseRelation;
	m_praiseRelation = nullptr;
	
	delete m_attitudeRelation;
	m_attitudeRelation = nullptr;

	delete m_perceivedSocialRelation;
	m_perceivedSocialRelation = nullptr;
	
	delete m_emotionalState;
	m_emotionalState = nullptr;
	
	delete m_personality;
	m_personality = nullptr;
}


void Actor::Render()
{
	DrawProfile();
}


void Actor::Die()
{
	
}


void Actor::DrawProfile()
{
	std::string window_name = "Character Profile";

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	const ImVec2 size = ImVec2(m_profileSize.x, m_profileSize.y);
	const float alpha_override = -1.0f;
	if (!ImGui::Begin(window_name.c_str(), &m_displayVerboseProfile, size, alpha_override, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Columns(2, nullptr, false);
	ImGui::SetColumnWidth(0, s_charPortraitSize + s_charPortraitBoarderOffset);
	
	DrawPortrait();
	ImGui::NextColumn();
	
	m_personality->DrawImguiValues();
	m_emotionalState->DrawImguiGraph();
	m_perceivedSocialRelation->DrawImguiGraph();
	m_attitudeRelation->DrawImguiGraph();
	m_praiseRelation->DrawImguiGraph();
	
	ImGui::End();
}


bool Actor::DestroyEntity()
{
	return false;
}


bool Actor::PopulateFromXml(std::string& file_dir)
{
	UNUSED(file_dir);
	return false;
}

Emotion Actor::GenerateEmotionFromAction(const Action& action, const Actor& from)
{
	Emotion emotion_generated;
	float starting_valiance = action.m_communalEffect;

	return emotion_generated;
}

// using PME model to calculate new emotion
void Actor::ReactToAction(Action& action, Actor& from)
{
	//Emotion_update = current_emotion + change_in_emotion + decay_emotion
	
	//Generate emotion from the action and with our current social relationship
	Emotion current_emotion = m_emotionalState->GetCurrentEmotion();
	Emotion change_in_emotion = GenerateEmotionFromAction(action, from);
	Emotion decay_emotion = Emotion::GenerateDecayEmotion();

	Emotion updated_emotion = current_emotion;
	updated_emotion += change_in_emotion;
	updated_emotion += decay_emotion;
	m_emotionalState->AddEmotion(updated_emotion);

	
	//Updating our social relationship based on the emotions that were created
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
 	test_to_dumb_update.m_origin = this;
 	test_to_dumb_update.m_towards = &from;
 	UpdateRelationship(test_to_dumb_update);

	// document and add to container
	ExperiencedActions new_experience;
	new_experience.actor = this;
	new_experience.action = &action;
	new_experience.patient = &from;
	new_experience.certainty = 1.0f;
	m_actionsExperienced.push_back(new_experience);
}


void Actor::AddRelationship(SocialRole& social_role)
{
	m_perceivedSocialRelation->AddSocialRole(social_role);
}


void Actor::UpdateRelationship(SocialRole& social_role)
{
	SocialRole current = m_perceivedSocialRelation->GetTheirRelationshipToMe(social_role.m_origin, social_role.m_towards);
	current += social_role;
	m_perceivedSocialRelation->AddSocialRole(current);
}


void Actor::LogData( Actor* relations_with )
{
	LogActionsExperienced();
	Logf("Actions", "\n");

	m_emotionalState->LogEmotionalState();
	Logf("emotional state", "\n");

	m_perceivedSocialRelation->LogSocialRelation(this, relations_with);
	Logf("social relation", "\n");

}

void Actor::LogActionsExperienced()
{
	Logf("Actions","instance, event");

	for(int action_idx = 0; action_idx < m_actionsExperienced.size(); ++action_idx)
	{
		// format: index: <actor, action, patient, certainty>
		const ExperiencedActions experienced_instance = m_actionsExperienced[action_idx];
		Logf("Actions",
			"%i, <%s, %s, %s, %f>",
			action_idx, 
			experienced_instance.actor->GetName().c_str(), 
			experienced_instance.action->GetName().c_str(),
			experienced_instance.patient->GetName().c_str(),
			experienced_instance.certainty);
	}

	LogFlush();
};

void Actor::ApplyEmotion(Emotion& emotion)
{
	m_emotionalState->AddEmotion(m_emotionalState->GetCurrentEmotion() + emotion);
}


void Actor::Update(float delta_seconds)
{
	UNUSED(delta_seconds);
}


Vec4 Actor::CalcUvPortraitCoord()
{
	float u_min = m_portraitCoord.x * s_charSheetUVStep.x;
	float v_min = m_portraitCoord.y * s_charSheetUVStep.y;
	float u_max = (m_portraitCoord.x + 1) * s_charSheetUVStep.x;
	float v_max = (m_portraitCoord.y + 1) * s_charSheetUVStep.y;

	return Vec4(u_min, v_min, u_max, v_max);
}


void Actor::DrawPortrait()
{
	const ImTextureID portrait = s_characterSheet;
	const ImVec2 portrait_size(s_charPortraitSize, s_charPortraitSize);
	const ImVec2 portrait_uv0(m_portraitUV.x, m_portraitUV.y);
	const ImVec2 portrait_uv1(m_portraitUV.z, m_portraitUV.w);
	const ImVec4 tint_col(1.0f, 1.0f, 1.0f, 1.0f);
	const ImVec4 boarder_col(1.0f, 1.0f, 1.0f, 1.0f);
	
	ImGui::Image(portrait, portrait_size, portrait_uv0, portrait_uv1, tint_col, boarder_col);

}

