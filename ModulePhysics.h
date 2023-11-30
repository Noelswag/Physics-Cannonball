#pragma once
#include "Module.h"
#include "Globals.h"

#define GRAVITY_ 10

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	
	double x;
	double y;
	double vx;
	double vy;
	double totalvelocity;
	double a;
	double t;
	int power;
	int floor;
	bool start;
	bool flying;
	bool inWater;
	bool end;
	int mode;
	double angle;
	double anglerad;
	char* modetext;
	char titletext[200] = { "\0" };
	int displayAngle;
	int displayx;
	int displayy;
	int displayPower;
	int spin;
	uint bonk = 0;
	uint boom = 0;

	float windx;
	float windy;
	float density;
	float densityW;
	
	

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
	movementOptions test = movementOptions::MOVX;
	class player
	{
	public:
		double x;
		double y;
		double w;
		double h;

		double v;
		double vx;
		double vy;

		double ax;
		double ay;

		float jumpv;
		float jumpa;
		float force;

		float Fx = 0;
		float Fy = 0;

		int m;
		float surface;
		float volumen;
		float Cd;

		float dragW = 1.0f;

		player() {

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

	void euler(player* entity);

	void eulerSympletic(player* entity);


	void velocityVerlet(player* entity);

	void bounceVertical(player* Entity);
	
	void bounceHorizontal(player* entity);

	void applyGravity(player* entity);

	void applyWind(player* entity);

	void applyHydrodynamics(player* entity);

	void applyAerodynamics(player* entity);

	void resetForces(player* entity);
	

private:

	
	int movOption = 0;
	bool debug;

public:
	player bullet;
	player testCannon;
	player Cannon;
	player airdrop;
};