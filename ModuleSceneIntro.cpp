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

	ray_on = false;
	sensed = false;

	//map

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/Pokeball.png"); 
	bonus_fx = App->audio->LoadFx("changed because sound is anoying");
	right_light = App->textures->Load("pinball/right_light.png");
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	return ret;
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

	if (pokeball == true) {
		circles = App->physics->CreateCircle(570, 870, 22, b2_dynamicBody);
		pokeball = false;
	}
		int ballx, bally;
		circles->GetPosition(ballx, bally);
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || circles->body->GetPosition().y >= 0.02*1025)
	{
		App->renderer->Blit(right_light, 415, 718, NULL, 0.1f, App->physics->right_bounce->GetRotation());
		circles->body->DestroyFixture(circles->body->GetFixtureList());
		pokeball = true;
	}

		App->renderer->Blit(circle, ballx - 6, bally - 7, NULL, 1.0f, circles->GetRotation());

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if (bodyA->body == App->physics->right_bounce->body && bodyB->body == circles->body)
	{
		App->renderer->Blit(right_light, 415, 718, NULL, 0.1f, App->physics->right_bounce->GetRotation());
	}

}
