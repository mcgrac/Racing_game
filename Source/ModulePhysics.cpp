#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include <box2d/box2d.h>
#include "p2Point.h"

#include <math.h>

#pragma region PHYS_BODY
b2Vec2 PhysBody::GetPosition() const {
	b2Vec2 posMeters = body->GetPosition();
	return b2Vec2(METERS_TO_PIXELS(posMeters.x), METERS_TO_PIXELS(posMeters.y));
}

void PhysBody::SetPos(float xPixels, float yPixels) {
	if (body != nullptr)
	{
		b2Vec2 newPos(PIXEL_TO_METERS(xPixels), PIXEL_TO_METERS(yPixels));
		body->SetTransform(newPos, body->GetAngle());
		body->SetAwake(true);
	}
}

b2Body* PhysBody::GetB2Body() const
{
	return body;
}

// Aplicar impulso lineal al centro (en coordenadas Box2D)
void PhysBody::ApplyLinearImpulseToCenter(b2Vec2 impulse, bool wake)
{
	if (body != nullptr)
		body->ApplyLinearImpulseToCenter(impulse, wake);
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

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = (float)(x2 - x1);
			float fy = (float)(y2 - y1);
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return (int)(output.fraction * dist);
		}
		fixture = fixture->GetNext();
	}

	return ret;
}
#pragma endregion
ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	//set gravity to zero
	gravity.Set(0.0f, 0.0f);

	world = new b2World(b2Vec2(gravity));

	//for mouse joint
	b2BodyDef bd;
	mouseGround = world->CreateBody(&bd);

	world->SetContactListener(this);

	//world->SetAllowSleeping(true); //optimization
	//world->SetContinuousPhysics(true); //better physics detection at high speed
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	float dt = GetFrameTime();

	int32 velocityIterations = 8;   // velocity iterations accuraccy
	int32 positionIterations = 3;   // position iteratiuon accuracy

	//Update world
	world->Step(dt, velocityIterations, positionIterations);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	
	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}

	//UseMouseJoint();

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	if (world) {
		delete world;
		world = nullptr;
	}

	return true;
}

#pragma region CREATION PHYSBODIES
PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type)
{
	if (world == nullptr)
		return nullptr;

	// --- Do phys body to return it ---
	PhysBody* pbody = new PhysBody();

	// --- Body definition ---
	b2BodyDef bodyDef;
	bodyDef.type = type; // dynamic or static
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)); //set initial position
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* body = world->CreateBody(&bodyDef); //adds that body into the body list (allocates memory)

	// --- Shape definition ---
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	// --- Fixture definition ---
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.3f;
	fixture.restitution = 0.4f; // mid rebote

	body->CreateFixture(&fixture);


	pbody->body = body;
	pbody->width = radius * 2;
	pbody->height = radius * 2;


	return pbody;
}

PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	if (world == nullptr)
		return nullptr;

	// --- Do phys body to return it ---
	PhysBody* pbody = new PhysBody();

	// --- Body definition ---
	b2BodyDef bodyDef;
	bodyDef.type = type; // dynamic or static
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)); //set initial position
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	
	b2Body* body = world->CreateBody(&bodyDef); //adds that body into the body list (allocates memory)

	// --- Shape definition ---
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	// --- Fixture definition ---
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.3f;
	fixture.restitution = 0.4f; // mid rebote
	fixture.isSensor = true;

	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;
	fixture.filter.groupIndex = groupIndex;

	body->CreateFixture(&fixture);


	pbody->body = body;
	pbody->width = radius * 2;
	pbody->height = radius * 2;


	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, const int* points, int size, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex) {

	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&bodyDef);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (int i = 0; i < size / 2; ++i) {
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;
	fixture.filter.groupIndex = groupIndex;
	b->CreateFixture(&fixture);

	delete[] p;

	pbody->body = b;
	pbody->width = pbody->height = 0;
	return pbody;
}

PhysBody* ModulePhysics::CreateChainSensor(int x, int y, const int* points, int size, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&bodyDef);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (int i = 0; i < size / 2; ++i) {
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = true;

	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;
	fixture.filter.groupIndex = groupIndex;
	b->CreateFixture(&fixture);

	delete[] p;

	pbody->body = b;
	pbody->width = pbody->height = 0;
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	PhysBody* pbody = new PhysBody();
	pbody->listener = nullptr;

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(
		PIXEL_TO_METERS(x + width * 0.5f),
		PIXEL_TO_METERS(y + height * 0.5f)
	);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&bodyDef);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;		 //default density
	fixture.friction = 0.3f;     // default friction
	fixture.restitution = 0.0f;  // default bounce


	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;
	fixture.filter.groupIndex = groupIndex;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, b2BodyType type)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&bodyDef);
	b2PolygonShape box;
	//box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	box.SetAsBox(PIXEL_TO_METERS(width) * 1, PIXEL_TO_METERS(height) * 1);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	pbody->body = b;
	//pbody->width = (int)(width * 0.5f);
	//pbody->height = (int)(height * 0.5f);

	pbody->width = width;
	pbody->height = height;
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float rotation, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	bodyDef.angle = rotation * DEGTORAD;
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&bodyDef);
	b2PolygonShape box;
	//box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;
	fixture.filter.groupIndex = groupIndex;

	b->CreateFixture(&fixture);

	pbody->body = b;

	pbody->width = width;
	pbody->height = height;
	return pbody;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int count, b2BodyType type, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	// Crear body Box2D
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* body = world->CreateBody(&bodyDef);

	// Convertir puntos (int[]) → b2Vec2[]
	b2Vec2* vecs = new b2Vec2[count];
	for (int i = 0; i < count; i += 2)
	{
		vecs[i / 2].Set(PIXEL_TO_METERS(points[i]), PIXEL_TO_METERS(points[i + 1]));
	}

	b2PolygonShape shape;
	shape.Set(vecs, count / 2);

	b2FixtureDef fix;
	fix.shape = &shape;
	fix.density = 2.0f;
	fix.friction = 0.5f;
	fix.restitution = 0.1f;

	fix.filter.categoryBits = categoryBits;
	fix.filter.maskBits = maskBits;
	fix.filter.groupIndex = groupIndex;

	body->CreateFixture(&fix);

	delete[] vecs;

	// Conectar PhysBody
	PhysBody* pbody = new PhysBody();
	pbody->body = body;
	body->GetUserData().pointer = reinterpret_cast<uintptr_t>(pbody);

	return pbody;
}
#pragma endregion

void ModulePhysics::DestroyPhysBody(PhysBody* pbody)
{
	if (pbody == nullptr)
		return;

	if (pbody->body != nullptr)
	{
		world->DestroyBody(pbody->body);
		pbody->body = nullptr;
	}

	delete pbody;
}

void ModulePhysics::DrawDebug()
{
	if(debug)
	{
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();

					DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{ 0, 0, 0, 128 });
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->m_count;
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->m_vertices[i]);
						if (i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
				}
				break;
				}
			}
		}
	}
}

void ModulePhysics::UseMouseJoint(Camera2D camera)
{
	if (!debug) { return; }

	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
	float mouseX = mousePos.x;
	float mouseY = mousePos.y;

	b2Vec2 mouseWorld(
		PIXEL_TO_METERS(mouseX),
		PIXEL_TO_METERS(mouseY)
	);

	b2Body* mouseSelect = nullptr;

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetType() != b2_dynamicBody) continue;

		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
			if (mouseJoint == nullptr && mouseSelect == nullptr && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {

				if (f->TestPoint(mouseWorld)) {

					mouseSelect = b;
					break;
				}
			}
		}

		if (mouseSelect) break;
	}

	if (mouseSelect && mouseJoint == nullptr) {

		std::cout << "[MouseJoint] Creando MouseJoint\n";

		b2MouseJointDef def;
		def.bodyA = mouseGround;
		def.bodyB = mouseSelect;
		def.target = mouseWorld;
		def.damping = 0.5f;
		def.stiffness = 20.f;
		def.maxForce = 100.f * mouseSelect->GetMass();

		mouseJoint = (b2MouseJoint*)world->CreateJoint(&def);
		mouseSelect->SetAwake(true);
	}
	
	if (mouseJoint && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {

		mouseJoint->SetTarget(mouseWorld);
		b2Vec2 anchor = mouseJoint->GetAnchorB();
		DrawLine(
			METERS_TO_PIXELS(anchor.x),
			METERS_TO_PIXELS(anchor.y),
			mousePos.x,
			mousePos.y,
			RED
		);
	}
	
	if (mouseJoint && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

		world->DestroyJoint(mouseJoint);
		mouseJoint = nullptr;
	}
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	if (!contact) return;


	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (!fixtureA || !fixtureB) {
		std::cout << "ERROR: One of the fixtures is null!"<<std::endl;
		return;
	}


	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if (!bodyA || !bodyB) {
		std::cout << "ERROR: One of the bodies is null!" << std::endl;
		return;
	}


	b2BodyUserData dataA = bodyA->GetUserData();
	b2BodyUserData dataB = bodyB->GetUserData();

	PhysBody* physA = reinterpret_cast<PhysBody*>(dataA.pointer);
	PhysBody* physB = reinterpret_cast<PhysBody*>(dataB.pointer);


	if (!physA || !physB) {
		std::cout<< "WARNING: One PhysBody is null(might be ground / walls)" <<std::endl;
		return;
	}

	if (physA && physA->listener) {
		physA->listener->OnCollision(physA, physB);
	}


	if (physB && physB->listener) {
		physB->listener->OnCollision(physB, physA);
	}

}

void ModulePhysics::EndContact(b2Contact* contact)
{
	if (!contact) return;

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	PhysBody* physA = (PhysBody*)bodyA->GetUserData().pointer;
	PhysBody* physB = (PhysBody*)bodyB->GetUserData().pointer;

	if (physA && physA->listener)
		physA->listener->OnCollisionEnd(physA, physB);

	if (physB && physB->listener)
		physB->listener->OnCollisionEnd(physB, physA);
}