#pragma once
#include "Module.h"
#include "Globals.h"

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

	float wind;
	float density;
	float densityW;
	float Cd;


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

		float jumpv;
		float jumpa;
		float force;

		double ax;
		double ay;

		int m;
		float surface;
		float Drag;
		float DragWater;
		float Fb;

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

	void euler(double* x, double* v, double* a = nullptr);

	void eulerSympletic(double* x, double* v, double* a);


	void velocityVerlet(double* x, double* v, double* a);

	void bounceVertical(player* Entity);
	

	void bounceHorizontal(player* entity);

	void applyWind(player* entity);

	void hydrodynamics(player* entity);
	

private:

	
	int movOption = 0;
	bool debug;

public:
	player bullet;
	player testCannon;
	player Cannon;
};