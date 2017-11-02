#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

enum screen_state {
	screen_start = 1,
	screen_x1,
	screen_x2,
	screen_x3,
	screen_x5,
	screen_extralife,
	screen_gameover
};

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
	PhysBody* poliwag_button;
	PhysBody* psyduck_button;

	PhysBody* sensor_center;
	PhysBody* sensor_slow;
	PhysBody* sensor_cloy;
	PhysBody* sensor_pika;
	PhysBody* sensor_pika2;
	PhysBody* sensor_c;
	PhysBody* sensor_a;
	PhysBody* sensor_v;
	PhysBody* sensor_e;
	PhysBody* sensor_sh1;
	PhysBody* sensor_sh2;
	PhysBody* sensor_sh3;
	bool pokeball = true;

	SDL_Rect UI_Space;

	SDL_Texture* circle;
	uint theme;
	uint bouncer;
	uint buttons;
	uint flipper;
	uint pika;
	uint poliwag;
	uint psyduck;
	uint shelders;
	uint dook_down;
	uint dook_up;

	p2Point<int> ray;
	bool ray_on;

	screen_state screen;

	private:
		
		bool s1, s2, s3, t1, t2, b1, b2, pb, ps, se, ss, sc, sp, sp2, c, a, v, e, sh1, sh2, sh3;

		SDL_Texture* map;
		SDL_Texture* screen_sprite;

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

		SDL_Rect* cloyster;
		Animation* current_cloy;
		Animation cloy;

		SDL_Rect* left_button_rect;
		Animation* current_left_butt;
		Animation left_butt;
		Animation left_on;

		SDL_Rect* right_button_rect;
		Animation* current_right_butt;
		Animation right_butt;
		Animation right_on;

		SDL_Rect* poli_button;
		Animation* current_poli_butt;
		Animation poli_butt;
		Animation poli_on;

		SDL_Rect* psy_button;
		Animation* current_psy_butt;
		Animation psy_butt;
		Animation psy_on;

		SDL_Rect* pika_sensor;
		Animation* current_pika;
		Animation pika_sen;

		SDL_Rect* thun;
		Animation* current_thun;
		Animation thunder;

		SDL_Rect* c_sensor;
		Animation* current_c;
		Animation c_sen;
		Animation c_sor;

		SDL_Rect* a_sensor;
		Animation* current_a;
		Animation a_sen;
		Animation a_sor;

		SDL_Rect* v_sensor;
		Animation* current_v;
		Animation v_sen;
		Animation v_sor;

		SDL_Rect* e_sensor;
		Animation* current_e;
		Animation e_sen;
		Animation e_sor;

		SDL_Rect* sh_sensor;
		Animation* current_sh;
		Animation sh_sen;
		Animation sh_sor;

		SDL_Rect* left_arrows;
		Animation* current_left_arr;
		Animation left_arrow;

		SDL_Rect* mid_arrows;
		Animation* current_mid_arr;
		Animation mid_arrow;

		SDL_Rect* right_arrows;
		Animation* current_right_arr;
		Animation right_arrow;

		SDL_Rect* screens;
		Animation* current_screen;
		Animation start;
		Animation x1;
		Animation x2;
		Animation x3;
		Animation x5;
		Animation extra_life;
		Animation game_over;

		int multiplier;
};
