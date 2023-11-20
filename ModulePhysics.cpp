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


void ModulePhysics::euler(player* entity)
{
	entity->ax = entity->Fx / entity->m;
	entity->ay = entity->Fy / entity->m;
	
	entity->x += PIXEL_TO_METERS(entity->vx);
	entity->vx += entity->ax;

	entity->y += PIXEL_TO_METERS(entity->vy);
	entity->vy += entity->ay;
	
	
}

void ModulePhysics::eulerSympletic(player* entity)

{
	entity->ax = entity->Fx / entity->m;
	entity->ay = entity->Fy / entity->m;

	entity->vx += entity->ax * t;
	entity->x += entity->vx * t;
	
	entity->vy += entity->ay * t;
	entity->y += entity->vy * t;
	
}

void ModulePhysics::velocityVerlet(player* entity)
{

	entity->ax = entity->Fx / entity->m;
	entity->ay = entity->Fy / entity->m;

	entity->x += (entity->vx * t + (double)1 / 2 * entity->ax * pow(t, 2));
	entity->vx += entity->ax * t;

	entity->y += (entity->vy * t + (double)1 / 2 * entity->ay * pow(t, 2));
	entity->vy += entity->ay * t;
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



void ModulePhysics::applyHydrodynamics(player* entity)
{
	entity->Fy -= densityW * GRAVITY_ * entity->volumen;
	entity->Fy -= dragW * entity->vy;
}

void ModulePhysics::applyWind(player* entity)
{
	entity->Fx += -(density * entity->surface * windx * windx * entity->Cd) / 2;

}

void ModulePhysics::applyGravity(player* entity)
{
	entity->Fy = entity->m * GRAVITY_;
}

void ModulePhysics::applyAerodynamics(player* entity)
{
	entity->Fy += (density * entity->surface * windy * windy * entity->Cd) / 2;
}

void ModulePhysics::resetForces(player* entity)
{
	applyGravity(entity);
	
}



bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	start = true;
	flying = false;
	inWater = false;
	end = false;
	
	t = 0.016f;	
	mode = 1;
	floor = 300;
	
	power = 350;
	angle = -45;

	windx = 0.0f;
	windy = -1.0f;
	density = 0.7f;
	densityW = 1.0f;

	bullet.Cd = 0.47f;
	bullet.x = 999;
	bullet.y = (double)floor - 999;
	bullet.surface = 10.0f;
	bullet.volumen = 100.0f;
	bullet.m = 50;

	applyGravity(&bullet);
	applyAerodynamics(&bullet);

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

	
	totalvelocity = sqrt(pow(bullet.vx, 2) + pow(bullet.vy, 2));
	
	if (bullet.y >= floor && flying  && bullet.x >= 400)
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
		applyHydrodynamics(&bullet);
		
		
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

	if (flying)
	{

		applyWind(&bullet);
		
		if (mode == 1)
			euler(&bullet);
		if (mode == 2)
			eulerSympletic(&bullet);
		if (mode == 3)
			velocityVerlet(&bullet);

		spin += 10;
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

	
	resetForces(&bullet);

	return UPDATE_CONTINUE;
}




// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}
