#include "Game/Actor.hpp"
#include "Game/Personality.hpp"
#include "Game/EmotionalState.hpp"
#include "Game/Emotion.hpp"
#include "Engine/EngineCommon.hpp"
#include "ThirdParty/imGUI/imgui.h"
#include "Engine/Core/ErrorWarningAssert.hpp"
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


Actor::Actor(Game* the_game, std::string& name, IntVec2& portrait_coord):
	Entity(the_game, name), m_portraitCoord(portrait_coord)
{
	//given the portrait_coord, need to calculate the image UVs
	m_portraitUV = CalcUvPortraitCoord();

	// temporary, expecting to read from XML file
	m_personality = new Personality(0.7199829181, 0.8648539581, 0.6463935774, 0.8783411433, 0.3178747115);
	m_emotionalState = new EmotionalState();

	m_emotionalState->AddEmotion(Emotion::GenerateRandomEmotionInit());

	for(int num = 0; num < 10; ++num)
	{
		Emotion temp_emotion = Emotion::GenerateRandomEmotion();
		m_emotionalState->AddEmotion(m_emotionalState->GetCurrentEmotion() + temp_emotion);
	}
}


Actor::~Actor()
{
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
	
	DrawPersonalityInfo();
	DrawEmotionalState();
	
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

void Actor::DrawPersonalityInfo()
{
	if(ImGui::TreeNode("Personality"))
	{
		// Various
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat(":O", &(*m_personality)[0], 0.0f, 1.0f); ImGui::SameLine();
		ImGui::SliderFloat(":C", &(*m_personality)[1], 0.0f, 1.0f); ImGui::SameLine();
		ImGui::SliderFloat(":E", &(*m_personality)[2], 0.0f, 1.0f); ImGui::SameLine();
		ImGui::SliderFloat(":A", &(*m_personality)[3], 0.0f, 1.0f); ImGui::SameLine();
		ImGui::SliderFloat(":N", &(*m_personality)[4], 0.0f, 1.0f);
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}
}

void Actor::DrawEmotionalState()
{
	if(ImGui::TreeNode("Emotional State"))
	{
		for(uint emo_idx = 0; emo_idx < NUM_EMOTIONS; ++emo_idx)
		{
			ImGui::PlotLines(Emotion::m_emotionName[emo_idx].c_str(), 
				&m_emotionalState->m_individualEmotionHistory[emo_idx][0],
				m_emotionalState->m_individualEmotionHistory[emo_idx].size(),
				0,0, 0.0f, 1.0f);
		}

		ImGui::TreePop();
	}
}
