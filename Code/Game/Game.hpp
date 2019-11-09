#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Renderer/RenderContext.hpp"

class Camera;
class Shader;
class GPUMesh;
class Material;
class Actor;

class Game
{
public:
	bool m_inDevMode;

public:
	Game();
	~Game();
	
	void Startup();
	void Update(double delta_seconds);
	void UpdateImGUI();
	void Render() const;
	void Shutdown();
	bool HandleKeyPressed(unsigned char key_code);
	bool HandleKeyReleased(unsigned char key_code);
	void GarbageCollection() const;

	Actor* GetTestActor();
	Actor* GetPlayableActor();

private:
	
	Camera* m_gameCamera = nullptr;

 	Shader* m_defaultShader = nullptr;

	float m_time = 0.0f;
	float m_cameraTime = 0.0f;
	int m_currentFrame = 0;

	Vec3 m_camPosition		= Vec3(0.0f, 0.0f, -10.0f); // we'll start 10 back; 
	Vec3 m_camEuler			= Vec3(0.0f, 0.0f, 0.0f); 
	float m_camFOVDegrees	= 90.0f;

	Actor* m_testActor = nullptr;
	Actor* m_dumbActor = nullptr;

private:
	void TestImGui() const;

	void LoadGameAssets();

	void EddPerformsRandomAction();

	// debug code: stating from the child and bubbling back up
	// social role updating is still random, but because this
	// is debug code, I did not implement their proper functionality
	void EddPerformsAHopefulAction();
	void EddPerformsAFearfulAction();
	void EddPerformsALovableAction();
	void EddPerformsAHatefulAction();
	void EddPerformsAnIntriguingAction();
	void EddPerformsADisgustingAction();
	
	void UpdateEdsActions();
};
