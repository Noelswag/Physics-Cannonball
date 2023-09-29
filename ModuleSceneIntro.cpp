#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	graphics = App->textures->Load("Graphics/Background.png");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	App->audio->PlayMusic("Sound/US3.ogg");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, 0, 0, NULL);
	

	return UPDATE_CONTINUE;
}
