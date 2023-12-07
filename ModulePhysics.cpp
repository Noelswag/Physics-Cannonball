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
	entity->x += PIXEL_TO_METERS(entity->vx);
	
	entity->vy += entity->ay * t;
	entity->y += PIXEL_TO_METERS(entity->vy);
	
}

void ModulePhysics::velocityVerlet(player* entity)
{

	entity->ax = entity->Fx / entity->m;
	entity->ay = entity->Fy / entity->m;

	entity->x += PIXEL_TO_METERS(entity->vx + (double)1 / 2 * entity->ax);
	entity->vx += entity->ax * t;

	entity->y += PIXEL_TO_METERS(entity->vy + (double)1 / 2 * entity->ay);
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
	entity->Fy -= entity->dragW * entity->vy;
}

void ModulePhysics::applyWind(player* entity)
{
	entity->Fx += (density * entity->surface * windx * windx * entity->Cd) / 2;

}

void ModulePhysics::applyGravity(player* entity)
{
	entity->Fy = entity->m * GRAVITY_;
}

void ModulePhysics::applyAerodynamics(player* entity)
{
	entity->Fy -= (density * entity->surface * windy * windy * entity->Cd) / 2;
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
	start2 = true;
	flying2 = false;
	inWater = false;
	end = false;
	
	t = 0.016f;	
	mode = 1;
	floor = 312;
	
	power = 350;
	angle = -45;

	power2 = 350;
	angle2 = -45;

	windx = 1.0f;
	windy = -1.0f;
	density = 0.7f;
	densityW = 1.0f;

	bullet.Cd = 0.47f;
	bullet.x = 999;
	bullet.y = (double)floor - 999;
	bullet.surface = 25.0f;
	bullet.volumen = 100.0f;
	bullet.m = 50;

	bullet2.Cd = 0.47f;
	bullet2.x = 999;
	bullet2.y = (double)floor - 999;
	bullet2.surface = 25.0f;
	bullet2.volumen = 100.0f;
	bullet2.m = 50;

	applyGravity(&bullet);
	applyAerodynamics(&bullet);

	applyGravity(&bullet2);
	applyAerodynamics(&bullet2);

	bonk = App->audio->LoadFx("Sound/bonk.wav");
	boom = App->audio->LoadFx("Sound/boom.wav");

	gravity = App->textures->Load("Graphics/GravityCartel.png");
	wind = App->textures->Load("Graphics/WindCartel.png");
	buoyancy = App->textures->Load("Graphics/BuoyancyCartel.png");
	framerate = App->textures->Load("Graphics/FramerateCartel.png");
	r1 = App->textures->Load("Graphics/1.png");
	r2 = App->textures->Load("Graphics/2.png");
	r3 = App->textures->Load("Graphics/3.png");
	r4 = App->textures->Load("Graphics/4.png");
	r5 = App->textures->Load("Graphics/5.png");
	r6 = App->textures->Load("Graphics/6.png");
	active = App->textures->Load("Graphics/Active.png");
	inactive = App->textures->Load("Graphics/InActive.png");
	move = App->textures->Load("Graphics/MovementCartel.png");
	jump = App->textures->Load("Graphics/JumpCartel.png");


	

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

	// Change Framerate option
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (App->VariableFrame == true) {
			App->VariableFrame = false;
		}
		else {
			App->VariableFrame = true;
		}

	}

	// Enable / Disable Forces
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		windActive = !windActive;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		gravityActive = !gravityActive;
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		buoyancyActive = !buoyancyActive;
	}

	// Draw Debug Modes in Screen

	App->renderer->Blit(framerate, 10, 10, NULL);
	App->renderer->Blit(gravity, 10, 30, NULL);
	App->renderer->Blit(wind, 10, 50, NULL);
	App->renderer->Blit(buoyancy, 10, 70, NULL);
	App->renderer->Blit(move, 10, 90, NULL);
	App->renderer->Blit(jump, 10, 110, NULL);


	if (App->VariableFrame == true) {
		App->renderer->Blit(r1, 107, 10, NULL);
	}
	else {
		App->renderer->Blit(r2, 107, 10, NULL);
	}

	if (gravityActive) {
		App->renderer->Blit(active, 97, 30, NULL);
	}
	else {
		App->renderer->Blit(inactive, 97, 30, NULL);
	}

	if (windActive) {
		App->renderer->Blit(active, 97, 50, NULL);
	}
	else {
		App->renderer->Blit(inactive, 97, 50, NULL);
	}

	if (buoyancyActive) {
		App->renderer->Blit(active, 97, 70, NULL);
	}
	else {
		App->renderer->Blit(inactive, 97, 70, NULL);
	}

	
	

	return UPDATE_CONTINUE;
}




// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}
