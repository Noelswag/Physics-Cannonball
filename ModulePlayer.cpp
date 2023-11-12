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

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		jumpVal++;

		if (jumpVal > 7) {
			jumpVal = 0;
		}
	}
	LOG("-----------------------JUMPVAL IS %d", jumpVal);

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
			if (cannonposX < SCREEN_WIDTH - 101)
			{
				cannonposX++;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (cannonposX > 0)
			{
				cannonposX--;
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
			if (cannonposX < SCREEN_WIDTH - 101)
			{
				cannonposX += vx;
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
			if (cannonposX > 0)
			{
				cannonposX -= vx;
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
			if (cannonposX < SCREEN_WIDTH - 101)
			{
				cannonposX += vx;
				vx += ax;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (vx > 0) {
				cannonposX -= vx;
			}
			else {
				cannonposX += vx;
			}

			vx -= ax;
			

			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (cannonposX > 0)
			{
				if (vx > 0) {
				cannonposX -= vx;
			}
			else {
				cannonposX += vx;
			}

			vx -= ax;
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
			if (cannonposX < SCREEN_WIDTH - 101)
			{
				cannonposX += vx;
				vx += ax;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (vx > 0) {
				cannonposX -= vx;
			}
			else {
				cannonposX += vx;
			}

			vx -= ax;


			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			if (cannonposX > 0)
			{
				if (vx > 0) {
					cannonposX -= vx;
				}
				else {
					cannonposX += vx;
				}

				vx -= ax;
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
			ax = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->physics->angle <= 0) {
			if (!front)
			{
				front = true;
				App->physics->angle = -180 - App->physics->angle;
			}
			ax = 1;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->physics->angle <= 0) {
			if (front)
			{
				front = false;
				App->physics->angle = -180 - App->physics->angle;
			}
			ax = -1;

		}

		ax = ax / m;
		vx += ax;
		// Apply friction
		if (vx > 0)
		{
			vx -= FRICTION;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0)
		{
			vx += FRICTION;
			if (vx > 0)
				vx = 0;
		}
		cannonposX += vx;
		
		break;

	}
	
	
	

	switch (testJump)
	{
	case ModulePlayer::JUMPX:
		cannonposY -= 20;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMPVEL:
		cannonposY -= jumpv;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMPACC:
		cannonposY -= jumpv;
		jumpv -= ay;
		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMP_MOMENTUM:
		break;
	case ModulePlayer::JUMP_IMPULSE:
		break;
	case ModulePlayer::JUMP_ACCELERATION:
		if (jumpa < 2) {
			jumpa += 0.1;
		}
		jumpv += jumpa;
		cannonposY -= jumpv;

		testPlayer = playerStatus::GRAVITY;
		break;
	case ModulePlayer::JUMP_FORCE:
		jumpa = force / m;
		jumpv += jumpa;
		cannonposY -= jumpv;
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
		vy = 5;
		jumpv = 20;
		break;
	case ModulePlayer::GRAVITY:
		vy += ay;
		
		if (Euler) {
			cannonposY += vy;
		}
		else {
			cannonposY += vy + 1 / 2 * ay;
		}

		if (cannonposY >= 275) {
			jumpOption = 7;
			testPlayer = playerStatus::STOP_PLAYER;
			testJump = jumpOptions::NO_JUMP;
		}
		break;
	}

	
	App->renderer->Blit(ball, App->physics->x, App->physics->y, NULL);
	if (front)
	{
		App->renderer->Blit(cannon, cannonposX, cannonposY, NULL, NULL, App->physics->angle, 51, 51);
	}
	else
	{
		App->renderer->BlitMirror(cannon, cannonposX-51, cannonposY, NULL, NULL, (App->physics->angle - 180), 149, 51);
	}


	return UPDATE_CONTINUE;
}



