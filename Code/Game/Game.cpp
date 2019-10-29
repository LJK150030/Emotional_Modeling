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


Game::Game()
{
	LoadGameAssets();
}


Game::~Game()
{
	delete m_testActor;
	m_testActor = nullptr;
}

void Game::Startup()
{	
	LogHook(&OutputLog);

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


void Game::UpdateImGUI() const
{

	// Feed inputs to dear imgui, start new frame
	g_imGUI->BeginFrame();
	ImGui::NewFrame();

	//TestImGui();

	m_testActor->Render();

	
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
	std::string temp_char_name = std::string("Bob");
	IntVec2 temp_char_coord = IntVec2(1, 5);

	Actor::InitCharacterSheet(8, 8, 4096, 4096);
	m_testActor = new Actor(this, temp_char_name, temp_char_coord);
}
