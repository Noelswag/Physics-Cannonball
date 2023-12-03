#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"

#define FRICTION 0.1f
ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player2");

	App->physics->Cannon2.x = 500;
	App->physics->Cannon2.y = 300;
	App->physics->Cannon2.jumpv = 10;
	App->physics->Cannon2.jumpa = 0;
	App->physics->Cannon2.ax = 2;
	App->physics->Cannon2.m = 100;
	App->physics->Cannon2.force = -50;
	App->physics->Cannon2.surface = 20;
	App->physics->Cannon2.volumen = 200.0f;

	App->physics->applyGravity(&App->physics->Cannon2);
	App->physics->applyAerodynamics(&App->physics->Cannon2);


	cannon2 = App->textures->Load("Graphics/Cannon2.png");
	ball2 = App->textures->Load("Graphics/Ball2.png");

	Player2Rect.x = App->physics->Cannon2.x;
	Player2Rect.y = App->physics->Cannon2.y;
	Player2Rect.w = 25;
	Player2Rect.h = 25;

	bullet2Rect.x = App->physics->bullet2.x;
	bullet2Rect.y = App->physics->bullet2.y;
	bullet2Rect.w = 10;
	bullet2Rect.h = 10;

	player2Collider = App->collisions->AddCollider(Player2Rect, Collider::Type::PLAYER2, this);
	bullet2Collider = App->collisions->AddCollider(bullet2Rect, Collider::Type::BULLET2, this);

	lifeBar2.x = App->physics->Cannon2.x;
	lifeBar2.y = App->physics->Cannon2.y - 15;
	lifeBar2.w = 50;
	lifeBar2.h = 7;

	return true;
}

// Unload assets
bool ModulePlayer2::CleanUp()
{
	LOG("Unloading player2");

	return true;
}

// Update: draw background
update_status ModulePlayer2::Update()
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


	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
		if (App->physics->power2 < 600)
			App->physics->power2 += 10;
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
		if (App->physics->power2 > 100)
			App->physics->power2 -= 10;


	App->physics->totalvelocity2 = sqrt(pow(App->physics->bullet2.vx, 2) + pow(App->physics->bullet2.vy, 2));

	if (App->physics->bullet2.y >= App->physics->floor && App->physics->flying2 && (App->physics->bullet2.x <= 200 || App->physics->bullet2.x >= 400))
	{
		App->physics->bullet2.y = App->physics->floor;
		App->physics->bounceVertical(&App->physics->bullet2);
		App->audio->PlayFx(App->physics->bonk);
		if ((App->physics->bullet2.x > (double)SCREEN_WIDTH || App->physics->bullet2.x < -25 || App->physics->totalvelocity2 < 125) && App->physics->flying2)
		{
			App->physics->flying2 = false;
			App->physics->start2 = true;
		}
	}

	if (App->physics->bullet2.y >= App->physics->floor && App->physics->bullet2.x > 200 && App->physics->bullet2.x < 400 && App->physics->flying2)
	{
		App->physics->applyHydrodynamics(&App->physics->bullet2);
		if (App->physics->bullet2.x > (double)SCREEN_WIDTH)
		{
			App->physics->flying2 = false;
			App->physics->start2 = true;
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (App->physics->start2)
		{
			App->physics->start2 = false;
			App->physics->flying2 = true;

			App->physics->anglerad2 = -App->physics->angle2 * M_PI / 180;
			App->physics->bullet2.vx = cos(App->physics->anglerad2) * App->physics->power2;
			App->physics->bullet2.vy = -sin(App->physics->anglerad2) * App->physics->power2;

			App->physics->bullet2.x = (App->physics->Cannon2.x + 13) + 75 * cos(App->physics->anglerad2);
			App->physics->bullet2.y = (double)App->physics->Cannon2.y - 75 * sin(App->physics->anglerad2);
			App->physics->spin2 = 0;
			App->audio->PlayFx(App->physics->boom);
			//Offsets so that the ball comes out off the cannon. Uncomment the lines below for 0,0

			App->physics->bullet2.x = App->physics->Cannon2.x + 13;
			App->physics->bullet2.y = App->physics->Cannon2.y + 13;
			/*
			x = 0;
			y = floor;
			*/
		}
	}

	if (App->physics->flying2)
	{

		App->physics->applyWind(&App->physics->bullet2);

		if (App->physics->mode == 1)
			App->physics->euler(&App->physics->bullet2);
		if (App->physics->mode == 2)
			App->physics->eulerSympletic(&App->physics->bullet2);
		if (App->physics->mode == 3)
			App->physics->velocityVerlet(&App->physics->bullet2);

		App->physics->spin2 += 10;
	}

	if (App->physics->mode == 1)
		App->physics->modetext = "Euler";

	if (App->physics->mode == 2)
		App->physics->modetext = "Sympletic Euler";

	if (App->physics->mode == 3)
		App->physics->modetext = "Velocity Verlet";

	App->physics->displayx2 = App->physics->bullet2.x;
	App->physics->displayy2 = -(App->physics->bullet2.y - App->physics->floor);


	if (front2)
	{
		App->physics->displayAngle2 = -App->physics->angle2;
	}
	else
	{
		App->physics->displayAngle2 = (-180 - App->physics->angle2);
	}

	App->physics->displayPower2 = (App->physics->power2 / 10 - 10) * 2;



	//sprintf_s(App->physics->titletext, 200, "X:%03d Y:%03d Angle: %02d (left/right) Power: %02d (up/down) Mode: %s(1,2,3)", App->physics->displayx, App->physics->displayy, App->physics->displayAngle, App->physics->displayPower, App->physics->modetext);

	//App->window->SetTitle(App->physics->titletext);


	App->physics->resetForces(&App->physics->bullet2);

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
	{


		if (front2 && App->physics->angle2 > -90)
		{
			App->physics->angle2--;

		}

		if (!front2 && App->physics->angle2 > -180)
		{
			App->physics->angle2--;

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT)
	{
		if (front2 && App->physics->angle2 < 0)
		{
			App->physics->angle2++;
		}

		if (!front2 && App->physics->angle2 < -90)
		{
			App->physics->angle2++;
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		movOption2++;
		if (movOption2 > 6) {
			movOption2 = 0;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		jumpVal2++;

		if (jumpVal2 > 7) {
			jumpVal2 = 0;
		}
	}
	LOG("-----------------------JUMPVAL IS %d", jumpVal2);

	if (App->physics->Cannon2.x > 200 && App->physics->Cannon2.x < 400 && App->physics->Cannon2.y > 300) {
		App->physics->applyHydrodynamics(&App->physics->Cannon2);
		App->physics->Cannon2.Fy += hydroVar2;
		if (hydroVar2 < App->physics->Cannon2.m * GRAVITY_) {
			hydroVar2 += 2;
		}

	}

	switch (movOption2) {
	case 0:
		test2 = movement2Options::MOVX;
		LOG("--------------------USING MOVX");
		break;
	case 1:
		test2 = movement2Options::FIXVEL;
		LOG("--------------------USING FIXVEL");
		break;
	case 2:
		test2 = movement2Options::FIXACC;
		LOG("--------------------USING FIXACC");
		break;
	case 3:
		test2 = movement2Options::MOMENTUM;
		LOG("--------------------USING MOMENTUM");
		break;
	case 4:
		test2 = movement2Options::IMPULSE;
		LOG("--------------------USING IMPULSE");
		break;
	case 5:
		test2 = movement2Options::ACCELERATION;
		LOG("--------------------USING ACCELERATION");
		break;
	case 6:
		test2 = movement2Options::FORCE;
		LOG("--------------------USING FORCE");
		break;

	}

	switch (jumpOption2) {
	case 0:
		testJump2 = jump2Options::JUMPX;
		LOG("--------------------USING MOVX FOR JUMP");
		break;
	case 1:
		testJump2 = jump2Options::JUMPVEL;
		LOG("--------------------USING FIXVEL FOR JUMP");
		break;
	case 2:
		testJump2 = jump2Options::JUMPACC;
		LOG("--------------------USING FIXACC FOR JUMP");
		break;
	case 3:
		testJump2 = jump2Options::JUMP_MOMENTUM;
		LOG("--------------------USING MOMENTUM FOR JUMP");
		break;
	case 4:
		testJump2 = jump2Options::JUMP_IMPULSE;
		LOG("--------------------USING IMPULSE FOR JUMP");
		break;
	case 5:
		testJump2 = jump2Options::JUMP_ACCELERATION;
		LOG("--------------------USING ACCELERATION FOR JUMP");
		break;
	case 6:
		testJump2 = jump2Options::JUMP_FORCE;
		LOG("--------------------USING FORCE FOR JUMP");
		break;
	case 7:
		testJump2 = jump2Options::NO_JUMP;
		LOG("--------------------NO JUMPING");
		break;
	}

	switch (test2)
	{
	case ModulePhysics::MOVX:
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && App->physics->angle2 <= 0) {


			if (!front2)
			{
				front2 = true;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x < SCREEN_WIDTH)
			{
				App->physics->Cannon2.x++;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && App->physics->angle2 <= 0) {


			if (front2)
			{
				front2 = false;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x > 0)
			{
				App->physics->Cannon2.x--;
			}
		}
		break;
	case ModulePhysics::FIXVEL:
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && App->physics->angle2 <= 0) {


			if (!front2)
			{
				front2 = true;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x < SCREEN_WIDTH)
			{
				App->physics->Cannon2.x += App->physics->Cannon2.vx;
			}


		}

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && App->physics->angle2 <= 0) {


			if (front2)
			{
				front2 = false;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x > 0)
			{
				App->physics->Cannon2.x -= App->physics->Cannon2.vx;
			}
		}
		break;
	case ModulePhysics::FIXACC:
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && App->physics->angle2 <= 0) {

			if (!front2)
			{
				front2 = true;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x < SCREEN_WIDTH)
			{
				App->physics->Cannon2.x += App->physics->Cannon2.vx;
				App->physics->Cannon2.vx += App->physics->Cannon2.ax;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && App->physics->angle2 <= 0) {
			if (App->physics->Cannon2.vx > 0) {
				App->physics->Cannon2.x -= App->physics->Cannon2.vx;
			}
			else {
				App->physics->Cannon2.x += App->physics->Cannon2.vx;
			}

			App->physics->Cannon2.vx -= App->physics->Cannon2.ax;


			if (front2)
			{
				front2 = false;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x > 0)
			{
				if (App->physics->Cannon2.vx > 0) {
					App->physics->Cannon2.x -= App->physics->Cannon2.vx;
				}
				else {
					App->physics->Cannon2.x += App->physics->Cannon2.vx;
				}

				App->physics->Cannon2.vx -= App->physics->Cannon2.ax;
			}
		}
		break;
	case ModulePhysics::MOMENTUM:
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && App->physics->angle2 <= 0) {

			if (!front2)
			{
				front2 = true;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x < SCREEN_WIDTH)
			{
				App->physics->Cannon2.x += App->physics->Cannon2.vx;
				App->physics->Cannon2.vx += App->physics->Cannon2.ax;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && App->physics->angle2 <= 0) {
			if (App->physics->Cannon2.vx > 0) {
				App->physics->Cannon2.x -= App->physics->Cannon2.vx;
			}
			else {
				App->physics->Cannon2.x += App->physics->Cannon2.vx;
			}

			App->physics->Cannon2.vx -= App->physics->Cannon2.ax;


			if (front2)
			{
				front2 = false;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x > 0)
			{
				if (App->physics->Cannon2.vx > 0) {
					App->physics->Cannon2.x -= App->physics->Cannon2.vx;
				}
				else {
					App->physics->Cannon2.x += App->physics->Cannon2.vx;
				}

				App->physics->Cannon2.vx -= App->physics->Cannon2.ax;
			}
		}

		break;
	case ModulePhysics::IMPULSE:

		break;
	case ModulePhysics::ACCELERATION:
		break;
	case ModulePhysics::FORCE:
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_UP || App->input->GetKey(SDL_SCANCODE_J) == KEY_UP) {
			App->physics->Cannon2.Fx = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && App->physics->angle2 <= 0) {
			if (!front2)
			{
				front2 = true;
				App->physics->angle2 = -180 - App->physics->angle2;
			}

			if (testPlayer2 == player2Status::STOP_PLAYER) {
				App->physics->Cannon2.Fx = 30;
			}
			else {
				App->physics->Cannon2.Fx = 15;
			}

		}
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && App->physics->angle2 <= 0) {
			if (front2)
			{
				front2 = false;
				App->physics->angle2 = -180 - App->physics->angle2;
			}

			if (testPlayer2 == player2Status::STOP_PLAYER) {
				App->physics->Cannon2.Fx = -30;
			}
			else {
				App->physics->Cannon2.Fx = -15;
			}

		}

		App->physics->Cannon2.ax = App->physics->Cannon2.Fx / App->physics->Cannon2.m;
		App->physics->Cannon2.vx += App->physics->Cannon2.ax;

		// Apply friction
		if (App->physics->Cannon2.vx > 0)
		{
			App->physics->Cannon2.vx -= FRICTION;
			if (App->physics->Cannon2.vx < 0)
				App->physics->Cannon2.vx = 0;
		}
		else if (App->physics->Cannon2.vx < 0)
		{
			App->physics->Cannon2.vx += FRICTION;
			if (App->physics->Cannon2.vx > 0)
				App->physics->Cannon2.vx = 0;
		}
		App->physics->Cannon2.x += App->physics->Cannon2.vx;


		break;

	}



	switch (testJump2)
	{
	case ModulePlayer2::JUMPX:
		App->physics->Cannon2.y -= 5;
		testPlayer2 = player2Status::GRAVITY;
		break;
	case ModulePlayer2::JUMPVEL:
		App->physics->Cannon2.y -= App->physics->Cannon2.jumpv;
		testPlayer2 = player2Status::GRAVITY;
		break;
	case ModulePlayer2::JUMPACC:
		App->physics->Cannon2.y -= App->physics->Cannon2.jumpv;
		App->physics->Cannon2.jumpv -= GRAVITY_;
		testPlayer2 = player2Status::GRAVITY;
		break;
	case ModulePlayer::JUMP_MOMENTUM:
		break;
	case ModulePlayer2::JUMP_IMPULSE:
		break;
	case ModulePlayer2::JUMP_ACCELERATION:
		if (App->physics->Cannon2.jumpa < 2) {
			App->physics->Cannon2.jumpa += 0.1;
		}
		App->physics->Cannon2.jumpv += App->physics->Cannon2.jumpa;
		App->physics->Cannon2.y -= App->physics->Cannon2.jumpv;

		testPlayer2 = player2Status::GRAVITY;
		break;
	case ModulePlayer2::JUMP_FORCE:
		if (testPlayer2 == player2Status::STOP_PLAYER) {
			App->physics->Cannon2.vy = -300;
		}
		
		testPlayer2 = player2Status::GRAVITY;
		break;
	case ModulePlayer2::NO_JUMP:
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
			jumpOption2 = jumpVal2;
		}
		break;

	}




	switch (testPlayer2)
	{
	case ModulePlayer2::STOP_PLAYER:
		//App->physics->Cannon.vy = 5;
		App->physics->Cannon2.jumpv = 30;
		App->physics->Cannon2.jumpa = 0;
		App->physics->Cannon2.vy = 0;
		if (App->physics->Cannon2.x > 200 && App->physics->Cannon2.x < 400) {
			testPlayer2 = player2Status::GRAVITY;
		}
		break;
	case ModulePlayer2::GRAVITY:
		App->physics->applyWind(&App->physics->Cannon2);

		if (App->physics->mode == 1)
			App->physics->euler(&App->physics->Cannon2);
		if (App->physics->mode == 2)
			App->physics->eulerSympletic(&App->physics->Cannon2);
		if (App->physics->mode == 3)
			App->physics->velocityVerlet(&App->physics->Cannon2);

		if (App->physics->Cannon2.y >= 300 && (App->physics->Cannon2.x <= 200 || App->physics->Cannon2.x >= 400)) {
			jumpOption2 = 7;
			testPlayer2 = player2Status::STOP_PLAYER;
			testJump2 = jump2Options::NO_JUMP;

		}

		break;
	}

	App->physics->resetForces(&App->physics->Cannon2);


	if (life2 > 0) {
		App->renderer->Blit(ball2, App->physics->bullet2.x, App->physics->bullet2.y, NULL);
		if (front2)
		{
			App->renderer->Blit(cannon2, App->physics->Cannon2.x, App->physics->Cannon2.y, NULL, NULL, App->physics->angle2, 25, 25);
		}
		else
		{
			App->renderer->BlitMirror(cannon2, App->physics->Cannon2.x - 25, App->physics->Cannon2.y, NULL, NULL, (App->physics->angle2 - 180), 75, 25);
		}
	}
	

	lifeBar2.x = App->physics->Cannon2.x;
	lifeBar2.y = App->physics->Cannon2.y - 15;
	App->renderer->DrawQuad(lifeBar2, 0, 255, 0, 255);

	// Debug draw

	player2Collider->SetPos(App->physics->Cannon2.x, App->physics->Cannon2.y);
	Player2Rect.x = App->physics->Cannon2.x;
	Player2Rect.y = App->physics->Cannon2.y;
	bullet2Collider->SetPos(App->physics->bullet2.x, App->physics->bullet2.y);
	bullet2Rect.x = App->physics->bullet2.x + 2;
	bullet2Rect.y = App->physics->bullet2.y + 2;

	if (debugCollider)
	{
		App->renderer->DrawQuad(Player2Rect, 0, 255, 0, 80);
		App->renderer->DrawQuad(bullet2Rect, 255, 0, 0, 80);
	}

	waitForDmg2++;
	if (waitForDmg2 >= 60) {
		canDmg2 = true;
		waitForDmg2 = 0;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer2::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::PLAYER2 && c2->type == Collider::Type::BULLET1) {
		if (canDmg2) {
			life2 -= dmg2;
			lifeBar2.w -= 10;
			canDmg2 = false;
		}

	}
}