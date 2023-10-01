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
	ball = App->textures->Load("Graphics/Ball2.png");
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
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->physics->angle > -90)
	{
		App->physics->angle--;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->physics->angle < 0)
	{

		App->physics->angle++;
		
	}


	


	
	App->renderer->Blit(ball, App->physics->x, App->physics->y, NULL, NULL, App->physics->spin, 19, 13);

	App->renderer->Blit(cannon, 0, App->physics->floor - 26, NULL, NULL, App->physics->angle, 51, 51);
	


	return UPDATE_CONTINUE;
}



