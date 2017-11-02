#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleUI.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	lifes = 3;
	LOG("Loading player");
	left_flipper_png = App->textures->Load("pinball/sprites/left_flipper.png");
	right_flipper_png = App->textures->Load("pinball/sprites/right_flipper.png");

	bouncy = App->physics->CreateRectangle(572, 921, 45, 6, b2_dynamicBody);
	pivotBouncy = App->physics->CreateRectangle(572, 944, 45, 6, b2_staticBody);

	//bouncy

	b2PrismaticJointDef dook1;
	dook1.collideConnected = true;
	dook1.bodyA = bouncy->body;
	dook1.bodyB = pivotBouncy->body;

	dook1.localAnchorA.Set(0, 0);
	dook1.localAnchorB.Set(0, -1);
	dook1.localAxisA.Set(0, -1);
	dook1.enableLimit = true;
	dook1.lowerTranslation = -0.02;
	dook1.upperTranslation = 1;
	(b2PrismaticJoint*)App->physics->world->CreateJoint(&dook1);


	//left flipper

	left_flipper_anchor = App->physics->CreateCircle(201, 880, 8, b2_staticBody);
	left_flipper = App->physics->CreateRectangle(350, 880, 63, 16, b2_dynamicBody);

	b2RevoluteJointDef left_joint_def;
	left_joint_def.bodyA = left_flipper->body;
	left_joint_def.bodyB = left_flipper_anchor->body;

	b2Vec2 LsetA = { -0.4f,0 };

	left_joint_def.localAnchorA.Set(LsetA.x, LsetA.y);
	left_joint_def.enableLimit = true;
	left_joint_def.lowerAngle = -30 * DEGTORAD;
	left_joint_def.upperAngle = 30 * DEGTORAD;

	joint_left_flipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_joint_def);


	//right flipper

	right_flipper_anchor = App->physics->CreateCircle(341, 880, 8, b2_staticBody);
	right_flipper = App->physics->CreateRectangle(250, 880, 63, 16, b2_dynamicBody);

	b2RevoluteJointDef right_joint_def;
	right_joint_def.bodyA = right_flipper->body;
	right_joint_def.bodyB = right_flipper_anchor->body;

	b2Vec2 RsetA = { 0.4f,0 };

	right_joint_def.localAnchorA.Set(RsetA.x, RsetA.y);
	right_joint_def.enableLimit = true;
	right_joint_def.lowerAngle = -30 * DEGTORAD;
	right_joint_def.upperAngle = 30 * DEGTORAD;

	joint_right_flipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_joint_def);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	App->renderer->Blit(left_flipper_png, 175, 865, NULL, 1.0f, left_flipper->GetRotation(), 17, 15);
	App->renderer->Blit(right_flipper_png, 288, 865, NULL, 1.0f, right_flipper->GetRotation(), 64, 15);

	bouncy->body->ApplyForce(b2Vec2(0, -5), b2Vec2(0, 0), true);

	left_flipper->body->ApplyForce(b2Vec2(0, -2), b2Vec2(0, 0), true);
	right_flipper->body->ApplyForce(b2Vec2(0, 2), b2Vec2(0, 0), true);

	if (App->scene_intro->screen != screen_start && App->scene_intro->screen != screen_gameover) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			bouncy->body->ApplyForce(b2Vec2(0, 10), b2Vec2(0, 0), true);
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			bouncy->body->ApplyForce(b2Vec2(0, -1250), b2Vec2(0, 0), true);
		}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
			joint_left_flipper->GetBodyA()->ApplyAngularImpulse(-7, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			joint_right_flipper->GetBodyA()->ApplyAngularImpulse(7, true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		if(App->scene_intro->screen == screen_gameover)
		App->scene_intro->screen = screen_start;
		
		else if (App->scene_intro->screen == screen_start)
			App->scene_intro->screen = screen_x1;
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		App->scene_intro->screen = screen_gameover;
	}

	return UPDATE_CONTINUE;
}