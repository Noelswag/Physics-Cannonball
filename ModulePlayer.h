#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

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

	int cannonposX = 0;
	int cannonposY = 274;

	double v = 30;
	double vx = 3;
	double vy = 5;
	float jumpv = 10;
	float jumpa = 0;

	float ax = 2;
	int ay = 1;

	float m = 5;
	float force = 3;

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

	class player
	{
	public:
		int x;
		int y;
		int w;
		int h;

		double v;
		double vx;
		double vy;
		float jumpv;
		float jumpa;

		float ax;
		int ay;

		int m;
		float force;

		player() {
			x = 80;
			y = 560;
			w = 50;
			h = 80;
			v = 30;
			vx = 1;
			vy = 20;
			ax = 2;
			ay = 1;
			m = 5;

		}

		player(int posX, int posY, int width, int height, double vel, double velX, double velY, float accX, int accY, int mass) {
			x = posX;
			y = posY;
			w = width;
			h = height;
			v = vel;
			vx = velX;
			vy = velY;
			ax = accX;
			ay = accY;
			m = mass;
		}
	};

	class bullet
	{
	public:
		int x;
		int y;

		int r;

		double angle;

		double v;
		double vx;
		double vy;

		int ax;
		int ay;

		int m;



		bullet() {
			x = 80;
			y = 550;
			r = 20;
			angle = 30;
			v = 25;
			vx = v * cos(angle * DEGTORAD);
			vy = -v * sin(angle * DEGTORAD);
			ax = 0;
			ay = 1;
			m = 5;
		}

		bullet(int posX, int posY, int radius, int a, double vel, double velX, double velY, int accX, int accY, int mass) {
			x = posX;
			y = posY;
			r = radius;
			angle = a;
			v = vel;
			vx = velX;
			vy = velY;
			ax = accX;
			ay = accY;
			m = mass;
		}
	};
	

public:
	
private:
	int movOption = 0;
	int jumpOption = 7;
	int jumpVal = 0;
public:
	bool Euler = true;

	bullet bullet;
	player testCannon;
};