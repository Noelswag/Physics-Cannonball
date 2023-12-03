#pragma once
#include "Module.h"
#define MAX_COLLIDERS 1000
#include "Collider.h"



class ModuleCollisions : public Module
{
private:
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];

public:
	ModuleCollisions(Application* app, bool start_enabled = true);
	virtual ~ModuleCollisions();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

	// Removes the collider memory and removes it from the colliders array
	void RemoveCollider(Collider* collider);

	// Draws all existing colliders with some transparency
	void DebugDraw();

	

	// Simple debugging flag to draw all colliders
	bool debug = false;

};