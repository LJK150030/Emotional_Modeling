#include "Game/Personality.hpp"
#include "ThirdParty/imGUI/imgui.h"

Personality::Personality(float o, float c, float e, float a, float n) :
	m_personality{ o, c, e, a, n } { }

Personality::~Personality() = default;

void Personality::DrawImguiValues()
{
	if(ImGui::TreeNode("Personality"))
	{
		// Various
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat(":O", &m_personality[PERSONALITY_OPENNESS], MIN_UNIT_VALUE, MAX_UNIT_VALUE); ImGui::SameLine();
		ImGui::SliderFloat(":C", &m_personality[PERSONALITY_CONSCIENTIOUSNESS], MIN_UNIT_VALUE, MAX_UNIT_VALUE); ImGui::SameLine();
		ImGui::SliderFloat(":E", &m_personality[PERSONALITY_EXTROVERSION], MIN_UNIT_VALUE, MAX_UNIT_VALUE); ImGui::SameLine();
		ImGui::SliderFloat(":A", &m_personality[PERSONALITY_AGREEABLENESS], MIN_UNIT_VALUE, MAX_UNIT_VALUE); ImGui::SameLine();
		ImGui::SliderFloat(":N", &m_personality[PERSONALITY_NEUROTICISM], MIN_UNIT_VALUE, MAX_UNIT_VALUE);
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}
}

float& Personality::operator[](int idx)
{
	return m_personality[idx];
}
