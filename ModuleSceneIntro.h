#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

#define BOUNCER_TIME 200

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

struct Light
{
	Light() : texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics;

	SDL_Texture* tex_light_tiny;
	SDL_Texture* tex_light_medium;
	SDL_Texture* tex_light_big;
	
	uint fx_light_tiny;
	uint fx_light_medium;
	uint fx_light_big;

	p2DynArray<Light> lights;

	uint player_lose_fx;

	Collider* block1;
	Collider* block2;
	Collider* block3;
	Collider* block4;
	Collider* block5;
	Collider* water;
	Collider* fall1;
	Collider* fall2;
	Collider* fall3;
	Collider* fall4;
	Collider* fall5;
	Collider* fall6;


	SDL_Rect block1rect;
	SDL_Rect block2rect;
	SDL_Rect block3rect;
	SDL_Rect block4rect;
	SDL_Rect block5rect;
	SDL_Rect waterRect;
	SDL_Rect fall1Rect;
	SDL_Rect fall2Rect;
	SDL_Rect fall3Rect;
	SDL_Rect fall4Rect;
	SDL_Rect fall5Rect;
	SDL_Rect fall6Rect;

	bool debugCollider = false;
};
