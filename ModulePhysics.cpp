#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	start = true;
	flying = false;
	end = false;
	a = 10;
	t = 0.16f;	
	mode = 1;
	floor = 214;
	x = 26;
	y = floor - 26;
	power = 35;
	angle = -45;



	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;
	
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		mode = 1;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		mode = 2;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		mode = 3;


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		if (power < 60)
			power++;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		if (power > 10)
			power--;

	if (flying)
	{
		euler(&x, &vx);
		if (mode == 1)
			euler(&y, &vy, &a);
		if (mode == 2)
			eulerSympletic(&y, &vy, &a);
		if (mode == 3)
			velocityVerlet(&y, &vy, &a);
	}
	
	
	if (y >= floor && flying)
	{
		y = floor;
		flying = false;
		start = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (start)
		{
			start = false;
			flying = true;
			
			anglerad = -angle * M_PI / 180;
			vx = cos(anglerad)*power;
			vy = -sin(anglerad) * power;

			x = 13+75*cos(anglerad);
			y = floor-13-75*sin(anglerad);

			//Offsets so that the ball comes out off the cannon. Uncomment the lines below for 0,0
			/*
			x = 0;
			y = floor;
			*/
		}
	}


	if (mode == 1)
		modetext = "Euler";

	if (mode == 2)
		modetext = "Sympletic Euler";
	
	if (mode == 3)
		modetext = "Velocity Verlet";

	

	displayangle = -angle;
	sprintf_s(titletext, 800, "Angle: %d (left/right) Power: %d (up/down) Mode: %s (1,2,3)", displayangle, power, modetext);

	App->window->SetTitle(titletext);
	

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}
