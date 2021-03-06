#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/DebugRender.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/WindowContext.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Memory/Mem.hpp"

STATIC bool QuitRequest(EventArgs& args)
{
	UNUSED(args);
	g_theApp->HandleQuitRequested();
	return true;
}

App::App(): m_theGame(nullptr)
{
	ParseXmlFileToNamedString(g_gameConfigBlackboard, "Data/GameConfig.xml");
}

App::~App()
{
	delete m_theGame;
	m_theGame = nullptr;

	delete m_devCamera;
	m_devCamera = nullptr;
}

void App::Startup()
{
	EngineStartup();
	g_theWindow->SetMouseMode(MOUSE_MODE_ABSOLUTE);

	//dev console camera
	m_devCamera = new Camera();
	m_devCamera->SetColorTarget(nullptr);
	m_devCamera->SetOrthoView( Vec2(0.0f, 0.0),	Vec2((WORLD_HEIGHT * WORLD_ASPECT), (WORLD_HEIGHT)) );

	g_theEventSystem->SubscribeEventCallbackFunction("quit", QuitRequest);
	
	m_theGame = new Game;
	m_theGame->Startup();

	//DevConPrintMemTrackType();
}

void App::Shutdown()
{
	m_theGame->Shutdown();
	EngineShutdown();
}

void App::RunFrame()
{
	BeginFrame();
	Update();
	Render();
	EndFrame();
}

void App::BeginFrame() const
{
	g_theRenderer->BeginFrame();
	g_theEventSystem->BeginFrame();
	g_theDevConsole->BeginFrame();
	g_theDebugRenderer->BeginFrame();
}

void App::Update()
{
	const double current_time = GetCurrentTimeSeconds();
	//double delta_seconds = ClampDouble(current_time - m_timeLastFrame, 0.0, 0.1);
	double delta_seconds = current_time - m_timeLastFrame;;
	m_timeLastFrame = current_time;
	

	g_theClock->Step(delta_seconds);
	g_theDevConsole->Update(g_theClock->m_frameTime);
	m_theGame->Update(g_theClock->m_frameTime);
}

void App::Render() const
{
	// Draw a line from the bottom-left corner of the screen (0,0) to the center of the screen (50,50)
	m_theGame->Render();
	g_theDebugRenderer->RenderToScreen();

	m_theGame->UpdateImGUI();

	if(DEV_CONSOLE_IN_USE)
	{
		m_devCamera->SetColorTarget(g_theRenderer->GetFrameColorTarget());

		m_devCamera->SetModelMatrix( Matrix44::IDENTITY );
		g_theRenderer->BeginCamera(m_devCamera);
		g_theRenderer->ClearDepthStencilTarget( 1.0f );

		g_theDevConsole->Render(g_theRenderer, *m_devCamera);

		g_theRenderer->EndCamera(m_devCamera);
	}
}

void App::EndFrame() const
{
	// "Present" the back buffer by swapping the front (visible) and back (working) screen buffers
	g_theRenderer->EndFrame();
	g_theEventSystem->EndFrame();
	g_theDevConsole->EndFrame();
	g_theDebugRenderer->EndFrame();
}


bool App::HandleQuitRequested()
{
	m_isQuitting = true;
	return true;
}

void App::HardRestart()
{
	Shutdown();
	delete m_theGame;
	m_theGame = nullptr;
	Startup();
}

Game* App::GetTheGame()
{
	return m_theGame;
}

bool App::HandleKeyPressed(const unsigned char key_code)
{
	switch (key_code)
	{
	case ESC_KEY:
		if (!DEV_CONSOLE_IN_USE)
			m_isQuitting = true;
		return true;

	case T_KEY:
		if (!DEV_CONSOLE_IN_USE)
			m_isSlowMo = true;
		return true;

	case P_KEY:
		if (!DEV_CONSOLE_IN_USE)
			m_isPaused = true;
		return true;

	case F8_KEY:
		if (!DEV_CONSOLE_IN_USE)
			HardRestart();
		return true;

	case TILDE_KEY:
		DEV_CONSOLE_IN_USE = !DEV_CONSOLE_IN_USE;
		return true;

	default:
		if (!DEV_CONSOLE_IN_USE)
			return m_theGame->HandleKeyPressed(key_code);

		return false;
	}
}

bool App::HandleKeyReleased(const unsigned char key_code)
{
	switch (key_code)
	{
	case T_KEY:
		m_isSlowMo = false;
		return true;

	case P_KEY:
		m_isPaused = false;
		return true;

	default:
		return m_theGame->HandleKeyReleased(key_code);
	}
}