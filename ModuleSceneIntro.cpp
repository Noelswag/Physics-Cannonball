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
	graphics = App->textures->Load("Graphics/mapa_worms_fisica.png");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	App->audio->PlayMusic("Sound/US3.ogg");

	block1rect.x = 0;
	block1rect.y = 340;
	block1rect.w = 120;
	block1rect.h = 100;

	block1 = App->collisions->AddCollider(block1rect, Collider::Type::SOLID_BODY, this);

	block2rect.x = 50;
	block2rect.y = 210;
	block2rect.w = 60;
	block2rect.h = 140;

	block2 = App->collisions->AddCollider(block2rect, Collider::Type::SOLID_BODY, this);

	block3rect.x = 120;
	block3rect.y = 380;
	block3rect.w = 140;
	block3rect.h = 30;

	block3 = App->collisions->AddCollider(block3rect, Collider::Type::SOLID_BODY, this);

	block4rect.x = 380;
	block4rect.y = 350;
	block4rect.w = 220;
	block4rect.h = 100;

	block4 = App->collisions->AddCollider(block4rect, Collider::Type::SOLID_BODY, this);

	block5rect.x = 480;
	block5rect.y = 165;
	block5rect.w = 70;
	block5rect.h = 185;

	block5 = App->collisions->AddCollider(block5rect, Collider::Type::SOLID_BODY, this);

	waterRect.x = 260;
	waterRect.y = 380;
	waterRect.w = 120;
	waterRect.h = 30;

	water = App->collisions->AddCollider(waterRect, Collider::Type::LIQUID_BODY, this);

	fall1Rect.x = 20;
	fall1Rect.y = 210;
	fall1Rect.w = 10;
	fall1Rect.h = 10;

	fall1 = App->collisions->AddCollider(fall1Rect, Collider::Type::FALL, this);

	fall2Rect.x = 130;
	fall2Rect.y = 210;
	fall2Rect.w = 10;
	fall2Rect.h = 10;

	fall2 = App->collisions->AddCollider(fall2Rect, Collider::Type::FALL, this);

	fall3Rect.x = 140;
	fall3Rect.y = 340;
	fall3Rect.w = 10;
	fall3Rect.h = 10;

	fall3 = App->collisions->AddCollider(fall3Rect, Collider::Type::FALL, this);

	fall4Rect.x = 340;
	fall4Rect.y = 340;
	fall4Rect.w = 10;
	fall4Rect.h = 10;

	fall4 = App->collisions->AddCollider(fall4Rect, Collider::Type::FALL, this);

	fall5Rect.x = 460;
	fall5Rect.y = 165;
	fall5Rect.w = 10;
	fall5Rect.h = 10;

	fall5 = App->collisions->AddCollider(fall5Rect, Collider::Type::FALL, this);

	fall6Rect.x = 560;
	fall6Rect.y = 165;
	fall6Rect.w = 10;
	fall6Rect.h = 10;

	fall6 = App->collisions->AddCollider(fall6Rect, Collider::Type::FALL, this);
	
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
	

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debugCollider = !debugCollider;
	}

	if (debugCollider) {
		App->renderer->DrawQuad(block1rect, 255, 0, 255, 80);
		App->renderer->DrawQuad(block2rect, 255, 0, 255, 80);
		App->renderer->DrawQuad(block3rect, 255, 0, 255, 80);
		App->renderer->DrawQuad(block4rect, 255, 0, 255, 80);
		App->renderer->DrawQuad(block5rect, 255, 0, 255, 80);
		App->renderer->DrawQuad(waterRect, 0, 0, 255, 80);
		App->renderer->DrawQuad(fall1Rect, 0, 255, 255, 80);
		App->renderer->DrawQuad(fall2Rect, 0, 255, 255, 80);
		App->renderer->DrawQuad(fall3Rect, 0, 255, 255, 80);
		App->renderer->DrawQuad(fall4Rect, 0, 255, 255, 80);
		App->renderer->DrawQuad(fall5Rect, 0, 255, 255, 80);
		App->renderer->DrawQuad(fall6Rect, 0, 255, 255, 80);
		
	}

	return UPDATE_CONTINUE;
}
