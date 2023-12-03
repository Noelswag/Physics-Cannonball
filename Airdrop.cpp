#include "Globals.h"
#include "Application.h"
#include "Airdrop.h"

Airdrop::Airdrop(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Airdrop::~Airdrop()
{}

// Load assets
bool Airdrop::Start()
{
	App->physics->airdrop.x = 100;
	App->physics->airdrop.y = 10;
	App->physics->airdrop.m = 30;
	App->physics->airdrop.surface = 500;
	App->physics->airdrop.volumen = 200.0f;
	App->physics->airdrop.Cd = 1.5f;

	App->physics->applyGravity(&App->physics->airdrop);
	App->physics->applyAerodynamics(&App->physics->airdrop);

	box = App->textures->Load("Graphics/airdrop.png");
	return true;
}

update_status Airdrop::Update()
{

	if (App->physics->airdrop.y < 265) {
		if (App->physics->mode == 1)
			App->physics->euler(&App->physics->airdrop);
		if (App->physics->mode == 2)
			App->physics->eulerSympletic(&App->physics->airdrop);
		if (App->physics->mode == 3)
			App->physics->velocityVerlet(&App->physics->airdrop);
	}
	



	//App->renderer->Blit(box, App->physics->airdrop.x, App->physics->airdrop.y, NULL);

	return UPDATE_CONTINUE;
}

bool Airdrop::CleanUp()
{

	return true;
}