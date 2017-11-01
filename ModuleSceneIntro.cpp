#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	shelder.PushBack({ 857, 228, 54, 55 });
	shelder_bounce.PushBack({ 680, 1034, 55, 47 });

	right.PushBack({ 374,711, 51, 95 });
	right_light.PushBack({ 881,1656,51,95 });

	left.PushBack({ 119,711,51,95 });
	left_light.PushBack({ 575,1656,51,95 });
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	
	bool ret = true;

	circle = App->textures->Load("pinball/Pokeball.png"); 
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	sensor = App->physics->CreateRectangleSensor(50,100, SCREEN_WIDTH, 50);

	map = App->textures->Load("pinball/Map.png");

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
	if (b1)
		current_right = &right_light;
	else
		current_right = &right;

	if (b2)
		current_left = &left_light;
	else
		current_left = &left;

	right_triangl = &current_right->GetCurrentFrame();
	App->renderer->Blit(map, 374, 711, right_triangl);

	left_triangl = &current_left->GetCurrentFrame();
	App->renderer->Blit(map, 119, 711, left_triangl);

	//Pokeball
	if (pokeball == true) {
		circles = App->physics->CreateCircle(570, 870, 22, b2_dynamicBody);
		circles->listener = this;//clean
		pokeball = false;
	}
		int ballx, bally;
		circles->GetPosition(ballx, bally);
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || circles->body->GetPosition().y >= 0.02*1025)
	{
		//App->renderer->Blit(right_light, 415, 718, NULL, 0.1f, right_bounce->GetRotation());
		circles->body->DestroyFixture(circles->body->GetFixtureList());
		pokeball = true;
	}

		App->renderer->Blit(circle, ballx - 6, bally - 7, NULL, 1.0f, circles->GetRotation());	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	s1 = false;
	s2 = false;
	s3 = false;
	b1 = false;
	b2 = false;

	if (bodyA->body == shelder1->body && bodyB->body == circles->body || bodyB->body == shelder1->body && bodyA->body == circles->body)
	{
		s1 = true;
	}

	if (bodyA->body == shelder2->body && bodyB->body == circles->body || bodyB->body == shelder2->body && bodyA->body == circles->body)
	{
		s2 = true;
	}

	if (bodyA->body == shelder3->body && bodyB->body == circles->body || bodyB->body == shelder3->body && bodyA->body == circles->body)
	{
		s3 = true;
	}

	if (bodyA->body == right_bounce->body && bodyB->body == circles->body || bodyB->body == right_bounce->body && bodyA->body == circles->body)
	{
		b1 = true;
	}
	
	if (bodyA->body == left_bounce->body && bodyB->body == circles->body || bodyB->body == left_bounce->body && bodyA->body == circles->body)
	{
		b2 = true;
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
}
