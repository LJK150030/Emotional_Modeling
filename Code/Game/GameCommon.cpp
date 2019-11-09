#include "Game/GameCommon.hpp"
#include "Engine/Tools/Profiler.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Renderer/DebugRender.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/ImGUISystem.hpp"
#include "Engine/Async/Dispatcher.hpp"
#include "Game/App.hpp"


RandomNumberGenerator g_randomNumberGenerator(32);
WindowContext* g_theWindow = nullptr;
RenderContext* g_theRenderer = nullptr;
EventSystem* g_theEventSystem = nullptr;
DevConsole* g_theDevConsole = nullptr;
DebugRender* g_theDebugRenderer = nullptr;
Clock* g_theClock = nullptr;
LogSystem* g_theLogSystem = nullptr;
AsyncRingBuffer* g_messageBuffer = nullptr;
ImGUISystem* g_imGUI = nullptr;
App* g_theApp = nullptr;

NamedStrings g_gameConfigBlackboard;
float g_startTime = 0.0f;
int g_numActionsEdTook = 1;

//--------------------------------------------------------------
// Dev Console Static functions'


STATIC bool DevConLog(EventArgs& args)
{
	std::string filter = args.GetValue(std::string("tag"), std::string("DevCons"));
	std::string message = args.GetValue(std::string("msg"), std::string("DevCons"));

	Logf(filter.c_str(), message.c_str());
	return false;
}


// log_enable_all
STATIC bool DevConLogEnableAll(EventArgs& args)
{
	UNUSED (args);
	LogEnableAll();
	return false;
}


// log_disable_all
STATIC bool DevConLogDisableAll(EventArgs& args)
{
	UNUSED (args);
	LogDisableAll();
	return false;
}


// log_enable
STATIC bool DevConLogEnable(EventArgs& args)
{
	std::string filter = args.GetValue(std::string("tag"), std::string("DevCons"));
	LogEnable(filter.c_str());
	return false;
}

// log_disable
STATIC bool DevConLogDisable(EventArgs& args)
{
	std::string filter = args.GetValue(std::string("tag"), std::string("DevCons"));
	LogDisable(filter.c_str());
	return false;
}


// log_thread_test
#define LOG_MESSAGES_PER_THREAD_TEST   (1024)
STATIC void LogTest( uint start_count ) 
{
	std::thread::id this_id = std::this_thread::get_id(); 
	size_t hash_id = std::hash<std::thread::id>{}( this_id );
	char const* format = "Thread[%llu]: Printing Message %u";

	for (unsigned int i = 0; i < LOG_MESSAGES_PER_THREAD_TEST; ++i) {
		const float coin_flip = g_randomNumberGenerator.GetRandomFloatZeroToOne();
		if (coin_flip > 0.5f)
		{
			LogCallstackf( "debug", format, hash_id, i ); 
		}
		else 
		{
			Logf( "debug", format, hash_id, i + start_count ); 
		}
	}
}


STATIC bool LogThreadedTest(EventArgs& args)
{
	// leave one thread free (main thread)
	UNUSED(args);
	uint core_count = std::thread::hardware_concurrency() - 1; 
	for (uint i = 0; i < core_count; ++i) {
		std::thread test_thread( LogTest, i * LOG_MESSAGES_PER_THREAD_TEST ); 
		test_thread.detach(); 
	}

	return true;
}


STATIC bool DevConsLogFlush(EventArgs& args)
{
	UNUSED(args);
	// leave one thread free (main thread)
	// 	Logf("PrintFilter", "I am a Logf call");
	// 	Logf("FlushFilter", "I am now calling flush");
	LogFlush();
	return true;
}


STATIC bool QuitRequest(EventArgs& args)
{
	UNUSED(args);
	g_theApp->HandleQuitRequested();
	return true;
}

STATIC bool PrintMemAlloc(EventArgs& args)
{
	UNUSED(args);
	DevConPrintMemTrack();
	return true;
}

STATIC bool LogMemAlloc(EventArgs& args)
{
	UNUSED(args);
	MemTrackLogLiveAllocations();
	return true;
}

//----------------------------------------------------------------
//Hooks

// log_hook_dev_console
void OutputLog(const LogMessageHeaderT* log_info)
{
	char filter[256];
	char message[256];

	memcpy(filter, log_info->filter, log_info->size_of_filter);
	memcpy(message, log_info->message, log_info->size_of_message);

	DebuggerPrintf("%s: %s \n", filter, message);
}

//--------------------------------------------------------------
// Engine start up, only asking for systems that I know I will use

// Engine
void EngineStartup() 
{
	g_theClock = new Clock(Clock::g_theMasterClock);

	g_theEventSystem = new EventSystem;
	g_theEventSystem->Startup();
	g_theEventSystem->SubscribeEventCallbackFunction("log", DevConLog);
	g_theEventSystem->SubscribeEventCallbackFunction("logEnbAll", DevConLogEnableAll);
	g_theEventSystem->SubscribeEventCallbackFunction("logDisAll", DevConLogDisableAll);
	g_theEventSystem->SubscribeEventCallbackFunction("logEnb", DevConLogEnable);
	g_theEventSystem->SubscribeEventCallbackFunction("logDis", DevConLogDisable);
	g_theEventSystem->SubscribeEventCallbackFunction("logTest", LogThreadedTest);
	g_theEventSystem->SubscribeEventCallbackFunction("logFlush", DevConsLogFlush);
	g_theEventSystem->SubscribeEventCallbackFunction("quit", QuitRequest);
	g_theEventSystem->SubscribeEventCallbackFunction("ShowMemAlloc", PrintMemAlloc);
	g_theEventSystem->SubscribeEventCallbackFunction("LogMemAlloc", LogMemAlloc);

	g_theRenderer->Startup();

	g_theDevConsole = new DevConsole;
	g_theDevConsole->Startup(g_theClock);
	
	g_theDebugRenderer = new DebugRender;
	g_theDebugRenderer->Startup(g_theRenderer);

	g_imGUI = new ImGUISystem(g_theRenderer);
	g_imGUI->Startup();
	
	Dispatcher::Init(7);
	LogSystemInit(LOG_DIR.c_str());
	LogEnableAll();
}

void EngineShutdown()
{
	// shutdown common
	g_theDebugRenderer->Shutdown();
	g_theDevConsole->ShutDown();
	g_theRenderer->Shutdown();
	g_theEventSystem->Shutdown();

	delete g_imGUI;
	g_imGUI = nullptr;

	delete g_theClock;
	g_theClock = nullptr;

	delete g_theDebugRenderer;
	g_theDebugRenderer = nullptr;

	delete g_theDevConsole;
	g_theDevConsole = nullptr;

	delete g_theEventSystem;
	g_theEventSystem = nullptr;

	//LogSystemShutdown(); 
}