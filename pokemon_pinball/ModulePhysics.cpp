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

	int pinball_map[190] = {
		0, 0,
		0, 945,
		191, 945,
		179, 934,
		157, 921,
		133, 905,
		115, 892,
		95, 878,
		79, 867,
		72, 867,
		72, 888,
		28, 888,
		28, 707,
		39, 686,
		51, 676,
		64, 670,
		82, 667,
		82, 554,
		68, 535,
		61, 521,
		51, 505,
		42, 486,
		34, 457,
		31, 435,
		27, 409,
		23, 373,
		22, 295,
		28, 265,
		31, 237,
		34, 218,
		40, 191,
		50, 165,
		62, 144,
		74, 121,
		91, 102,
		107, 87,
		122, 75,
		140, 61,
		164, 47,
		189, 36,
		211, 32,
		246, 22,
		343, 22,
		369, 27,
		394, 32,
		415, 39,
		438, 49,
		459, 62,
		482, 76,
		498, 89,
		522, 113,
		546, 141,
		560, 165,
		572, 188,
		581, 212,
		590, 239,
		595, 253,
		595, 945,
		548, 945,
		548, 272,
		540, 238,
		533, 218,
		524, 195,
		516, 179,
		506, 166,
		497, 153,
		493, 153,
		493, 164,
		498, 174,
		504, 191,
		511, 218,
		517, 266,
		521, 300,
		521, 375,
		517, 409,
		510, 457,
		500, 488,
		490, 511,
		481, 528,
		463, 554,
		463, 667,
		487, 672,
		500, 681,
		507, 687,
		511, 697,
		515, 706,
		517, 711,
		517, 888,
		473, 888,
		473, 867,
		467, 867,
		354, 942,
		354, 945,
		654, 945,
		654, 0
	};



	
	b2Vec2 p[95];

	for (uint i = 0; i < 95; i++)
	{
		p[i].x = PIXEL_TO_METERS(pinball_map[2 * i]);
		p[i].y = PIXEL_TO_METERS(pinball_map[2 * i + 1]);
	}


	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(0,0);

	b2Body* pinball_world = world->CreateBody(&body);
	
	b2ChainShape shape;
	shape.CreateChain(p,95);
	
	

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

	int slowbro_pixels[118]{
		154, 109,
		142, 119,
		127, 134,
		115, 150,
		98, 177,
		92, 194,
		84, 219,
		79, 255,
		74, 278,
		74, 396,
		78, 418,
		85, 457,
		93, 473,
		103, 498,
		114, 515,
		122, 518,
		126, 524,
		136, 524,
		147, 520,
		159, 514,
		167, 507,
		167, 492,
		163, 477,
		160, 466,
		154, 455,
		157, 449,
		160, 439,
		167, 429,
		177, 421,
		191, 414,
		204, 407,
		218, 402,
		232, 395,
		245, 388,
		245, 378,
		109, 378,
		109, 337,
		136, 330,
		164, 330,
		163, 334,
		191, 334,
		191, 337,
		218, 337,
		218, 340,
		245, 340,
		245, 327,
		237, 323,
		218, 313,
		196, 302,
		176, 292,
		162, 286,
		152, 279,
		142, 266,
		139, 255,
		140, 245,
		147, 217,
		157, 197,
		164, 187,
		164, 109,
	};

	b2Vec2 p[59];

	for (uint i = 0; i < 59; i++)
	{
		p[i].x = PIXEL_TO_METERS(slowbro_pixels[2 * i]);
		p[i].y = PIXEL_TO_METERS(slowbro_pixels[2 * i + 1]);
	}


	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(0, 0);

	b2Body* pinball_world = world->CreateBody(&body);

	b2ChainShape shape;
	shape.CreateChain(p, 59);



	b2FixtureDef fixture_world;
	fixture_world.shape = &shape;
	pinball_world->CreateFixture(&fixture_world);
	
		
	// wall cloyster

		int cloyster_pixels[114]{
			381, 108,
			381, 188,
			387, 198,
			395, 211,
			398, 218,
			401, 231,
			405, 244,
			405, 255,
			398, 272,
			381, 286,
			368, 293,
			354, 300,
			335, 309,
			313, 320,
			300, 327,
			299, 340,
			327, 340,
			327, 337,
			354, 337,
			354, 334,
			381, 334,
			381, 330,
			408, 330,
			436, 327,
			436, 378,
			300, 378,
			300, 389,
			314, 394,
			326, 401,
			339, 407,
			346, 412,
			361, 419,
			374, 425,
			381, 432,
			388, 448,
			388, 455,
			381, 477,
			378, 494,
			378, 507,
			385, 514,
			398, 521,
			408, 524,
			419, 524,
			428, 517,
			439, 503,
			449, 483,
			459, 455,
			465, 419,
			470, 394,
			470, 279,
			463, 234,
			453, 194,
			438, 164,
			425, 143,
			412, 130,
			401, 119,
			391, 109
	};

	b2Vec2 p2[57];

	for (uint i = 0; i < 57; i++)
	{
		p2[i].x = PIXEL_TO_METERS(cloyster_pixels[2 * i]);
		p2[i].y = PIXEL_TO_METERS(cloyster_pixels[2 * i + 1]);
	}


	b2BodyDef body2;
	body2.type = b2_staticBody;
	body2.position.Set(0, 0);

	b2Body* pinball_world2 = world->CreateBody(&body2);

	b2ChainShape shape2;
	shape2.CreateChain(p2, 57);



	b2FixtureDef fixture_world2;
	fixture_world2.shape = &shape2;
	pinball_world2->CreateFixture(&fixture_world2);
	
}