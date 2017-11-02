#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
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
	PhysBody* left_button;
	PhysBody* right_button;
	PhysBody* shelder1;
	PhysBody* shelder2;
	PhysBody* shelder3;

	PhysBody* sensor;
	bool pokeball = true;

	SDL_Rect UI_Space;

	SDL_Texture* circle;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	private:
		
		bool s1, s2, s3, t1, t2, b1, b2;

		SDL_Texture* map;

		SDL_Rect* shelder1_1;
		SDL_Rect* shelder1_2;
		SDL_Rect* shelder1_3;
		Animation* current_shelder1;
		Animation* current_shelder2;
		Animation* current_shelder3;
		Animation shelder;
		Animation shelder_bounce;

		SDL_Rect* right_triangl;
		Animation* current_right;
		Animation right;
		Animation right_light;

		SDL_Rect* left_triangl;
		Animation* current_left;
		Animation left;
		Animation left_light;

		SDL_Rect* slowbro;
		Animation* current_slow;
		Animation slow;
		//Animation bro;

		SDL_Rect* cloyster;
		Animation* current_cloy;
		Animation cloy;
		//Animation ster;

		SDL_Rect* left_button_rect;
		Animation* current_left_butt;
		Animation left_butt;
		Animation left_on;

		SDL_Rect* right_button_rect;
		Animation* current_right_butt;
		Animation right_butt;
		Animation right_on;
};
