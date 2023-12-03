#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"




class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();
	bool Start();
	update_status Update();
	bool CleanUp();
	void ModulePlayer2::OnCollision(Collider* c1, Collider* c2);
	SDL_Texture* cannon2 = nullptr;

	SDL_Texture* ball2 = nullptr;

	bool front2 = true;

	int waitForDmg2 = 0;
	bool canDmg2 = true;

	float hydroVar2 = 3.0f;

	bool debugCollider = false;

	Collider* player2Collider;
	SDL_Rect Player2Rect;

	Collider* bullet2Collider;
	SDL_Rect bullet2Rect;

	float life2 = 100;
	float dmg2 = 20;

	SDL_Rect lifeBar2;

	enum movement2Options
	{
		MOVX,
		FIXVEL,
		FIXACC,
		MOMENTUM,
		IMPULSE,
		ACCELERATION,
		FORCE
	};
	enum jump2Options
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
	enum player2Status
	{
		STOP_PLAYER,
		GRAVITY
	};
	movement2Options test2 = movement2Options::MOVX;
	jump2Options testJump2 = jump2Options::NO_JUMP;
	player2Status testPlayer2 = player2Status::STOP_PLAYER;


public:

private:
	
public:
	int movOption2 = 0;
	int jumpOption2 = 7;
	int jumpVal2 = 0;

	bool Euler = true;


};
