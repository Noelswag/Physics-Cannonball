#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

#define FRICTION 0.1f

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}


void ModulePhysics::euler(double* x, double* v, double* a)
{
	*x += *v * t;
	if (a != nullptr)
	{
		*v += *a * t;
	}
}

void ModulePhysics::eulerSympletic(double* x, double* v, double* a)

{
	*v += *a * t;
	*x += *v * t;
}

void ModulePhysics::velocityVerlet(double* x, double* v, double* a)
{
	*x += (*v * t + (double)1 / 2 * *a * pow(t, 2));
	*v += *a * t;
}

void ModulePhysics::applyWind(player* entity)
{
	entity->Drag = (density * entity->surface * wind * wind * Cd) / 3;
	entity->vx -= entity->Drag;

}

void ModulePhysics::bounceVertical(player* Entity)
{
	Entity->vy = -Entity->vy * 0.75;
	Entity->vx = Entity->vx * 0.9;
}

void ModulePhysics::bounceHorizontal(player* entity)
{
	entity->vx = -entity->vx;
}

void ModulePhysics::hydrodynamics(player* entity)
{
	entity->Fb = densityW * entity->ay * entity->m;
	entity->vy -= entity->Fb / 100;
}


bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	start = true;
	flying = false;
	end = false;
	
	t = 0.016f;	
	mode = 1;
	floor = 300;
	
	power = 350;
	angle = -45;

	wind = 0.8f;
	density = 0.7f;
	densityW = 1.0f;
	Cd = 0.47f;

	bullet.Drag = 0;
	bullet.ay = 1000;
	bullet.x = 999;
	bullet.y = (double)floor - 999;
	bullet.surface = 10.0f;
	bullet.m = 10;
	bullet.DragWater = 0;
	bullet.Fb = 0;

	bonk = App->audio->LoadFx("Sound/bonk.wav");
	boom = App->audio->LoadFx("Sound/boom.wav");


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
		euler(&bullet.x, &bullet.vx);
		applyWind(&bullet);
		
		if (mode == 1)
			euler(&bullet.y, &bullet.vy, &bullet.ay);
		if (mode == 2)
			eulerSympletic(&bullet.y, &bullet.vy, &bullet.ay);
		if (mode == 3)
			velocityVerlet(&bullet.y, &bullet.vy, &bullet.ay);

		spin += 10;
	}
	
	
	totalvelocity = sqrt(pow(bullet.vx, 2) + pow(bullet.vy, 2));
	
	if (bullet.y >= floor && flying && bullet.x >= 400)
	{
		bullet.y = floor;
		bounceVertical(&bullet);
		App->audio->PlayFx(bonk);
		if ((bullet.x > (double)SCREEN_WIDTH || bullet.x < -25 || totalvelocity<125) && flying)
		{
			flying = false;
			start = true;
		}
	}
	
	if (bullet.y >= floor && bullet.x < 400 && flying)
	{
		hydrodynamics(&bullet);
		
	}

	
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (start)
		{
			start = false;
			flying = true;
			
			anglerad = -angle * M_PI / 180;
			bullet.vx = cos(anglerad)*power;
			bullet.vy = -sin(anglerad) * power;

			bullet.x = (Cannon.x+13)+75*cos(anglerad);
			bullet.y = (double)Cannon.y-75*sin(anglerad);
			spin = 0;
			App->audio->PlayFx(boom);
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


	if (App->player->front)
	{
		displayAngle = -angle;
	}
	else
	{
		displayAngle = (-180 - angle);
	}

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
