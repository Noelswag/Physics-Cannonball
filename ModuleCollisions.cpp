#include "Globals.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::PLAYER1][Collider::Type::PLAYER1] = false;
	matrix[Collider::Type::PLAYER1][Collider::Type::PLAYER2] = true;
	matrix[Collider::Type::PLAYER1][Collider::Type::BULLET1] = false;
	matrix[Collider::Type::PLAYER1][Collider::Type::BULLET2] = true;
	matrix[Collider::Type::PLAYER1][Collider::Type::LIQUID_BODY] = true;
	matrix[Collider::Type::PLAYER1][Collider::Type::SOLID_BODY] = true;

	matrix[Collider::Type::PLAYER2][Collider::Type::PLAYER1] = true;
	matrix[Collider::Type::PLAYER2][Collider::Type::PLAYER2] = false;
	matrix[Collider::Type::PLAYER2][Collider::Type::BULLET1] = true;
	matrix[Collider::Type::PLAYER2][Collider::Type::BULLET2] = false;
	matrix[Collider::Type::PLAYER2][Collider::Type::LIQUID_BODY] = true;
	matrix[Collider::Type::PLAYER2][Collider::Type::SOLID_BODY] = true;


	matrix[Collider::Type::BULLET1][Collider::Type::PLAYER1] = false;
	matrix[Collider::Type::BULLET1][Collider::Type::PLAYER2] = true;
	matrix[Collider::Type::BULLET1][Collider::Type::BULLET1] = false;
	matrix[Collider::Type::BULLET1][Collider::Type::BULLET2] = true;
	matrix[Collider::Type::BULLET1][Collider::Type::LIQUID_BODY] = true;
	matrix[Collider::Type::BULLET1][Collider::Type::SOLID_BODY] = true;
}

ModuleCollisions::~ModuleCollisions()
{

}

bool ModuleCollisions::Start()
{
	return true;
}

update_status ModuleCollisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c1->listeners[i] != nullptr) c1->listeners[i]->OnCollision(c1, c2);

				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c2->listeners[i] != nullptr) c2->listeners[i]->OnCollision(c2, c1);
			}
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollisions::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (debug)
	{
		DebugDraw();

	}
	
	return UPDATE_CONTINUE;
}


update_status ModuleCollisions::PostUpdate()
{

	


	return UPDATE_CONTINUE;
}


void ModuleCollisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch(colliders[i]->type)
		{
			//I need to complete this
		case Collider::Type::PLAYER1: // blue
			App->renderer->DrawQuad(colliders[i]->rect, 255, 0, 0, 255);
			break;
		}
	}
}



bool ModuleCollisions::CleanUp()
{
	return true;
}


Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

void ModuleCollisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}