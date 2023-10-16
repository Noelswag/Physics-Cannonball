#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	cannon = App->textures->Load("Graphics/Cannon.png");
	ball = App->textures->Load("Graphics/Ball.png");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{


		if (front && App->physics->angle > -90)
		{
			App->physics->angle--;

		}

		if (!front && App->physics->angle > -180)
		{
			App->physics->angle--;

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (front && App->physics->angle < 0)
		{
			App->physics->angle++;
		}
		
		if (!front && App->physics->angle < -90)
		{
			App->physics->angle++;
		}
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0)
	{

		if (!front)
		{
			front = true;
			App->physics->angle = -180 - App->physics->angle;
		}
		if (cannonpos < SCREEN_WIDTH - 101)
		{
		cannonpos++;
		}

	}
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0)
	{

		if (front)
		{
			front = false;
			App->physics->angle = -180 - App->physics->angle;
		}
		if (cannonpos > 0)
		{
			cannonpos--;
		}
	}


	
	App->renderer->Blit(ball, App->physics->x, App->physics->y, NULL);
	if (front)
	{
		App->renderer->Blit(cannon, cannonpos, App->physics->floor - 26, NULL, NULL, App->physics->angle, 51, 51);
	}
	else
	{
		App->renderer->BlitMirror(cannon, cannonpos-51, App->physics->floor - 26, NULL, NULL, (App->physics->angle - 180), 149, 51);
	}


	return UPDATE_CONTINUE;
}



