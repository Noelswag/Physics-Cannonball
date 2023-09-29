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
	int displayangle;
	int displayx;
	int displayy;
	uint pipe = 0;


	void euler(double* x, double* v, double* a = nullptr)
	{
		*x += *v * t;
		if (a != nullptr)
		{
			*v += *a * t;
		}
	}

	void eulerSympletic(double* x, double* v, double* a)
	{
		*v += *a * t;
		*x += *v * t;
	}



	void velocityVerlet(double* x, double* v, double* a)
	{
		*x += (*v * t + (double)1 / 2 * *a * pow(t, 2));
		*v += *a * t;
	}


private:

	bool debug;
};