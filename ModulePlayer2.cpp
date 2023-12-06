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

	App->physics->Cannon2.x = 430;
	App->physics->Cannon2.vx = 5;
	App->physics->Cannon2.y = 300;
	App->physics->Cannon2.jumpv = 10;
	App->physics->Cannon2.jumpa = 0;
	App->physics->Cannon2.ax = 2;
	App->physics->Cannon2.m = 100;
	App->physics->Cannon2.force = -50;
	App->physics->Cannon2.surface = 50;
	App->physics->Cannon2.volumen = 200.0f;
	App->physics->Cannon2.Cd = 0.47f;

	App->physics->applyGravity(&App->physics->Cannon2);
	App->physics->applyAerodynamics(&App->physics->Cannon2);


	cannon2 = App->textures->Load("Graphics/Cannon2.png");
	ball2 = App->textures->Load("Graphics/Ball2.png");
	p1wins = App->textures->Load("Graphics/p1wins.png");

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

	

	


	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (App->physics->start2)
		{
			App->physics->bullet2.Fx = 0;
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
		if (App->physics->windActive) {
			App->physics->applyWind(&App->physics->bullet2);
		}
		

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
		App->physics->Cannon2.vx = 5;
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

		if (Collide2(Player2Rect, App->scene_intro->block1rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block2rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block3rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block4rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block5rect)) {
			App->physics->Cannon2.vx = 5;
		}

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
		

		break;
	case ModulePhysics::IMPULSE:

		break;
	case ModulePhysics::ACCELERATION:
		if (Collide2(Player2Rect, App->scene_intro->block1rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block2rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block3rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block4rect)) {
			App->physics->Cannon2.vx = 5;
		}
		if (Collide2(Player2Rect, App->scene_intro->block5rect)) {
			App->physics->Cannon2.vx = 5;
		}

		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && App->physics->angle2 <= 0) {

			if (!front2)
			{
				front2 = true;
				App->physics->angle2 = -180 - App->physics->angle2;
			}
			if (App->physics->Cannon2.x < SCREEN_WIDTH - 101)
			{
				App->physics->Cannon2.x += App->physics->Cannon2.vx;
				App->physics->Cannon2.vx += App->physics->Cannon2.ax;
				App->physics->Cannon2.ax += 0.5;
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
				if (App->physics->Cannon2.vx > 0) {
					App->physics->Cannon2.x -= App->physics->Cannon2.vx;
				}
				else {
					App->physics->Cannon2.x += App->physics->Cannon2.vx;
				}

				App->physics->Cannon2.vx -= App->physics->Cannon2.ax;
				App->physics->Cannon2.ax += 0.5;
			}
		}
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
				App->physics->Cannon2.Fx = 15;
			}
			else {
				App->physics->Cannon2.Fx = 7.5f;
			}

		}
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && App->physics->angle2 <= 0) {
			if (front2)
			{
				front2 = false;
				App->physics->angle2 = -180 - App->physics->angle2;
			}

			if (testPlayer2 == player2Status::STOP_PLAYER) {
				App->physics->Cannon2.Fx = -15;
			}
			else {
				App->physics->Cannon2.Fx = -7.5f;
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
		if (App->physics->Cannon2.jumpa < 0.15) {
			App->physics->Cannon2.jumpa += 0.01;
		}
		
		App->physics->Cannon2.y -= App->physics->Cannon2.jumpv;
		App->physics->Cannon2.jumpv += App->physics->Cannon2.jumpa;

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
		App->physics->Cannon2.jumpv = 5;
		App->physics->Cannon2.jumpa = 0;
		App->physics->Cannon2.vy = 0;
		App->physics->Cannon2.Fx = 0;
		
		break;
	case ModulePlayer2::GRAVITY:
		

		if (App->physics->mode == 1)
			App->physics->euler(&App->physics->Cannon2);
		if (App->physics->mode == 2)
			App->physics->eulerSympletic(&App->physics->Cannon2);
		if (App->physics->mode == 3)
			App->physics->velocityVerlet(&App->physics->Cannon2);


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
		lifeBar2.x = App->physics->Cannon2.x;
		lifeBar2.y = App->physics->Cannon2.y - 15;
		App->renderer->DrawQuad(lifeBar2, 0, 255, 0, 255);
	}
	else {
		App->renderer->Blit(p1wins, 0, 0, NULL);
	}
	

	

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

	if (Collide2(Player2Rect, App->scene_intro->block1rect)) {
		CollisionResolutionPlayer2(App->scene_intro->block1rect);
	}
	if (Collide2(Player2Rect, App->scene_intro->block2rect)) {
		CollisionResolutionPlayer2(App->scene_intro->block2rect);
	}
	if (Collide2(Player2Rect, App->scene_intro->block3rect)) {
		CollisionResolutionPlayer2(App->scene_intro->block3rect);
	}
	if (Collide2(Player2Rect, App->scene_intro->block4rect)) {
		CollisionResolutionPlayer2(App->scene_intro->block4rect);
	}
	if (Collide2(Player2Rect, App->scene_intro->block5rect)) {
		CollisionResolutionPlayer2(App->scene_intro->block5rect);
	}

	waitForDmg2++;
	if (waitForDmg2 >= 60) {
		canDmg2 = true;
		waitForDmg2 = 0;
	}

	if (App->physics->Cannon2.y > 400) {
		App->physics->Cannon2.y -= 25;
	}
	

	if ((App->physics->bullet2.x > (double)SCREEN_WIDTH || App->physics->bullet2.y > (double)SCREEN_HEIGHT || App->physics->bullet2.x < -25) && App->physics->flying2) {
		App->physics->flying2 = false;
		App->physics->start2 = true;
	}

	

	return UPDATE_CONTINUE;
}

void ModulePlayer2::CollisionResolutionPlayer2(SDL_Rect& r) {
	Vector2D objpos;
	Vector2D rpos;
	Vector2D diff;
	objpos.x = App->physics->Cannon2.x;
	objpos.y = App->physics->Cannon2.y;

	rpos.x = r.x;
	rpos.y = r.y;

	diff.x = rpos.x - objpos.x;
	diff.y = rpos.y - objpos.y;
	int colWidth, colHeight;

	// Calculate collision box
	if (diff.x > 0) {
		colWidth = Player2Rect.w - diff.x;
	}
	else {
		colWidth = r.w + diff.x;
	}


	if (diff.y > 0) {
		colHeight = Player2Rect.h - diff.y;
	}
	else {
		colHeight = r.h + diff.y;
	}

	// Reposition object
	if (colWidth < colHeight) {
		// Reposition by X-axis
		if (diff.x > 0) {
			App->physics->Cannon2.x -= colWidth;
		}
		else {
			App->physics->Cannon2.x += colWidth;
		}

		App->physics->Cannon2.vx = -App->physics->Cannon2.vx * 0.5f;
	}


}


bool ModulePlayer2::Collide2(SDL_Rect& r, SDL_Rect& r2)
{
	return (r2.x < r.x + r.w &&
		r2.x + r2.w > r.x &&
		r2.y < r.y + r.h &&
		r2.h + r2.y > r.y);
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

	if (c1->type == Collider::Type::PLAYER2 && c2->type == Collider::Type::FALL && testPlayer2 == player2Status::STOP_PLAYER) {
		testPlayer2 = player2Status::GRAVITY;
	}

	if (c1->type == Collider::Type::PLAYER2 && c2->type == Collider::Type::SOLID_BODY && testPlayer2 == player2Status::GRAVITY) {
		if (c1->rect.y < c2->rect.y) {
			testPlayer2 = player2Status::STOP_PLAYER;
			jumpOption2 = 7;
			testJump2 = jump2Options::NO_JUMP;
		}

	}

	if (c1->type == Collider::Type::PLAYER2 && c2->type == Collider::Type::LIQUID_BODY) {
		App->physics->applyHydrodynamics(&App->physics->Cannon2);
		testPlayer2 = player2Status::GRAVITY;
	}

	if (c1->type == Collider::Type::BULLET2 && c2->type == Collider::Type::SOLID_BODY) {
		if (c1->rect.y < c2->rect.y) {

			App->physics->bounceVertical(&App->physics->bullet2);
			if ((App->physics->bullet2.x > (double)SCREEN_WIDTH || App->physics->bullet2.y > (double)SCREEN_HEIGHT || App->physics->bullet2.x < -25 || App->physics->totalvelocity2 < 125) && App->physics->flying2) {
				App->physics->flying2 = false;
				App->physics->start2 = true;
			}
		}
		else if (c1->rect.y > c2->rect.y) {
			App->physics->bounceHorizontal(&App->physics->bullet2);
		}
	}
	else if (c1->type == Collider::Type::BULLET2 && c2->type == Collider::Type::LIQUID_BODY && App->physics->flying2 == true) {
		App->physics->applyHydrodynamics(&App->physics->bullet2);

		if (App->physics->bullet2.x > (double)SCREEN_WIDTH)
		{
			App->physics->flying2 = false;
			App->physics->start2 = true;
		}
	}
}