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
	void ModulePlayer::OnCollision(Collider* c1, Collider* c2);
	// Collision Resolution
	void CollisionResolutionPlayer(SDL_Rect& r);
	// Collide
	bool Collide(SDL_Rect& r, SDL_Rect& r2);

	SDL_Texture* cannon = nullptr;

	SDL_Texture* ball = nullptr;

	SDL_Texture* p2wins = nullptr;

	bool front = true;

	int waitForDmg = 0;
	bool canDmg = true;

	float hydroVar = 3.0f;

	bool debugCollider = false;

	Collider* playerCollider;
	SDL_Rect PlayerRect;

	Collider* bulletCollider;
	SDL_Rect bulletRect;

	float life = 100;
	float dmg = 20;

	SDL_Rect lifeBar;


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