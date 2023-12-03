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
	App->physics->Cannon.y = 315;
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

	PlayerRect.x = App->physics->Cannon.x;
	PlayerRect.y = App->physics->Cannon.y;
	PlayerRect.w = 25;
	PlayerRect.h = 25;

	bulletRect.x = App->physics->bullet.x;
	bulletRect.y = App->physics->bullet.y;
	bulletRect.w = 10;
	bulletRect.h = 10;

	playerCollider = App->collisions->AddCollider(PlayerRect, Collider::Type::PLAYER1, this);
	bulletCollider = App->collisions->AddCollider(bulletRect, Collider::Type::BULLET1, this);

	lifeBar.x = App->physics->Cannon.x;
	lifeBar.y = App->physics->Cannon.y - 15;
	lifeBar.w = 50;
	lifeBar.h = 7;

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
	

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debugCollider = !debugCollider;

	// Bullet Launch
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->physics->mode = 1;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		App->physics->mode = 2;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->physics->mode = 3;


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		if (App->physics->power < 600)
			App->physics->power += 10;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		if (App->physics->power > 100)
			App->physics->power -= 10;


	App->physics->totalvelocity = sqrt(pow(App->physics->bullet.vx, 2) + pow(App->physics->bullet.vy, 2));

	/*if (App->physics->bullet.y >= App->physics->floor && App->physics->flying && (App->physics->bullet.x <= 200 || App->physics->bullet.x >= 400))
	{
		App->physics->bullet.y = App->physics->floor;
		App->physics->bounceVertical(&App->physics->bullet);
		App->audio->PlayFx(App->physics->bonk);
		if ((App->physics->bullet.x > (double)SCREEN_WIDTH || App->physics->bullet.x < -25 || App->physics->totalvelocity < 125) && App->physics->flying)
		{
			App->physics->flying = false;
			App->physics->start = true;
		}
	}*/

	/*if (App->physics->bullet.y >= 380 && App->physics->bullet.x > 260 && App->physics->bullet.x < 380 && App->physics->flying)
	{
		App->physics->applyHydrodynamics(&App->physics->bullet);
		if (App->physics->bullet.x > (double)SCREEN_WIDTH)
		{
			App->physics->flying = false;
			App->physics->start = true;
		}
	}*/


	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (App->physics->start)
		{
			App->physics->start = false;
			App->physics->flying = true;

			App->physics->anglerad = -App->physics->angle * M_PI / 180;
			App->physics->bullet.vx = cos(App->physics->anglerad) * App->physics->power;
			App->physics->bullet.vy = -sin(App->physics->anglerad) * App->physics->power;

			App->physics->bullet.x = (App->physics->Cannon.x + 13) + 75 * cos(App->physics->anglerad);
			App->physics->bullet.y = (double)App->physics->Cannon.y - 75 * sin(App->physics->anglerad);
			App->physics->spin = 0;
			App->audio->PlayFx(App->physics->boom);
			//Offsets so that the ball comes out off the cannon. Uncomment the lines below for 0,0

			App->physics->bullet.x = App->physics->Cannon.x + 13;
			App->physics->bullet.y = App->physics->Cannon.y + 13;
			/*
			x = 0;
			y = floor;
			*/
		}
	}

	if (App->physics->flying)
	{

		App->physics->applyWind(&App->physics->bullet);

		if (App->physics->mode == 1)
			App->physics->euler(&App->physics->bullet);
		if (App->physics->mode == 2)
			App->physics->eulerSympletic(&App->physics->bullet);
		if (App->physics->mode == 3)
			App->physics->velocityVerlet(&App->physics->bullet);

		App->physics->spin += 10;
	}

	if (App->physics->mode == 1)
		App->physics->modetext = "Euler";

	if (App->physics->mode == 2)
		App->physics->modetext = "Sympletic Euler";

	if (App->physics->mode == 3)
		App->physics->modetext = "Velocity Verlet";

	App->physics->displayx = App->physics->bullet.x;
	App->physics->displayy = -(App->physics->bullet.y - App->physics->floor);


	if (App->player->front)
	{
		App->physics->displayAngle = -App->physics->angle;
	}
	else
	{
		App->physics->displayAngle = (-180 - App->physics->angle);
	}

	App->physics->displayPower = (App->physics->power / 10 - 10) * 2;



	sprintf_s(App->physics->titletext, 200, "X:%03d Y:%03d Angle: %02d (left/right) Power: %02d (up/down) Mode: %s(1,2,3)", App->physics->displayx, App->physics->displayy, App->physics->displayAngle, App->physics->displayPower, App->physics->modetext);

	App->window->SetTitle(App->physics->titletext);


	App->physics->resetForces(&App->physics->bullet);

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

	if (App->physics->Cannon.x > 200 && App->physics->Cannon.x < 400 && App->physics->Cannon.y > 300) {
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
		if (App->physics->Cannon.x > 200 && App->physics->Cannon.x < 400) {
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
		
		break;
	}

	App->physics->resetForces(&App->physics->Cannon);
	
	
	if (life > 0) {
		App->renderer->Blit(ball, App->physics->bullet.x, App->physics->bullet.y, NULL);
		if (front)
		{
			App->renderer->Blit(cannon, App->physics->Cannon.x, App->physics->Cannon.y, NULL, NULL, App->physics->angle, 25, 25);
		}
		else
		{
			App->renderer->BlitMirror(cannon, App->physics->Cannon.x - 25, App->physics->Cannon.y, NULL, NULL, (App->physics->angle - 180), 75, 25);
		}
	}
	

	lifeBar.x = App->physics->Cannon.x;
	lifeBar.y = App->physics->Cannon.y - 15;
	App->renderer->DrawQuad(lifeBar, 0, 255, 0, 255);

	// Debug colliders
	playerCollider->SetPos(App->physics->Cannon.x, App->physics->Cannon.y);
	PlayerRect.x = App->physics->Cannon.x;
	PlayerRect.y = App->physics->Cannon.y;
	bulletCollider->SetPos(App->physics->bullet.x, App->physics->bullet.y);
	bulletRect.x = App->physics->bullet.x + 2;
	bulletRect.y = App->physics->bullet.y + 2;

	if (debugCollider)
	{
		App->renderer->DrawQuad(PlayerRect, 0, 255, 0, 80);
		App->renderer->DrawQuad(bulletRect, 255, 0, 0, 80);

	}

	waitForDmg++;
	if (waitForDmg >= 60) {
		canDmg = true;
		waitForDmg = 0;
	}

	
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{

	if (c1->type == Collider::Type::PLAYER1 && c2->type == Collider::Type::BULLET2) {
		if (canDmg) {
			life -= dmg;
			lifeBar.w -= 10;
			canDmg = false;
		}
		
	}
	if (c1->type == Collider::Type::PLAYER1 && c2->type == Collider::Type::FALL && testPlayer == playerStatus::STOP_PLAYER) {
		testPlayer = playerStatus::GRAVITY;
	}

	if (c1->type == Collider::Type::PLAYER1 && c2->type == Collider::Type::SOLID_BODY && testPlayer == playerStatus::GRAVITY) {
		if (c1->rect.y < c2->rect.y) {
			testPlayer = playerStatus::STOP_PLAYER;
			jumpOption = 7;
			testJump = jumpOptions::NO_JUMP;
		}
		
		if (c1->rect.y > c2->rect.y) {
			App->physics->Cannon.x--;
			canMove = false;
		}
		
		
	}

	if (c1->type == Collider::Type::BULLET1 && c2->type == Collider::Type::SOLID_BODY) {
		if (c1->rect.y < c2->rect.y) {
			
			App->physics->bounceVertical(&App->physics->bullet);
			if ((App->physics->bullet.x > (double)SCREEN_WIDTH || App->physics->bullet.x < -25 || App->physics->totalvelocity < 125) && App->physics->flying) {
				App->physics->flying = false;
				App->physics->start = true;
			}
		}
		else if (c1->rect.y > c2->rect.y) {
			App->physics->bounceHorizontal(&App->physics->bullet);
		}
	}
	else if (c1->type == Collider::Type::BULLET1 && c2->type == Collider::Type::LIQUID_BODY && App->physics->flying == true) {
		App->physics->applyHydrodynamics(&App->physics->bullet);
		if (App->physics->bullet.x > (double)SCREEN_WIDTH)
		{
			App->physics->flying = false;
			App->physics->start = true;
		}
	}

}

