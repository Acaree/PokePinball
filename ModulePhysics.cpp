#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{

	map = App->textures->Load("pinball/Map.png");
	left_flipper_png = App->textures->Load("pinball/left_flipper.png");
	right_flipper_png = App->textures->Load("pinball/right_flipper.png");

	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	
	

	//bouncy

	bouncy = CreateRectangle(572, 921, 45, 6, b2_dynamicBody);
	pivotBouncy = CreateRectangle(572, 944, 45, 6, b2_staticBody);

	b2PrismaticJointDef dook;
	dook.collideConnected = true;
	dook.bodyA = bouncy->body;
	dook.bodyB = pivotBouncy->body;

	dook.localAnchorA.Set(0, 0);
	dook.localAnchorB.Set(0, -1);
	dook.localAxisA.Set(0, -1);
	dook.enableLimit = true;
	dook.lowerTranslation = -0.02;
	dook.upperTranslation = 1;
	(b2PrismaticJoint*)world->CreateJoint(&dook);

	//left flipper

	left_flipper_anchor = CreateCircle(201,880,8,b2_staticBody);
	left_flipper = CreateRectangle(350, 880, 64, 16, b2_dynamicBody);

	b2RevoluteJointDef left_joint_def;
	left_joint_def.bodyA = left_flipper->body;
	left_joint_def.bodyB = left_flipper_anchor->body;

	b2Vec2 LsetA = { -0.4f,0 };

	left_joint_def.localAnchorA.Set(LsetA.x, LsetA.y);
	left_joint_def.enableLimit = true;
	left_joint_def.lowerAngle = -30 * DEGTORAD;
	left_joint_def.upperAngle = 30 * DEGTORAD;

	joint_left_flipper = (b2RevoluteJoint*)world->CreateJoint(&left_joint_def);

	//right flipper

	right_flipper_anchor = CreateCircle(341, 880, 8, b2_staticBody);
	right_flipper = CreateRectangle(250, 880, 64, 16, b2_dynamicBody);

	b2RevoluteJointDef right_joint_def;
	right_joint_def.bodyA = right_flipper->body;
	right_joint_def.bodyB = right_flipper_anchor->body;

	b2Vec2 RsetA = { 0.4f,0 };

	right_joint_def.localAnchorA.Set(RsetA.x, RsetA.y);
	right_joint_def.enableLimit = true;
	right_joint_def.lowerAngle = -30 * DEGTORAD;
	right_joint_def.upperAngle = 30 * DEGTORAD;

	joint_right_flipper = (b2RevoluteJoint*)world->CreateJoint(&right_joint_def);

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
	App->physics->CreateMapObstacles();
	
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}


	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update() {



	App->renderer->Blit(map, 0, 0, NULL, 0, 0);
	App->renderer->Blit(left_flipper_png, 175, 865, NULL, 1.0f, left_flipper->GetRotation(),17,15);
	App->renderer->Blit(right_flipper_png, 288, 865, NULL, 1.0f, right_flipper->GetRotation(), 64, 15);

	App->physics->bouncy->body->ApplyForce(b2Vec2(0, -5), b2Vec2(0, 0), true);

	App->physics->left_flipper->body->ApplyForce(b2Vec2(0, -2), b2Vec2(0, 0), true);
	App->physics->right_flipper->body->ApplyForce(b2Vec2(0, 2), b2Vec2(0, 0), true);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->physics->bouncy->body->ApplyForce(b2Vec2(0, 10), b2Vec2(0, 0), true);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		App->physics->bouncy->body->ApplyForce(b2Vec2(0, -1250), b2Vec2(0, 0), true);
	}
		
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		joint_left_flipper->GetBodyA()->ApplyAngularImpulse(-7, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		joint_right_flipper->GetBodyA()->ApplyAngularImpulse(7, true);
	}

	

 	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.bullet = true;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;


	
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	b2Vec2 mouse_position(App->input->GetMouseX(), App->input->GetMouseY());
	mouse_position.x = PIXEL_TO_METERS(mouse_position.x);
	mouse_position.y = PIXEL_TO_METERS(mouse_position.y);
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
				if (f->GetShape()->TestPoint(b->GetTransform(), mouse_position)) {
					// test if the current body contains mouse position
					if (body_clicked == nullptr) {
						body_clicked = b;
					}
				}
			
			}
		}
	}

	

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::CreateMapObstacles() {

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

	PhysBody* slowbro_wall = CreateChain(0, 0, slowbro_pixels, 66, b2_staticBody);
	
		
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

		PhysBody* cloyster_wall = CreateChain(0, 0, cloyster_pixels, 74, b2_staticBody);

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

		PhysBody* left_flipper_wall = CreateChain(0, 0, left_flipper_wall_pixels, 24, b2_staticBody);

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

		PhysBody* right_flipper_wall = CreateChain(0, 0, right_flipper_wall_pixels, 30, b2_staticBody);

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

		PhysBody* little_upleft_wall = CreateChain(0, 0, little_upleft_pixels, 16, b2_staticBody);

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

		PhysBody* little_upright_wall = CreateChain(0, 0, little_upright_pixels, 16, b2_staticBody);

		//red point

		int red_point_pixels[10] = {
			273, 929,
			267, 933,
			267, 945,
			278, 945,
			278, 933
		};

		PhysBody* red_point_wall = CreateChain(0, 0, red_point_pixels, 10, b2_staticBody);

		//left_triangle

		int left_triangle_pixels[8] = {
			125, 713,
			125, 764,
			167, 793,
			126, 713
		};

		left_triangle = CreateChain(0, 0, left_triangle_pixels, 8, b2_staticBody);

		//right_triangle

		int right_triangle_pixels[8] = {
			420, 713,
			420, 764,
			377, 793,
			419, 713
		};

		right_triangle = CreateChain(0, 0, right_triangle_pixels, 8, b2_staticBody);

		//left_bounce

		int left_bounce_pixels[6] = {
			130, 718,
			130, 719,
			160, 780,

		};

		left_bounce = CreateChain(0, 0, left_bounce_pixels, 6, b2_staticBody);

		left_bounce->body->GetFixtureList()->SetRestitution(1.5f);

		//right_bounce

		int right_bounce_pixels[6] = {
			415, 718,
			415, 719,
			384, 780,

		};

		right_bounce = CreateChain(0, 0, right_bounce_pixels, 6, b2_staticBody);

		right_bounce->body->GetFixtureList()->SetRestitution(1.5f);
}