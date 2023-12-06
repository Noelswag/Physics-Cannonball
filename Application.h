#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "Airdrop.h"
#include "ModuleCollisions.h"
#include "PerfTimer.h"

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModulePlayer2* player2;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
	Airdrop* airdropModule;
	ModuleCollisions* collisions;

private:

	p2List<Module*> list_modules;

	PerfTimer frameTime;

	uint32 maxFrameDuration = 16;
	float maxDtDuration = 4;
	float TotalDt = 0;

public:

	Application();
	~Application();

	bool Init();
	void PrepareUpdate();
	update_status Update();
	void FinishUpdate();
	bool CleanUp();

	bool VariableFrame = true;

private:

	void AddModule(Module* mod);
};