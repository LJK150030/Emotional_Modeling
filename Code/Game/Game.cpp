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
#include "Game/App.hpp"
#include "Game/Actor.hpp"
#include "Game/Action.hpp"
#include "Game/SocialRole.hpp"

STATIC bool LogAgentData(EventArgs& args)
{
	UNUSED(args);
	Actor* test_actor = g_theApp->GetTheGame()->GetTestActor();
	test_actor->LogData(g_theApp->GetTheGame()->GetPlayableActor());
	
	return false;
}

Game::Game()
{
	LoadGameAssets();

	g_theEventSystem->SubscribeEventCallbackFunction("logData", LogAgentData);
}


Game::~Game()
{
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
	for(int relationship_idx = 0; relationship_idx < g_validRelationships.size(); ++relationship_idx)
	{
		delete g_validRelationships[relationship_idx];
		g_validRelationships[relationship_idx] = nullptr;
	}
	g_validRelationships.clear();

	for(int action_idx = 0; action_idx < g_validActions.size(); ++action_idx)
	{
		delete g_validActions[action_idx];
		g_validActions[action_idx] = nullptr;
	}
	g_validActions.clear();

	delete m_testActor;
	m_testActor = nullptr;

	delete m_dumbActor;
	m_dumbActor = nullptr;

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

Actor* Game::GetTestActor()
{
	return m_testActor;
}

Actor* Game::GetPlayableActor()
{
	return m_dumbActor;
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
	InitActions();
	InitSocialRelations();
	InitActors();
	InitSocialRoles();
}


void Game::InitActors()
{
	std::string test_name = std::string("Bob");
	IntVec2 test_uv_coord = IntVec2(1, 5);
	m_testActor = new Actor(this, test_name, test_uv_coord);

	std::string dumb_name = std::string("Ed");
	IntVec2 dumb_uv_coord = IntVec2(1, 3);
	m_dumbActor = new Actor(this, dumb_name, dumb_uv_coord);
}


void Game::InitSocialRoles()
{
	
	//set social roles for each actors
	SocialRole test_to_dumb_init = SocialRole::GenerateRandomSocialRoleInit();
	test_to_dumb_init.m_origin = m_testActor;
	test_to_dumb_init.m_towards = m_dumbActor;
	m_testActor->AddRelationship(test_to_dumb_init);


	//two ways of going about this
	//comment out this code, to set Bob's relationship type with Ed as a stranger
	//otherwise uncomment this code means Bob already has a relationship with Ed
	m_testActor->DetermineRelationshipWith(m_dumbActor);
}


void Game::InitActions()
{
	g_validActions = std::vector<Action*>();
	
	g_validActions.push_back(new Action(this, std::string("Admonish"), 0.36f)); // warn or reprimand someone firmly.
	g_validActions.push_back(new Action(this, std::string("Agitate"), 0.22f)); // make (someone) troubled or nervous
	g_validActions.push_back(new Action(this, std::string("Alleviate"), 0.65f)); // make (suffering, deficiency, or a problem) less severe.
	g_validActions.push_back(new Action(this, std::string("Awkward"), 0.28f)); // causing embarrassment or inconvenience.
	g_validActions.push_back(new Action(this, std::string("Backpedal"), 0.4f)); // reverse one's previous opinion.
	g_validActions.push_back(new Action(this, std::string("Blank"), 0.5f)); // To forget.
	g_validActions.push_back(new Action(this, std::string("Brainstorm"), 0.6f)); // To think through.
	g_validActions.push_back(new Action(this, std::string("Compromise"), 0.55f)); // To work out a deal.
	g_validActions.push_back(new Action(this, std::string("Comradery"), 0.7f)); // the quality of affording easy familiarity and sociability
}

void Game::InitSocialRelations()
{
	g_validRelationships = std::vector<RelationshipType*>();

	g_validRelationships.push_back(new RelationshipType("Parent child", 0.7f, 0.8f, 0.6f, 0.7f));
	g_validRelationships.push_back(new RelationshipType("Child parent", 0.7f, 0.2f, 0.6f, 0.7f));
	g_validRelationships.push_back(new RelationshipType("Boss worker", 0.5f, 0.8f, 0.5f, 0.5f));
	g_validRelationships.push_back(new RelationshipType("stranger", 0.5f, 0.2f, 0.5f, 0.5f));
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

	m_testActor->DrawSocialRoleGuess(*m_dumbActor);
	
	//display all of the actions available to ed
	for(int action_idx = 0; action_idx < g_validActions.size(); ++action_idx)
	{
		if(ImGui::Button(g_validActions[action_idx]->GetName().c_str()))
		{
			// call event <ed, m_validActions[action_idx], Tom, certainty? >
			// I understand how to do the first three arguments, but calculating certainty may
			// come from the receiving actor not knowing what kind of social role this person is.
			// This can work, but will have to use the current social relationship, then update
			// the social relationship accordingly.
			
			m_testActor->ReactToAction(*g_validActions[action_idx], *m_dumbActor);
			g_numActionsEdTook++;
			g_socialRoleSelectedThisTurn = false;
		}
	}
	
	ImGui::End();
}
