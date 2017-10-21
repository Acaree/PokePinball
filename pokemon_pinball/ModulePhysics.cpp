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
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

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
	
	b2Vec2 p[67];

	for (uint i = 0; i < 67; i++)
	{
		p[i].x = PIXEL_TO_METERS(pinball_map[2 * i]);
		p[i].y = PIXEL_TO_METERS(pinball_map[2 * i + 1]);
	}


	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(0,0);

	b2Body* pinball_world = world->CreateBody(&body);
	
	b2ChainShape shape;
	shape.CreateChain(p,67);
	
	

	b2FixtureDef fixture_world;
	fixture_world.shape = &shape;
	pinball_world->CreateFixture(&fixture_world);
	

	
	map = App->textures->Load("pinball/Map.png");
	

	CreateMapObstacles();
	
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

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

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

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
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

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
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

	b2Vec2 p[33];

	for (uint i = 0; i < 33; i++)
	{
		p[i].x = PIXEL_TO_METERS(slowbro_pixels[2 * i]);
		p[i].y = PIXEL_TO_METERS(slowbro_pixels[2 * i + 1]);
	}


	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(0, 0);

	b2Body* pinball_world = world->CreateBody(&body);

	b2ChainShape shape;
	shape.CreateChain(p, 33);



	b2FixtureDef fixture_world;
	fixture_world.shape = &shape;
	pinball_world->CreateFixture(&fixture_world);
	
		
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

	b2Vec2 p2[37];

	for (uint i = 0; i < 37; i++)
	{
		p2[i].x = PIXEL_TO_METERS(cloyster_pixels[2 * i]);
		p2[i].y = PIXEL_TO_METERS(cloyster_pixels[2 * i + 1]);
	}


	b2BodyDef body2;
	body2.type = b2_staticBody;
	body2.position.Set(0, 0);

	b2Body* pinball_world2 = world->CreateBody(&body2);

	b2ChainShape shape2;
	shape2.CreateChain(p2, 37);



	b2FixtureDef fixture_world2;
	fixture_world2.shape = &shape2;
	pinball_world2->CreateFixture(&fixture_world2);

	//left flipper

		int left_flipper_pixels[24] = {
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

		b2Vec2 p3[12];

		for (uint i = 0; i < 12; i++)
		{
			p3[i].x = PIXEL_TO_METERS(left_flipper_pixels[2 * i]);
			p3[i].y = PIXEL_TO_METERS(left_flipper_pixels[2 * i + 1]);
		}


		b2BodyDef body3;
		body3.type = b2_staticBody;
		body3.position.Set(0, 0);

		b2Body* pinball_world3 = world->CreateBody(&body3);

		b2ChainShape shape3;
		shape3.CreateChain(p3, 12);



		b2FixtureDef fixture_world3;
		fixture_world3.shape = &shape3;
		pinball_world3->CreateFixture(&fixture_world3);

		//right flipper

		int right_flipper_pixels[30] = {
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

		b2Vec2 p4[15];

		for (uint i = 0; i < 15; i++)
		{
			p4[i].x = PIXEL_TO_METERS(right_flipper_pixels[2 * i]);
			p4[i].y = PIXEL_TO_METERS(right_flipper_pixels[2 * i + 1]);
		}


		b2BodyDef body4;
		body4.type = b2_staticBody;
		body4.position.Set(0, 0);

		b2Body* pinball_world4 = world->CreateBody(&body4);

		b2ChainShape shape4;
		shape4.CreateChain(p4, 15);



		b2FixtureDef fixture_world4;
		fixture_world4.shape = &shape4;
		pinball_world4->CreateFixture(&fixture_world4);

	//little left

		int little_left[16] = {
			225, 82,
			221, 86,
			221, 140,
			226, 143,
			238, 143,
			242, 140,
			242, 85,
			238, 82
		};

		b2Vec2 p5[8];

		for (uint i = 0; i < 8; i++)
		{
			p5[i].x = PIXEL_TO_METERS(little_left[2 * i]);
			p5[i].y = PIXEL_TO_METERS(little_left[2 * i + 1]);
		}


		b2BodyDef body5;
		body5.type = b2_staticBody;
		body5.position.Set(0, 0);

		b2Body* pinball_world5 = world->CreateBody(&body5);

		b2ChainShape shape5;
		shape5.CreateChain(p5, 8);



		b2FixtureDef fixture_world5;
		fixture_world5.shape = &shape5;
		pinball_world5->CreateFixture(&fixture_world5);

		//little right

		int little_right[16] = {
			306, 82,
			303, 85,
			303, 140,
			306, 143,
			320, 143,
			323, 140,
			323, 85,
			320, 82
		};

		b2Vec2 p6[8];

		for (uint i = 0; i < 8; i++)
		{
			p6[i].x = PIXEL_TO_METERS(little_right[2 * i]);
			p6[i].y = PIXEL_TO_METERS(little_right[2 * i + 1]);
		}


		b2BodyDef body6;
		body6.type = b2_staticBody;
		body6.position.Set(0, 0);

		b2Body* pinball_world6 = world->CreateBody(&body6);

		b2ChainShape shape6;
		shape6.CreateChain(p6, 8);



		b2FixtureDef fixture_world6;
		fixture_world6.shape = &shape6;
		pinball_world6->CreateFixture(&fixture_world6);

		//red point

		int red_point[12] = {
			265, 945,
			266, 931,
			269, 928,
			277, 928,
			280, 932,
			280, 945
		};

		b2Vec2 p7[6];

		for (uint i = 0; i < 6; i++)
		{
			p7[i].x = PIXEL_TO_METERS(red_point[2 * i]);
			p7[i].y = PIXEL_TO_METERS(red_point[2 * i + 1]);
		}


		b2BodyDef body7;
		body7.type = b2_staticBody;
		body7.position.Set(0, 0);

		b2Body* pinball_world7 = world->CreateBody(&body7);

		b2ChainShape shape7;
		shape7.CreateChain(p7, 6);



		b2FixtureDef fixture_world7;
		fixture_world7.shape = &shape7;
		pinball_world7->CreateFixture(&fixture_world7);
}