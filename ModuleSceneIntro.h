#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;

class ModuleSceneIntro : public Module, public b2ContactListener
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void CreateMapObstacles();

public:
	PhysBody* circles;
	PhysBody* left_triangle;
	PhysBody* right_triangle;
	PhysBody* left_bounce;
	PhysBody* right_bounce;
	PhysBody* shelder1;
	PhysBody* shelder2;
	PhysBody* shelder3;

	PhysBody* sensor;
	bool sensed;
	bool pokeball = true;;

	SDL_Texture* circle;
	SDL_Texture* right_light;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	private:
		
		SDL_Texture* map;
		SDL_Texture* shelder_png;
};
