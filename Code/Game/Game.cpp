#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/WindowContext.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Renderer/GPUMesh.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/DebugRender.hpp"
#include "Engine/Renderer/ImGUISystem.hpp"
#include "Engine/Tools/Profiler.hpp"
#include "ThirdParty/imGUI/imgui.h"
#include "Game/Actor.hpp"
#include "Game/SocialRole.hpp"
#include "Emotion.hpp"

Game::Game()
{
	LoadGameAssets();
}


Game::~Game()
{
	delete m_testActor;
	m_testActor = nullptr;

	delete m_dumbActor;
	m_dumbActor = nullptr;
}

void Game::Startup()
{
	
	m_gameCamera = new Camera();
	m_gameCamera->SetColorTarget(nullptr); // when binding, if nullptr, use the backbuffer
	m_gameCamera->SetOrthoView(Vec2::ZERO, 
		Vec2(static_cast<float>(WINDOW_RES_X), 
			static_cast<float>(WINDOW_RES_Y)));

	m_defaultShader = g_theRenderer->CreateOrGetShader("default_lit.hlsl");

}


void Game::Shutdown()
{
	Actor::DeinitCharacterSheet();

	delete m_gameCamera;
	m_gameCamera = nullptr;

}


void Game::Update(const double delta_seconds)
{
	m_time += static_cast<float>(delta_seconds);
	m_currentFrame++;
	float avg_fps = static_cast<float>(m_currentFrame)/(GetCurrentTimeSecondsF() - g_startTime);

	g_theDebugRenderer->DebugRenderMessagef(0.0f, Rgba::YELLOW, Rgba::YELLOW, "current frame: %d", m_currentFrame);
	g_theDebugRenderer->DebugRenderMessagef(0.0f, Rgba::YELLOW, Rgba::YELLOW, "current time: %f", GetCurrentTimeSecondsF());
	g_theDebugRenderer->DebugRenderMessagef(0.0f, Rgba::YELLOW, Rgba::YELLOW, "FPS: %f", avg_fps);
}


void Game::UpdateImGUI()
{

	// Feed inputs to dear imgui, start new frame
	g_imGUI->BeginFrame();
	ImGui::NewFrame();

	//TestImGui();

	m_testActor->Render();
	UpdateEdsActions();

	// Render dear imgui into screen
	g_imGUI->Render();
	g_imGUI->EndFrame();

}


void Game::Render() const
{
	ColorTargetView* rtv = g_theRenderer->GetFrameColorTarget();
	m_gameCamera->SetColorTarget(rtv);

	// Move the camera to where it is in the scene
	// (right now, no rotation (looking forward), set 10 back (so looking at 0,0,0)
	const Matrix44 cam_model = Matrix44::ForEulerZXY(m_camEuler, m_camPosition);
	m_gameCamera->SetModelMatrix(cam_model);

	g_theRenderer->BeginCamera(m_gameCamera);
	g_theRenderer->ClearScreen(Rgba::BLACK);
	g_theRenderer->ClearDepthStencilTarget(1.0f);

	g_theRenderer->EndCamera(m_gameCamera);
	g_theDebugRenderer->RenderToCamera(m_gameCamera);
}


bool Game::HandleKeyPressed(const unsigned char key_code)
{
	switch (key_code)
	{
	case SPACE_BAR:
		{
			//Ed performs an action onto Bob
			EddPerformsRandomAction();
			return true;
		}
	default:
		return false;
	}
}


bool Game::HandleKeyReleased(const unsigned char key_code)
{
	UNUSED(key_code);
	return true;
}


void Game::GarbageCollection() const
{
}

void Game::TestImGui() const
{
	// Any application code here
	ImGui::Text("Hello, world!");

	static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
	ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));


	struct Funcs
	{
		static float Sin(void*, int i) { return sinf(i * 0.1f); }
		static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
	};

	static int func_type = 0;
	static int begin_i = 100, end_i = 1000;
	ImGui::Separator();
	ImGui::SetNextItemWidth(100);
	ImGui::Combo("func", &func_type, "Sin\0Saw\0");
	ImGui::SameLine();
	ImGui::DragIntRange2("Sample range", &begin_i, &end_i, 5, 0, 0, "Min: %d units", "Max: %d units");
	float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
	ImGui::PlotHistogram("Histogram", func, NULL, begin_i, end_i, NULL, -1.0f, 1.0f, ImVec2(0,80));

}

void Game::LoadGameAssets()
{
	//load in actor texture
	Actor::InitCharacterSheet(8, 8, 4096, 4096);

	
	//load in actors
	std::string test_name = std::string("Bob");
	IntVec2 test_uv_coord = IntVec2(1, 5);
	m_testActor = new Actor(this, test_name, test_uv_coord);

	std::string dumb_name = std::string("Ed");
	IntVec2 dumb_uv_coord = IntVec2(1, 3);
	m_dumbActor = new Actor(this, dumb_name, dumb_uv_coord);

	
	//set social roles for each actors
	SocialRole test_to_dumb_init = SocialRole::GenerateRandomSocialRoleInit();
	test_to_dumb_init.m_origin = m_testActor;
	test_to_dumb_init.m_towards = m_dumbActor;
	m_testActor->AddRelationship(test_to_dumb_init);
}

void Game::EddPerformsRandomAction()
{
	Emotion temp_emotion = Emotion::GenerateRandomEmotion();
	m_testActor->ApplyEmotion(temp_emotion);

	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;
}

void Game::EddPerformsAHopefulAction()
{
	Emotion hopeful_emotion;

	hopeful_emotion = Emotion::GenerateDecayEmotion();
	hopeful_emotion[EMOTION_HOPE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	hopeful_emotion[EMOTION_PLEASED] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	hopeful_emotion[EMOTION_POSITIVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	m_testActor->ApplyEmotion(hopeful_emotion);

	//TODO: need to update accordingly
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;

}

void Game::EddPerformsAFearfulAction()
{
	Emotion fearful_emotion;

	fearful_emotion = Emotion::GenerateDecayEmotion();
	fearful_emotion[EMOTION_FEAR] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	fearful_emotion[EMOTION_DISPLEASED] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	fearful_emotion[EMOTION_NEGATIVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	m_testActor->ApplyEmotion(fearful_emotion);

	//TODO: need to update accordingly
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;

}

void Game::EddPerformsALovableAction()
{
	Emotion fearful_emotion;

	fearful_emotion = Emotion::GenerateDecayEmotion();
	fearful_emotion[EMOTION_LOVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	fearful_emotion[EMOTION_LIKING] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	fearful_emotion[EMOTION_POSITIVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	m_testActor->ApplyEmotion(fearful_emotion);

	//TODO: need to update accordingly
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;

}

void Game::EddPerformsAHatefulAction()
{
	Emotion hateful_emotion;

	hateful_emotion = Emotion::GenerateDecayEmotion();
	hateful_emotion[EMOTION_HATE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	hateful_emotion[EMOTION_DISLIKING] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	hateful_emotion[EMOTION_NEGATIVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	m_testActor->ApplyEmotion(hateful_emotion);

	//TODO: need to update accordingly
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;

}

void Game::EddPerformsAnIntriguingAction()
{
	Emotion intriguing_emotion;

	intriguing_emotion = Emotion::GenerateDecayEmotion();
	intriguing_emotion[EMOTION_INTEREST] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	intriguing_emotion[EMOTION_LIKING] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	intriguing_emotion[EMOTION_POSITIVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	m_testActor->ApplyEmotion(intriguing_emotion);

	//TODO: need to update accordingly
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;

}

void Game::EddPerformsADisgustingAction()
{
	Emotion disgusting_emotion;

	disgusting_emotion = Emotion::GenerateDecayEmotion();
	disgusting_emotion[EMOTION_DISGUST] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	disgusting_emotion[EMOTION_DISLIKING] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	disgusting_emotion[EMOTION_NEGATIVE] = g_randomNumberGenerator.GetRandomFloatInRange(0.0f, 0.1f);
	m_testActor->ApplyEmotion(disgusting_emotion);

	//TODO: need to update accordingly
	SocialRole test_to_dumb_update = SocialRole::GenerateRandomSocialRole();
	test_to_dumb_update.m_origin = m_testActor;
	test_to_dumb_update.m_towards = m_dumbActor;
	m_testActor->UpdateRelationship(test_to_dumb_update);

	g_numActionsEdTook++;

}


void Game::UpdateEdsActions()
{
	std::string window_name = "Ed's Actions";

	Vec2 profile_size = Vec2(50, 50);
	bool display = true;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	const ImVec2 size = ImVec2(profile_size.x, profile_size.y);
	const float alpha_override = -1.0f;
	if (!ImGui::Begin(window_name.c_str(), &display, size, 
		alpha_override, window_flags))
	{
		ImGui::End();
		return;
	}

	if(ImGui::Button("Random Action"))
		EddPerformsRandomAction();

	if(ImGui::Button("Hopeful Action"))
		EddPerformsAHopefulAction();

	if(ImGui::Button("Fearful Action"))
		EddPerformsAFearfulAction();

	if(ImGui::Button("Lovable Action"))
		EddPerformsALovableAction();

	if(ImGui::Button("Hateful Action"))
		EddPerformsAHatefulAction();

	if(ImGui::Button("Intriguing Action"))
		EddPerformsAnIntriguingAction();

	if(ImGui::Button("Disgusting Action"))
		EddPerformsADisgustingAction();

	ImGui::End();
}
