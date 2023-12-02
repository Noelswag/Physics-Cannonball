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

	rectCollider.x = 100;
	rectCollider.y = 100;
	rectCollider.w = 100;
	rectCollider.h = 100;

	testCollider = App->collisions->AddCollider(rectCollider, Collider::Type::PLAYER1);

	//App->renderer->DrawQuad({ 100,100,100,100 }, 255, 0, 0, 80, true, true);

	
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
	
	App->renderer->DrawCircle(100, 100, 50, 255, 0, 0, 255);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debugCollider = !debugCollider;
	}

	if (debugCollider) {
		App->renderer->DrawQuad(rectCollider, 255, 0, 0, 255);
	}

	return UPDATE_CONTINUE;
}
