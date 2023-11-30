#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

struct SDL_Texture;



class Airdrop : public Module
{
private:
	

public:
	Airdrop(Application* app, bool start_enabled = true);
	virtual ~Airdrop();
	bool Start();
	update_status Update();
	bool CleanUp();

	SDL_Texture* box = nullptr;


};
