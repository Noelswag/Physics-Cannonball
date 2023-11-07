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
	void applyWind();
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
	float surface;
	float density;
	float Cd;
	float Drag;


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
		int x;
		int y;
		int w;
		int h;

		double v;
		double vx;
		double vy;

		float ax;
		int ay;

		int m;

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
	
	class ball
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

		

		ball() {
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

		ball(int posX, int posY, int radius, int a, double vel, double velX, double velY, int accX, int accY, int mass) {
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

	void euler(double* x, double* v, double* a = nullptr);

	void eulerSympletic(double* x, double* v, double* a);


	void velocityVerlet(double* x, double* v, double* a);

	void bounceVertical()
	{
		vy = -vy * 0.75;
		vx = vx * 0.9;
	}

	void bounceHorizontal()
	{
		vx = -vx;
	}

private:

	
	int movOption = 0;
	bool debug;

public:
	ball bullet;
	player testCannon;
};