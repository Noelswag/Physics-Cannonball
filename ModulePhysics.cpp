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
	a = 1000;
	t = 0.016f;	
	mode = 1;
	floor = 300;
	x = 999;
	y = (double)floor-999;
	power = 350;
	angle = -45;

	pipe = App->audio->LoadFx("Sound/pipe.wav");
	scream = App->audio->LoadFx("Sound/Scream.wav");


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
		if (power < 600)
			power+=10;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		if (power > 100)
			power-=10;

	if (flying)
	{
		euler(&x, &vx);
		
		if (mode == 1)
			euler(&y, &vy, &a);
		if (mode == 2)
			eulerSympletic(&y, &vy, &a);
		if (mode == 3)
			velocityVerlet(&y, &vy, &a);

		spin += 10;
	}
	

	if (y >= floor && flying)
	{
		y = floor;
		flying = false;
		start = true;
		App->audio->PlayFx(pipe);
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
			y = (double)floor-13-75*sin(anglerad);
			spin = 0;
			App->audio->PlayFx(scream);
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

	displayx = x;
	displayy = -(y-floor);

	displayAngle = -angle;

	displayPower = (power / 10 - 10) * 2;
	
	
	
	sprintf_s(titletext, 200, "X:%03d Y:%03d Angle: %02d (left/right) Power: %02d (up/down) Mode: %s(1,2,3)", displayx, displayy, displayAngle, displayPower, modetext);

	App->window->SetTitle(titletext);
	

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}
