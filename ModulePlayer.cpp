#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

#define FRICTION 0.1f
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
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		movOption++;
		if (movOption > 6) {
			movOption = 0;
		}
	}

	switch (movOption) {
	case 0:
		test = movementOptions::MOVX;
		LOG("--------------------USING MOVX");
		break;
	case 1:
		test = movementOptions::FIXVEL;
		LOG("--------------------USING FIXVEL");
		break;
	case 2:
		test = movementOptions::FIXACC;
		LOG("--------------------USING FIXACC");
		break;
	case 3:
		test = movementOptions::MOMENTUM;
		LOG("--------------------USING MOMENTUM");
		break;
	case 4:
		test = movementOptions::IMPULSE;
		LOG("--------------------USING IMPULSE");
		break;
	case 5:
		test = movementOptions::ACCELERATION;
		LOG("--------------------USING ACCELERATION");
		break;
	case 6:
		test = movementOptions::FORCE;
		LOG("--------------------USING FORCE");
		break;

	}

	switch (test)
	{
	case ModulePhysics::MOVX:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.x += 10;
			bullet.x = testCannon.x + testCannon.w;

			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x < SCREEN_WIDTH - 101)
			{
				cannonpos++;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.x -= 10;
			bullet.x = testCannon.x + testCannon.w;

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x > 0)
			{
				testCannon.x--;
			}
		}
		break;
	case ModulePhysics::FIXVEL:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.x += testCannon.vx;
			bullet.x = testCannon.x + testCannon.w;

			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x < SCREEN_WIDTH - 101)
			{
				testCannon.x++;
			}
		

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.x -= testCannon.vx;
			bullet.x = testCannon.x + testCannon.w;

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x > 0)
			{
				testCannon.x--;
			}
		}
		break;
	case ModulePhysics::FIXACC:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.x += testCannon.vx;
			testCannon.vx += testCannon.ax;

			bullet.x = testCannon.x + testCannon.w;
			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x < SCREEN_WIDTH - 101)
			{
				testCannon.x++;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (testCannon.vx > 0) {
				testCannon.x -= testCannon.vx;
			}
			else {
				testCannon.x += testCannon.vx;
			}

			testCannon.vx -= testCannon.ax;
			bullet.x = testCannon.x + testCannon.w;

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x > 0)
			{
				testCannon.x--;
			}
		}
		break;
	case ModulePhysics::MOMENTUM:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.x += testCannon.vx;
			testCannon.vx += testCannon.ax;
			bullet.x = testCannon.x + testCannon.w;

			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x < SCREEN_WIDTH - 101)
			{
				testCannon.x++;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (testCannon.vx > 0) {
				testCannon.x -= testCannon.vx;
			}
			else {
				testCannon.x += testCannon.vx;
			}
			testCannon.vx -= testCannon.ax;
			bullet.x = testCannon.x + testCannon.w;

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (testCannon.x > 0)
			{
				testCannon.x--;
			}
		}
		// Apply friction
		testCannon.vx -= (testCannon.vx > 0) ? FRICTION : (testCannon.vx < 0) ? -FRICTION : 0.0f;
		break;
	case ModulePhysics::IMPULSE:

		break;
	case ModulePhysics::ACCELERATION:
		break;
	case ModulePhysics::FORCE:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			testCannon.ax = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.ax = 1;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			testCannon.ax = -1;

		}

		testCannon.ax = testCannon.ax / testCannon.m;
		testCannon.vx += testCannon.ax;
		// Apply friction
		if (testCannon.vx > 0)
		{
			testCannon.vx -= FRICTION;
			if (testCannon.vx < 0)
				testCannon.vx = 0;
		}
		else if (testCannon.vx < 0)
		{
			testCannon.vx += FRICTION;
			if (testCannon.vx > 0)
				testCannon.vx = 0;
		}
		testCannon.x += testCannon.vx;
		bullet.x = testCannon.x + testCannon.w;
		break;

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

	switch (testJump)
	{
	case ModulePlayer::JUMPX:
		testCannon.y -= 60;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMPVEL:
		testCannon.y -= testCannon.jumpv;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMPACC:
		testCannon.y -= testCannon.jumpv;
		testCannon.jumpv -= testCannon.ay;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMP_MOMENTUM:
		break;
	case ModulePlayer::JUMP_IMPULSE:
		break;
	case ModulePlayer::JUMP_ACCELERATION:
		if (testCannon.jumpa < 2) {
			testCannon.jumpa += 0.1;
		}
		testCannon.jumpv += testCannon.jumpa;
		testCannon.y -= testCannon.jumpv;

		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMP_FORCE:
		testCannon.jumpa = testCannon.force / testCannon.m;
		testCannon.jumpv += testCannon.jumpa;
		testCannon.y -= testCannon.jumpv;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::NO_JUMP:
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			jumpOption = jumpVal;
		}
		break;

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



