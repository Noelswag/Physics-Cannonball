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
	uint pipe = 0;
	uint scream = 0;

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

	bool debug;
};