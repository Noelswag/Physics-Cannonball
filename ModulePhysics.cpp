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
	x = 0;
	y = 0;
	vx = 50;
	vy = 0;
	a = 10;
	t = 0.16f;
	floor = 214;

	ball = App->textures->Load("Graphics/Ball.png");

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
	
	euler(&x, &vx);
	velocityVerlet(&y, &vy, &a);
	if (y >= floor)
	{
		y = floor;
		vx = 0;
		vy = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		x = 0;
		y = 0;
		vy = 0;
		vx = 50;
	}

	App->renderer->Blit(ball, x, y, NULL);

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}
