#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void CreateMapObstacles();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type);

	PhysBody* left_flipper_anchor;
	PhysBody* left_flipper;
	PhysBody* right_flipper_anchor;
	PhysBody* right_flipper;
	PhysBody* left_triangle;
	PhysBody* right_triangle;
	PhysBody* left_bounce;
	PhysBody* right_bounce;
	PhysBody* shelder1;
	PhysBody* shelder2;
	PhysBody* shelder3;
	// b2ContactListener ---;
	void BeginContact(b2Contact* contact);

private:

	PhysBody *bouncy, *pivotBouncy;
	b2RevoluteJoint* joint_left_flipper;
	b2RevoluteJoint* joint_right_flipper;

	b2BodyDef dook;
	SDL_Texture* map;
	SDL_Texture* left_flipper_png;
	SDL_Texture* right_flipper_png;
	SDL_Texture* shelder_png;
	b2Body* body_clicked = nullptr;
	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};