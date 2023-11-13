#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

struct Object
{
	SDL_Texture* graphic;
	uint fx;

	Object() : graphic(NULL)
	{}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();
	bool Start();
	update_status Update();
	bool CleanUp();
	SDL_Texture* cannon = nullptr;

	SDL_Texture* ball = nullptr;

	bool front = true;

	enum movementOptions
	{
		MOVX,
		FIXVEL,
		FIXACC,
		MOMENTUM,
		IMPULSE,
		ACCELERATION,
		FORCE
	};
	enum jumpOptions
	{
		JUMPX,
		JUMPVEL,
		JUMPACC,
		JUMP_MOMENTUM,
		JUMP_IMPULSE,
		JUMP_ACCELERATION,
		JUMP_FORCE,
		NO_JUMP
	};
	enum playerStatus
	{
		STOP_PLAYER,
		GRAVITY
	};
	movementOptions test = movementOptions::MOVX;
	jumpOptions testJump = jumpOptions::NO_JUMP;
	playerStatus testPlayer = playerStatus::GRAVITY;
	

public:
	
private:
	int movOption = 0;
	int jumpOption = 7;
	int jumpVal = 0;
public:
	bool Euler = true;

	
};