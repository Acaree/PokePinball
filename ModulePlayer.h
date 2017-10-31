#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"

class ModulePlayer : public Module, public b2ContactListener
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	PhysBody* left_flipper_anchor;
	PhysBody* left_flipper;
	PhysBody* right_flipper_anchor;
	PhysBody* right_flipper;

private:
	PhysBody *bouncy;
	PhysBody *pivotBouncy;
	b2RevoluteJoint* joint_left_flipper;
	b2RevoluteJoint* joint_right_flipper;

	b2BodyDef dook;
	SDL_Texture* left_flipper_png;
	SDL_Texture* right_flipper_png;

};