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
	
	
	float x;
	float y;
	float vx;
	float vy;
	float a;
	float t;
	int floor;

	void euler(float* x, float* v, float* a = nullptr)
	{
		*x += *v * t;
		if (a != nullptr)
		{
			*v += *a * t;
		}
	}

	void eulerSympletic(float* x, float* v, float* a)
	{
		*v += *a * t;
		*x += *v * t;
	}


	
	void velocityVerlet(float* x, float* v, float* a)
	{
		*x += (double)*v * t + (double)1 / 2 * *a * pow(t, 2);
		*v += *a * t;
	}

	SDL_Texture* ball = nullptr;

private:

	bool debug;
};