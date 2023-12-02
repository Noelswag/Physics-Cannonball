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

	App->physics->Cannon.x = 0;
	App->physics->Cannon.y = 300;
	App->physics->Cannon.jumpv = 10;
	App->physics->Cannon.jumpa = 0;
	App->physics->Cannon.ax = 2;
	App->physics->Cannon.m = 100;
	App->physics->Cannon.force = -50;
	App->physics->Cannon.surface = 20;
	App->physics->Cannon.volumen = 200.0f;

	App->physics->applyGravity(&App->physics->Cannon);
	App->physics->applyAerodynamics(&App->physics->Cannon);
	

	cannon = App->textures->Load("Graphics/Cannon2.png");
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

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		jumpVal++;

		if (jumpVal > 7) {
			jumpVal = 0;
		}
	}
	LOG("-----------------------JUMPVAL IS %d", jumpVal);

	if (App->physics->Cannon.x > 300 && App->physics->Cannon.y > 300) {
		App->physics->applyHydrodynamics(&App->physics->Cannon);
		App->physics->Cannon.Fy += hydroVar;
		if (hydroVar < App->physics->Cannon.m * GRAVITY_) {
			hydroVar += 2;
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

	switch (jumpOption) {
	case 0:
		testJump = jumpOptions::JUMPX;
		LOG("--------------------USING MOVX FOR JUMP");
		break;
	case 1:
		testJump = jumpOptions::JUMPVEL;
		LOG("--------------------USING FIXVEL FOR JUMP");
		break;
	case 2:
		testJump = jumpOptions::JUMPACC;
		LOG("--------------------USING FIXACC FOR JUMP");
		break;
	case 3:
		testJump = jumpOptions::JUMP_MOMENTUM;
		LOG("--------------------USING MOMENTUM FOR JUMP");
		break;
	case 4:
		testJump = jumpOptions::JUMP_IMPULSE;
		LOG("--------------------USING IMPULSE FOR JUMP");
		break;
	case 5:
		testJump = jumpOptions::JUMP_ACCELERATION;
		LOG("--------------------USING ACCELERATION FOR JUMP");
		break;
	case 6:
		testJump = jumpOptions::JUMP_FORCE;
		LOG("--------------------USING FORCE FOR JUMP");
		break;
	case 7:
		testJump = jumpOptions::NO_JUMP;
		LOG("--------------------NO JUMPING");
		break;
	}

	switch (test)
	{
	case ModulePhysics::MOVX:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			

			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x < SCREEN_WIDTH - 101)
			{
				App->physics->Cannon.x++;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x > 0)
			{
				App->physics->Cannon.x--;
			}
		}
		break;
	case ModulePhysics::FIXVEL:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			

			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x < SCREEN_WIDTH - 101)
			{
				App->physics->Cannon.x += App->physics->Cannon.vx;
			}
		

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x > 0)
			{
				App->physics->Cannon.x -= App->physics->Cannon.vx;
			}
		}
		break;
	case ModulePhysics::FIXACC:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			
			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x < SCREEN_WIDTH - 101)
			{
				App->physics->Cannon.x += App->physics->Cannon.vx;
				App->physics->Cannon.vx += App->physics->Cannon.ax;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (App->physics->Cannon.vx > 0) {
				App->physics->Cannon.x -= App->physics->Cannon.vx;
			}
			else {
				App->physics->Cannon.x += App->physics->Cannon.vx;
			}

			App->physics->Cannon.vx -= App->physics->Cannon.ax;
			

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x > 0)
			{
				if (App->physics->Cannon.vx > 0) {
					App->physics->Cannon.x -= App->physics->Cannon.vx;
			}
			else {
					App->physics->Cannon.x += App->physics->Cannon.vx;
			}

				App->physics->Cannon.vx -= App->physics->Cannon.ax;
			}
		}
		break;
	case ModulePhysics::MOMENTUM:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {

			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x < SCREEN_WIDTH - 101)
			{
				App->physics->Cannon.x += App->physics->Cannon.vx;
				App->physics->Cannon.vx += App->physics->Cannon.ax;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (App->physics->Cannon.vx > 0) {
				App->physics->Cannon.x -= App->physics->Cannon.vx;
			}
			else {
				App->physics->Cannon.x += App->physics->Cannon.vx;
			}

			App->physics->Cannon.vx -= App->physics->Cannon.ax;


			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (App->physics->Cannon.x > 0)
			{
				if (App->physics->Cannon.vx > 0) {
					App->physics->Cannon.x -= App->physics->Cannon.vx;
				}
				else {
					App->physics->Cannon.x += App->physics->Cannon.vx;
				}

				App->physics->Cannon.vx -= App->physics->Cannon.ax;
			}
		}
		
		break;
	case ModulePhysics::IMPULSE:

		break;
	case ModulePhysics::ACCELERATION:
		break;
	case ModulePhysics::FORCE:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			App->physics->Cannon.Fx = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}

			if (testPlayer == playerStatus::STOP_PLAYER) {
				App->physics->Cannon.Fx = 30;
			}
			else {
				App->physics->Cannon.Fx = 15;
			}
			
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			
			if (testPlayer == playerStatus::STOP_PLAYER) {
				App->physics->Cannon.Fx = -30;
			}
			else {
				App->physics->Cannon.Fx = -15;
			}

		}

			App->physics->Cannon.ax = App->physics->Cannon.Fx / App->physics->Cannon.m;
			App->physics->Cannon.vx += App->physics->Cannon.ax;

			// Apply friction
			if (App->physics->Cannon.vx > 0)
			{
				App->physics->Cannon.vx -= FRICTION;
				if (App->physics->Cannon.vx < 0)
					App->physics->Cannon.vx = 0;
			}
			else if (App->physics->Cannon.vx < 0)
			{
				App->physics->Cannon.vx += FRICTION;
				if (App->physics->Cannon.vx > 0)
					App->physics->Cannon.vx = 0;
			}
			App->physics->Cannon.x += App->physics->Cannon.vx;
		
		
		break;

	}
	
	

		switch (testJump)
		{
		case ModulePlayer::JUMPX:
			App->physics->Cannon.y -= 5;
			testPlayer = playerStatus::GRAVITY;
			break;
		case ModulePlayer::JUMPVEL:
			App->physics->Cannon.y -= App->physics->Cannon.jumpv;
			testPlayer = playerStatus::GRAVITY;
			break;
		case ModulePlayer::JUMPACC:
			App->physics->Cannon.y -= App->physics->Cannon.jumpv;
			App->physics->Cannon.jumpv -= GRAVITY_;
			testPlayer = playerStatus::GRAVITY;
			break;
		case ModulePlayer::JUMP_MOMENTUM:
			break;
		case ModulePlayer::JUMP_IMPULSE:
			break;
		case ModulePlayer::JUMP_ACCELERATION:
			if (App->physics->Cannon.jumpa < 2) {
				App->physics->Cannon.jumpa += 0.1;
			}
			App->physics->Cannon.jumpv += App->physics->Cannon.jumpa;
			App->physics->Cannon.y -= App->physics->Cannon.jumpv;

			testPlayer = playerStatus::GRAVITY;
			break;
		case ModulePlayer::JUMP_FORCE:
			if (testPlayer == playerStatus::STOP_PLAYER) {
				App->physics->Cannon.vy = -300;
			}
			//App->physics->Cannon.jumpa = App->physics->Cannon.force / App->physics->Cannon.m;
			//App->physics->Cannon.jumpv += App->physics->Cannon.jumpa;
			//App->physics->Cannon.y -= App->physics->Cannon.jumpv;
			testPlayer = playerStatus::GRAVITY;
			break;
		case ModulePlayer::NO_JUMP:
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				jumpOption = jumpVal;
			}
			break;

		}


	

	switch (testPlayer)
	{
	case ModulePlayer::STOP_PLAYER:
		//App->physics->Cannon.vy = 5;
		App->physics->Cannon.jumpv = 30;
		App->physics->Cannon.jumpa = 0;
		App->physics->Cannon.vy = 0;
		if (App->physics->Cannon.x > 300) {
			testPlayer = playerStatus::GRAVITY;
		}
		break;
	case ModulePlayer::GRAVITY:
		App->physics->applyWind(&App->physics->Cannon);
		
		if (App->physics->mode == 1)
			App->physics->euler(&App->physics->Cannon);
		if (App->physics->mode == 2)
			App->physics->eulerSympletic(&App->physics->Cannon);
		if (App->physics->mode == 3)
			App->physics->velocityVerlet(&App->physics->Cannon);

		if (App->physics->Cannon.y >= 300 && App->physics->Cannon.x <= 300) {
			jumpOption = 7;
			testPlayer = playerStatus::STOP_PLAYER;
			testJump = jumpOptions::NO_JUMP;
			
		}
		
		break;
	}

	App->physics->resetForces(&App->physics->Cannon);
	

	
	App->renderer->Blit(ball, App->physics->bullet.x, App->physics->bullet.y, NULL);
	if (front)
	{
		App->renderer->Blit(cannon, App->physics->Cannon.x, App->physics->Cannon.y, NULL, NULL, App->physics->angle, 25, 25);
	}
	else
	{
		App->renderer->BlitMirror(cannon, App->physics->Cannon.x-25, App->physics->Cannon.y, NULL, NULL, (App->physics->angle - 180), 75, 25);
	}


	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{

}

