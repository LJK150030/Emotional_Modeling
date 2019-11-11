#pragma once
#include "Engine/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"

#define MAX_HISTORY 128
#define MIN_UNIT_VALUE 0.0f
#define MAX_UNIT_VALUE 1.0f
#define DEMO_NUM_INTERACTIONS 24
#define EMOTION_DECAY_VALUE -0.0001f

//--------------------------------------------------------
// Forward declirations
struct Rgba;
struct Vec2;
class App;

//---------------------------------------------------------
// global game objects

extern App* g_theApp;

//----------------------------------------------------------
// global game variables
//Camera global variables
constexpr float WORLD_ASPECT = 1.7777777777777778f;
constexpr float WORLD_WIDTH = 71.1111111110f;
constexpr float WORLD_HEIGHT = 40.0f;
constexpr float WORLD_CENTER_X = WORLD_WIDTH / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_HEIGHT / 2.f;

// key codes
constexpr int SHIFT_KEY = 16;
constexpr int ESC_KEY = 27;
constexpr int SPACE_BAR = 32;
constexpr int LEFT_ARROW = 37;
constexpr int UP_ARROW = 38;
constexpr int RIGHT_ARROW = 39;
constexpr int DOWN_ARROW = 40;
constexpr int NUM_0_KEY = 48;
constexpr int NUM_1_KEY = 49;
constexpr int NUM_2_KEY = 50;
constexpr int NUM_3_KEY = 51;
constexpr int NUM_4_KEY = 52;
constexpr int NUM_5_KEY = 53;
constexpr int NUM_6_KEY = 54;
constexpr int NUM_7_KEY = 55;
constexpr int NUM_8_KEY = 56;
constexpr int NUM_9_KEY = 57;
constexpr int A_KEY = 65;
constexpr int B_KEY = 66;
constexpr int C_KEY = 67;
constexpr int D_KEY = 68;
constexpr int E_KEY = 69;
constexpr int F_KEY = 70;
constexpr int G_KEY = 71;
constexpr int H_KEY = 72;
constexpr int I_KEY = 73;
constexpr int J_KEY = 74;
constexpr int K_KEY = 75;
constexpr int L_KEY = 76;
constexpr int M_KEY = 77;
constexpr int N_KEY = 78;
constexpr int O_KEY = 79;
constexpr int P_KEY = 80;
constexpr int Q_KEY = 81;
constexpr int R_KEY = 82;
constexpr int S_KEY = 83;
constexpr int T_KEY = 84;
constexpr int U_KEY = 85;
constexpr int V_KEY = 86;
constexpr int W_KEY = 87;
constexpr int X_KEY = 88;
constexpr int Y_KEY = 89;
constexpr int Z_KEY = 90;
constexpr int F1_KEY = 112;
constexpr int F2_KEY = 113;
constexpr int F8_KEY = 119;
constexpr int TILDE_KEY = 192;

extern float g_startTime;
extern int g_numActionsEdTook;
//---------------------------------------------------------
// global helper game functions

void EngineStartup();
void EngineShutdown();

//---------------------------------------------------------
// Dev console functions

static bool DevConLog(EventArgs& args);
static bool DevConLogEnableAll(EventArgs& args);
static bool DevConLogDisableAll(EventArgs& args);
static bool DevConLogEnable(EventArgs& args);
static bool DevConLogDisable(EventArgs& args);
static bool DevConsLogFlush(EventArgs& args);
static bool QuitRequest(EventArgs& args);
static bool PrintMemAlloc(EventArgs& args);
static bool LogMemAlloc(EventArgs& args);
static bool LogAgentData(EventArgs& args);

//---------------------------------------------------------
// Hooks
void OutputLog(const LogMessageHeaderT* log_info);

//---------------------------------------------------------
// Unit tests
static void LogTest( uint start_count );
static bool LogThreadedTest(EventArgs& args);
