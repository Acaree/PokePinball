#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleUI.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	shelder.PushBack({ 857, 228, 54, 55 });
	shelder_bounce.PushBack({ 680, 1034, 55, 47 });

	right.PushBack({ 374,711, 51, 95 });
	right_light.PushBack({ 881,1656,51,95 });

	left.PushBack({ 119,711,51,95 });
	left_light.PushBack({ 575,1656,51,95 });

	slow.PushBack({279,1095,82,109});
	slow.PushBack({368,1095,81,109 });
	slow.speed = 0.03f;

	cloy.PushBack({728,1098,81,102});
	cloy.PushBack({816,1098,78,102});
	cloy.speed = 0.03f;

	right_butt.PushBack({ 758,1255,47,34 });
	right_on.PushBack({ 813,1255,47,34 });

	left_butt.PushBack({ 487,1255,47,34 });
	left_on.PushBack({ 541,1255,47,34 });

	poli_butt.PushBack({ 545, 1537, 20, 54 });
	poli_on.PushBack({ 572,1537,20,54 });

	psy_butt.PushBack({ 864, 1537, 37, 68 });
	psy_on.PushBack({ 901,1537,38,64 });

	pika_sen.PushBack({548,1853,54,54});
	pika_sen.PushBack({609,1853,48,54});
	pika_sen.speed = 0.027f;

	thunder.PushBack({ 721,1866,48,41 });
	thunder.PushBack({771,1866,48,41 });
	thunder.speed = 0.3f;

	c_sen.PushBack({636,1635,24,24});
	c_sor.PushBack({697,1635,24,24 });

	a_sen.PushBack({667,1635,23,24});
	a_sor.PushBack({728,1635,24,24 });

	v_sen.PushBack({ 636,1635,24,24 });
	v_sor.PushBack({758,1635,24,24 });

	e_sen.PushBack({ 667,1635,23,24 });
	e_sor.PushBack({789,1635,24,24 });

	sh_sen.PushBack({653,976,21,20});
	sh_sor.PushBack({680,976,21,20 });

	left_arrow.PushBack({609,1414,51,85});
	left_arrow.PushBack({609,1523,51,85});
	left_arrow.speed = 0.05f;

	mid_arrow.PushBack({667,1428,119,51});
	mid_arrow.PushBack({ 667,1537,119,51 });
	mid_arrow.speed = 0.045f;

	right_arrow.PushBack({792,1414,51,85 });
	right_arrow.PushBack({792,1523,51,85 });
	right_arrow.speed = 0.05f;

	//up.PushBack({ 612,1132,48,44 });
	//down.PushBack({612,1177,48,44});
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	
	bool ret = true;

	theme = App->audio->PlayMusic("pinball/music/theme.ogg");

	bouncer = App->audio->LoadFx("pinball/fx/bouncer.wav");
	buttons = App->audio->LoadFx("pinball/fx/buttons.wav");
	pika = App->audio->LoadFx("pinball/fx/pika.wav");
	flipper = App->audio->LoadFx("pinball/fx/flipper.wav");
	poliwag = App->audio->LoadFx("pinball/fx/poliwag.wav");
	psyduck = App->audio->LoadFx("pinball/fx/psyduck.wav");
	shelders = App->audio->LoadFx("pinball/fx/shelder.wav");

	circle = App->textures->Load("pinball/sprites/Pokeball.png"); 
	screen = App->textures->Load("pinball/maps/bonuses.png");
	map = App->textures->Load("pinball/maps/Map.png");

	int pinball_map[134] = {
		0, 0,
		0, 945,
		191, 945,
		75, 869,
		75, 891,
		24, 891,
		24, 701,
		32, 684,
		50, 666,
		68, 660,
		82, 659,
		82, 555,
		54, 517,
		39, 487,
		25, 435,
		21, 374,
		21, 292,
		26, 232,
		37, 182,
		55, 140,
		74, 113,
		106, 77,
		131, 57,
		153, 45,
		188, 28,
		245, 21,
		344, 21,
		404, 34,
		439, 48,
		464, 62,
		503, 91,
		539, 127,
		562, 162,
		588, 221,
		595, 252,
		595, 945,
		548, 945,
		548, 272,
		544, 253,
		536, 228,
		524, 198,
		507, 170,
		494, 153,
		494, 163,
		506, 190,
		511, 215,
		517, 241,
		522, 282,
		524, 300,
		524, 374,
		521, 408,
		516, 436,
		509, 462,
		503, 487,
		491, 511,
		463, 554,
		463, 658,
		477, 661,
		494, 666,
		510, 678,
		521, 701,
		521, 891,
		470, 891,
		470, 870,
		354, 945,
		653, 945,
		653, 0
	};

	PhysBody* body_map = App->physics->CreateChain(0, 0, pinball_map, 134, b2_staticBody);
	CreateMapObstacles();
	UI_Space= { 0, 0, SCREEN_WIDTH, 25};

	return true;
}



// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	
	App->renderer->Blit(map, 0, 0, NULL, 0, 0);

	//Shelders
	if (s1)
		current_shelder1 = &shelder_bounce;
	else
		current_shelder1 = &shelder;

	if (s2)
		current_shelder2 = &shelder_bounce;
	else
		current_shelder2 = &shelder;

	if (s3)
		current_shelder3 = &shelder_bounce;
	else
		current_shelder3 = &shelder;

	shelder1_1 = &current_shelder1->GetCurrentFrame();
	b2Vec2 pos_shelder1 = shelder1->body->GetPosition();
	App->renderer->Blit(map, METERS_TO_PIXELS(pos_shelder1.x) - 27, METERS_TO_PIXELS(pos_shelder1.y) - 27, shelder1_1);

	shelder1_2 = &current_shelder2->GetCurrentFrame();
	b2Vec2 pos_shelder2 = shelder2->body->GetPosition();
	App->renderer->Blit(map, METERS_TO_PIXELS(pos_shelder2.x) - 27, METERS_TO_PIXELS(pos_shelder2.y) - 27, shelder1_2);

	shelder1_3 = &current_shelder3->GetCurrentFrame();
	b2Vec2 pos_shelder3 = shelder3->body->GetPosition();
	App->renderer->Blit(map, METERS_TO_PIXELS(pos_shelder3.x) - 27, METERS_TO_PIXELS(pos_shelder3.y) - 27, shelder1_3);

	//Bouncers
	if (t1)
		current_right = &right_light;
	else
		current_right = &right;

	if (t2)
		current_left = &left_light;
	else
		current_left = &left;

	right_triangl = &current_right->GetCurrentFrame();
	App->renderer->Blit(map, 374, 711, right_triangl);

	left_triangl = &current_left->GetCurrentFrame();
	App->renderer->Blit(map, 119, 711, left_triangl);

	//Pikachu
	current_pika = &pika_sen;
	pika_sensor = &current_pika->GetCurrentFrame();
	App->renderer->Blit(map, 28, 834, pika_sensor);
	App->renderer->Blit(map, 472, 834, pika_sensor);
	current_thun = &thunder;
	thun = &current_thun->GetCurrentFrame();

	//Buttons
	if (b1)
		current_right_butt = &right_on;
	else
		current_right_butt = &right_butt;

	if (b2)
		current_left_butt = &left_on;
	else
		current_left_butt = &left_butt;

	right_button_rect = &current_right_butt->GetCurrentFrame();
	App->renderer->Blit(map, 378, 490, right_button_rect);

	left_button_rect = &current_left_butt->GetCurrentFrame();
	App->renderer->Blit(map, 120, 490, left_button_rect);

	//Poliwag red button
	if (pb)
		current_poli_butt = &poli_on;
	else
		current_poli_butt = &poli_butt;

	poli_button = &current_poli_butt->GetCurrentFrame();
	App->renderer->Blit(map, 82, 592, poli_button);

	//Psyduck button
	if (ps)
		current_psy_butt = &psy_on;
	else
		current_psy_butt = &psy_butt;

	psy_button = &current_psy_butt->GetCurrentFrame();
	App->renderer->Blit(map, 426, 578, psy_button);

	//Sensors
	if (se) {
		circles->body->GetLinearVelocity();
		if (circles->body->GetLinearVelocity().y > 0)
			circles->body->SetLinearVelocity(b2Vec2(0, 20));
		else
			circles->body->SetLinearVelocity(b2Vec2(1, -5));

	}

	if (ss) {
			circles->body->SetLinearVelocity(b2Vec2(-20, 0));

	}

	if (sc) {
			circles->body->SetLinearVelocity(b2Vec2(20, 0));
	}

	if (sp) {
		circles->body->SetLinearVelocity(b2Vec2(0, -15));
		App->renderer->Blit(map, 25, 842, thun);
	}

	if (sp2) {
		circles->body->SetLinearVelocity(b2Vec2(0, -15));
		App->renderer->Blit(map, 470, 836, thun);
	}

	//Cave sensors
	if (c)
		current_c = &c_sor;
	else
		current_c = &c_sen;

	c_sensor = &current_c->GetCurrentFrame();
	App->renderer->Blit(map, 34, 701, c_sensor);	

	if (a) 
		current_a = &a_sor;
	else
		current_a = &a_sen;

	a_sensor = &current_a->GetCurrentFrame();
	App->renderer->Blit(map, 89, 701, a_sensor);

	if (v)
		current_v = &v_sor;
	else
		current_v = &v_sen;

	v_sensor = &current_v->GetCurrentFrame();
	App->renderer->Blit(map, 432, 701, v_sensor);

	if (e)
		current_e = &e_sor;
	else
		current_e = &e_sen;

	e_sensor = &current_e->GetCurrentFrame();
	App->renderer->Blit(map, 487, 701, e_sensor);

	//Shelders sensors
	if (sh1)
		current_sh = &sh_sor;
	else
		current_sh = &sh_sen;

	sh_sensor = &current_sh->GetCurrentFrame();
	App->renderer->Blit(map, 181, 109, sh_sensor);

	if (sh2)
		current_sh = &sh_sor;
	else
		current_sh = &sh_sen;

	sh_sensor = &current_sh->GetCurrentFrame();
	App->renderer->Blit(map, 262, 82, sh_sensor);

	if (sh3)
		current_sh = &sh_sor;
	else
		current_sh = &sh_sen;

	sh_sensor = &current_sh->GetCurrentFrame();
	App->renderer->Blit(map, 344, 109, sh_sensor);

	//Arrows
	current_left_arr = &left_arrow;
	left_arrows = &current_left_arr->GetCurrentFrame();
	App->renderer->Blit(map, 106, 534, left_arrows);

	current_mid_arr = &mid_arrow;
	mid_arrows = &current_mid_arr->GetCurrentFrame();
	App->renderer->Blit(map, 215, 507, mid_arrows);

	current_right_arr = &right_arrow;
	right_arrows = &current_right_arr->GetCurrentFrame();
	App->renderer->Blit(map, 388, 534, right_arrows);



	//Pokeball
	if (pokeball == true) {
		circles = App->physics->CreateCircle(570, 870, 17, b2_dynamicBody);
		circles->listener = this;//clean
		pokeball = false;
	}
		int ballx, bally;
		circles->GetPosition(ballx, bally);
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || circles->body->GetPosition().y >= 0.02*1025)
	{
		//App->renderer->Blit(right_light, 415, 718, NULL, 0.1f, right_bounce->GetRotation());
		circles->body->DestroyFixture(circles->body->GetFixtureList());
		if (App->player->lifes > 0) {
			App->player->lifes -= 1;
		}
		pokeball = true;
	}
	////Up-down arrow
	//if (circles->body->GetWorldPoint().y > 357)
	//	current_up_down = &up;
	//else
	//	current_up_down = &down;

	//up_down = &current_up_down->GetCurrentFrame();
	//App->renderer->Blit(map,251,343,up_down);


		App->renderer->Blit(circle, ballx - 10, bally - 11, NULL, 1.0f, circles->GetRotation());	
		App->renderer->DrawQuad(UI_Space, 0, 0, 0, 255);

	if(App->input->GetKey(SDL_SCANCODE_Q)==KEY_DOWN|| App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		App->audio->PlayFx(flipper);

	//Slowbro
	current_slow = &slow;
	slowbro = &current_slow->GetCurrentFrame();
	App->renderer->Blit(map, 81, 323, slowbro);

	//Cloyster
	current_cloy = &cloy;
	cloyster = &current_cloy->GetCurrentFrame();
	App->renderer->Blit(map, 381, 303, cloyster);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		s1 = false;
		s2 = false;
		s3 = false;
		t1 = false;
		t2 = false;
		b1 = false;
		b2 = false;
		pb = false;
		ps = false;
		se = false;
		ss = false;
		sc = false;
		sp = false;
		sp2 = false;
	}

	if (bodyA->body == shelder1->body && bodyB->body == circles->body || bodyB->body == shelder1->body && bodyA->body == circles->body)
	{
		s1 = true;
		App->UI->score += 500;
		App->audio->PlayFx(shelders);
	}

	if (bodyA->body == shelder2->body && bodyB->body == circles->body || bodyB->body == shelder2->body && bodyA->body == circles->body)
	{
		s2 = true;
		App->UI->score += 500;
		App->audio->PlayFx(shelders);
	}

	if (bodyA->body == shelder3->body && bodyB->body == circles->body || bodyB->body == shelder3->body && bodyA->body == circles->body)
	{
		s3 = true;
		App->UI->score += 500;
		App->audio->PlayFx(shelders);
	}

	if (bodyA->body == right_bounce->body && bodyB->body == circles->body || bodyB->body == right_bounce->body && bodyA->body == circles->body)
	{
		t1 = true;
		App->UI->score += 300;
		App->audio->PlayFx(bouncer);
	}

	if (bodyA->body == left_bounce->body && bodyB->body == circles->body || bodyB->body == left_bounce->body && bodyA->body == circles->body)
	{
		t2 = true;
		App->UI->score += 300;
		App->audio->PlayFx(bouncer);
	}

	if (bodyA->body == right_button->body && bodyB->body == circles->body || bodyB->body == right_button->body && bodyA->body == circles->body)
	{
		b1 = true;
		App->UI->score += 100;
		App->audio->PlayFx(buttons);
	}

	if (bodyA->body == left_button->body && bodyB->body == circles->body || bodyB->body == left_button->body && bodyA->body == circles->body)
	{
		b2 = true;
		App->UI->score += 100;
		App->audio->PlayFx(buttons);
	}

	if (bodyA->body == poliwag_button->body && bodyB->body == circles->body || bodyB->body == poliwag_button->body && bodyA->body == circles->body)
	{
		pb = true;
		App->UI->score += 200;
		App->audio->PlayFx(poliwag);
	}

	if (bodyA->body == psyduck_button->body && bodyB->body == circles->body || bodyB->body == psyduck_button->body && bodyA->body == circles->body)
	{
		ps = true;
		App->UI->score += 200;
		App->audio->PlayFx(psyduck);
	}

	if (bodyA->body == sensor_center->body && bodyB->body == circles->body || bodyB->body == sensor_center->body && bodyA->body == circles->body)
	{
		se = true;
	}

	if (bodyA->body == sensor_slow->body && bodyB->body == circles->body || bodyB->body == sensor_slow->body && bodyA->body == circles->body)
	{
		ss = true;
		App->UI->score += 1000;
	}

	if (bodyA->body == sensor_cloy->body && bodyB->body == circles->body || bodyB->body == sensor_cloy->body && bodyA->body == circles->body)
	{
		sc = true;
		App->UI->score += 1000;
	}

	if (bodyA->body == sensor_pika->body && bodyB->body == circles->body || bodyB->body == sensor_pika->body && bodyA->body == circles->body)
	{
		sp = true;
		App->UI->score += 1000;
		App->audio->PlayFx(pika);
	}

	if (bodyA->body == sensor_pika2->body && bodyB->body == circles->body || bodyB->body == sensor_pika2->body && bodyA->body == circles->body)
	{
		sp2 = true;
		App->UI->score += 1000;
		App->audio->PlayFx(pika);
	}

	if (bodyA->body == sensor_c->body && bodyB->body == circles->body || bodyB->body == sensor_c->body && bodyA->body == circles->body)
	{
		c = true;
	}

	if (bodyA->body == sensor_a->body && bodyB->body == circles->body || bodyB->body == sensor_a->body && bodyA->body == circles->body)
	{
		a = true;
	}

	if (bodyA->body == sensor_v->body && bodyB->body == circles->body || bodyB->body == sensor_v->body && bodyA->body == circles->body)
	{
		v = true;
	}

	if (bodyA->body == sensor_e->body && bodyB->body == circles->body || bodyB->body == sensor_e->body && bodyA->body == circles->body)
	{
		e = true;
	}

	if (bodyA->body == sensor_sh1->body && bodyB->body == circles->body || bodyB->body == sensor_sh1->body && bodyA->body == circles->body)
	{
		sh1 = true;
	}

	if (bodyA->body == sensor_sh2->body && bodyB->body == circles->body || bodyB->body == sensor_sh2->body && bodyA->body == circles->body)
	{
		sh2 = true;
	}

	if (bodyA->body == sensor_sh3->body && bodyB->body == circles->body || bodyB->body == sensor_sh3->body && bodyA->body == circles->body)
	{
		sh3 = true;
	}

}

void ModuleSceneIntro::CreateMapObstacles() {

	//wall slowbro

	int slowbro_pixels[66]{
		154, 111,
		122, 143,
		102, 177,
		88, 216,
		78, 279,
		78, 388,
		84, 431,
		97, 470,
		115, 506,
		123, 518,
		131, 524,
		165, 508,
		165, 492,
		157, 468,
		153, 441,
		170, 415,
		199, 401,
		233, 384,
		243, 383,
		242, 379,
		108, 380,
		110, 332,
		244, 329,
		217, 316,
		175, 296,
		161, 288,
		148, 276,
		138, 261,
		135, 237,
		142, 211,
		155, 187,
		161, 178,
		161, 111
	};

	PhysBody* slowbro_wall = App->physics->CreateChain(0, 0, slowbro_pixels, 66, b2_staticBody);


	// wall cloyster

	int cloyster_pixels[74]{
		384, 112,
		384, 181,
		395, 198,
		405, 225,
		409, 254,
		396, 277,
		375, 291,
		346, 306,
		322, 319,
		300, 329,
		435, 328,
		435, 379,
		302, 379,
		312, 384,
		331, 393,
		365, 409,
		378, 418,
		388, 433,
		393, 446,
		391, 456,
		383, 478,
		380, 494,
		379, 508,
		410, 523,
		420, 519,
		426, 514,
		437, 494,
		453, 453,
		463, 410,
		466, 386,
		465, 278,
		458, 232,
		447, 191,
		434, 163,
		415, 137,
		398, 119,
		389, 112
	};

	PhysBody* cloyster_wall = App->physics->CreateChain(0, 0, cloyster_pixels, 74, b2_staticBody);

	//left flipper wall

	int left_flipper_wall_pixels[24] = {
		76, 708,
		72, 710,
		72, 803,
		83, 818,
		119, 842,
		180, 883,
		188, 865,
		128, 826,
		99, 805,
		83, 789,
		79, 782,
		79, 710
	};

	PhysBody* left_flipper_wall = App->physics->CreateChain(0, 0, left_flipper_wall_pixels, 24, b2_staticBody);

	//right flipper wall

	int right_flipper_wall_pixels[30] = {
		470, 708,
		473, 710,
		472, 803,
		462, 814,
		444, 830,
		431, 839,
		402, 858,
		365, 883,
		354, 864,
		392, 838,
		416, 823,
		444, 805,
		461, 788,
		465, 781,
		465, 710
	};

	PhysBody* right_flipper_wall = App->physics->CreateChain(0, 0, right_flipper_wall_pixels, 30, b2_staticBody);

	//little left wall

	int little_upleft_pixels[16] = {
		225, 82,
		221, 86,
		221, 140,
		226, 143,
		238, 143,
		242, 140,
		242, 85,
		238, 82
	};

	PhysBody* little_upleft_wall = App->physics->CreateChain(0, 0, little_upleft_pixels, 16, b2_staticBody);

	//little right

	int little_upright_pixels[16] = {
		306, 82,
		303, 85,
		303, 140,
		306, 143,
		320, 143,
		323, 140,
		323, 85,
		320, 82
	};

	PhysBody* little_upright_wall = App->physics->CreateChain(0, 0, little_upright_pixels, 16, b2_staticBody);

	//red point

	int red_point_pixels[10] = {
		273, 929,
		267, 933,
		267, 945,
		278, 945,
		278, 933
	};

	PhysBody* red_point_wall = App->physics->CreateChain(0, 0, red_point_pixels, 10, b2_staticBody);

	//left_triangle

	int left_triangle_pixels[8] = {
		125, 713,
		125, 764,
		167, 793,
		126, 713
	};

	left_triangle = App->physics->CreateChain(0, 0, left_triangle_pixels, 8, b2_staticBody);

	//right_triangle

	int right_triangle_pixels[8] = {
		420, 713,
		420, 764,
		377, 793,
		419, 713
	};

	right_triangle = App->physics->CreateChain(0, 0, right_triangle_pixels, 8, b2_staticBody);
	right_triangle->listener = this;

	//left_bounce

	int left_bounce_pixels[6] = {
		130, 718,
		130, 719,
		160, 780,

	};

	left_bounce = App->physics->CreateChain(0, 0, left_bounce_pixels, 6, b2_staticBody);
	left_bounce->listener = this;
	left_bounce->body->GetFixtureList()->SetRestitution(1.5f);

	//right_bounce

	int right_bounce_pixels[6] = {
		415, 718,
		415, 719,
		384, 780,

	};

	right_bounce = App->physics->CreateChain(0, 0, right_bounce_pixels, 6, b2_staticBody);
	right_bounce->listener = this;
	right_bounce->body->GetFixtureList()->SetRestitution(1.5f);

	//left button

	int left_button_pixels[6] = {
		138, 501,
		131, 525,
		165, 509,

	};

	left_button = App->physics->CreateChain(0, 0, left_button_pixels, 6, b2_staticBody);
	left_button->listener = this;

	//right button

	int right_button_pixels[6] = {
		404, 495,
		379, 509,
		410, 524,

	};

	right_button = App->physics->CreateChain(0, 0, right_button_pixels, 6, b2_staticBody);
	right_button->listener = this;

	//shelder
	current_shelder1 = &shelder;
	shelder1 = App->physics->CreateCircle(204, 238, 10.0f, b2_staticBody);
	shelder1->body->GetFixtureList()->SetRestitution(1.5f);
	shelder1->listener = this;

	current_shelder2 = &shelder;
	shelder2 = App->physics->CreateCircle(269, 188, 10.0f, b2_staticBody);
	shelder2->body->GetFixtureList()->SetRestitution(1.5f);
	shelder2->listener = this;

	current_shelder3 = &shelder;
	shelder3 = App->physics->CreateCircle(334, 238, 10.0f, b2_staticBody);
	shelder3->body->GetFixtureList()->SetRestitution(1.5f);
	shelder3->listener = this;

	//Poliwag button

	current_poli_butt = &poli_butt;
	poliwag_button = App->physics->CreateRectangle(92, 619, 10, 54, b2_staticBody);
	poliwag_button->body->GetFixtureList()->SetRestitution(0.5f);
	poliwag_button->listener = this;

	//Psyduck button

	current_psy_butt = &psy_butt;
	psyduck_button = App->physics->CreateRectangle(455, 608, 10, 54, b2_staticBody);
	psyduck_button->body->GetFixtureList()->SetRestitution(0.5f);
	psyduck_button->listener = this;

	//Sensors

	sensor_center = App->physics->CreateRectangleSensor(271, 354, 10, 20);
	sensor_slow = App->physics->CreateRectangleSensor(135, 354, 50, 20);
	sensor_cloy = App->physics->CreateRectangleSensor(410, 354, 50, 20);
	sensor_pika = App->physics->CreateRectangleSensor(48, 867, 50, 50);
	sensor_pika2 = App->physics->CreateRectangleSensor(494, 867, 50, 50);
	sensor_c = App->physics->CreateRectangleSensor(48, 745, 14, 20);
	sensor_a = App->physics->CreateRectangleSensor(103, 745, 13, 20);
	sensor_v = App->physics->CreateRectangleSensor(443, 745, 13, 20);
	sensor_e = App->physics->CreateRectangleSensor(497, 745, 14, 20);
	sensor_sh1 = App->physics->CreateRectangleSensor(191, 143, 14, 20);
	sensor_sh2 = App->physics->CreateRectangleSensor(273, 116, 13, 20);
	sensor_sh3 = App->physics->CreateRectangleSensor(354, 143, 14, 20);
}
