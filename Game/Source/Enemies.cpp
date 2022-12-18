//#include "App.h"
//#include "Textures.h"
//#include "Audio.h"
//#include "Render.h"
//#include "Window.h"
//#include "Scene.h"
//#include "Map.h"
//#include "Physics.h"
//#include "Player.h"
//#include "Enemies.h"
//#include "Pathfinding.h"
//
//
//#include "Defs.h"
//#include "Log.h"
//
//#include <time.h>
//
//Enemies::Enemies() : Module()
//{
//	name.Create("enemies");
//	int pix = 64;
//
//	//Pig Animation
//	currentPigAnimation = &idlePigRightAnimation;
//
//
//	idlePigRightAnimation.loop = idlePigLeftAnimation.loop = goPigRightAnimation.loop = goPigLeftAnimation.loop = true;
//	idlePigRightAnimation.speed = idlePigLeftAnimation.speed = 16.0f;
//	goPigRightAnimation.speed = goPigLeftAnimation.speed = 25.0f;
//
//
//	jumpPigRightAnimation.PushBack({ 0,188,22,28 });
//
//	jumpPigLeftAnimation.PushBack({ 68,188,22,28 });
//
//	fallPigRightAnimation.PushBack({ 22,188,23,28 });
//
//	fallPigLeftAnimation.PushBack({ 45,188,23,28 });
//
//
//	PigDeathLAnim.loop = PigDeathRAnim.loop = false;
//
//	PigDeathLAnim.speed = PigDeathRAnim.speed = 11.0f;
//
//
//	for (int i = 0; i < 11; i++)
//	{
//		idlePigRightAnimation.PushBack({ i * 32,0,22,26 });
//	}
//
//	for (int i = 0; i < 11; i++)
//	{
//		idlePigLeftAnimation.PushBack({ i * 32,26,22,26 });
//	}
//
//	for (int i = 0; i < 11; i++)
//	{
//		goPigRightAnimation.PushBack({ i * 32,62,24,28 });
//	}
//
//	for (int i = 0; i < 11; i++)
//	{
//		goPigLeftAnimation.PushBack({ i * 32,90,24,28 });
//	}
//
//	//Bat
//
//	currentBatAnimation = &idleBatRightAnimation;
//
//
//	idleBatRightAnimation.loop = idleBatLeftAnimation.loop = goBatRightAnimation.loop = goBatLeftAnimation.loop = true;
//	idleBatRightAnimation.speed = idleBatLeftAnimation.speed = 16.0f;
//	goBatRightAnimation.speed = goBatLeftAnimation.speed = 25.0f;
//
//
//	jumpBatRightAnimation.PushBack({ 0,188,22,28 });
//
//	jumpBatLeftAnimation.PushBack({ 68,188,22,28 });
//
//	fallBatRightAnimation.PushBack({ 22,188,23,28 });
//
//	fallBatLeftAnimation.PushBack({ 45,188,23,28 });
//
//
//	BatDeathLAnim.loop = BatDeathRAnim.loop = false;
//
//	BatDeathLAnim.speed = BatDeathRAnim.speed = 11.0f;
//
//
//	for (int i = 0; i < 11; i++)
//	{
//		idleBatRightAnimation.PushBack({ i * 32,0,22,26 });
//	}
//
//	for (int i = 0; i < 11; i++)
//	{
//		idleBatLeftAnimation.PushBack({ i * 32,26,22,26 });
//	}
//
//	for (int i = 0; i < 11; i++)
//	{
//		goBatRightAnimation.PushBack({ i * 32,62,24,28 });
//	}
//
//	for (int i = 0; i < 11; i++)
//	{
//		goBatLeftAnimation.PushBack({ i * 32,90,24,28 });
//	}
//
//}
//
//// Destructor
//Enemies::~Enemies()
//{}
//
//// Called before render is available
//bool Enemies::Awake()
//{
//	srand(time(NULL));
//
//	return true;
//}
//
//// Called before the first frame
//bool Enemies::Start()
//{
//	pig = app->tex->Load("Assets/textures/pig.png");
//	bat = app->tex->Load("Assets/textures/bat.png");
//
//	return true;
//}
//
//// Called each loop iteration
//bool Player::PreUpdate()
//{
//	position.x = pbody->body->GetPosition().x;
//	position.y = pbody->body->GetPosition().y;
//
//
//	return true;
//}
//
//bool Player::Update()
//{
//
//	// L07 DONE 5: Add physics to the player - updated player position using physics
//	//atencio, s'ha comentat la segona línia després d'aquesta
//
//	int speed = 5;
//	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
//
//	
//
//	//Update player position in pixels
//	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
//	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
//
//	app->render->DrawTexture(texture, position.x, position.y);
//
//	return true;
//}
//
//bool Enemies::CleanUp()
//{
//	return true;
//}
//
//
//void Enemies::CreateGroundEnemy(float x, float y)
//{
//	Enemy* new_enemy = new Enemy();
//
//	// body
//	b2BodyDef e_body;
//	e_body.type = b2_dynamicBody;
//	e_body.fixedRotation = true;
//	e_body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	new_enemy->body = app->physics->world->CreateBody(&e_body);
//	new_enemy->body->SetFixedRotation(true);
//
//	b2PolygonShape box;
//	box.SetAsBox(PIXEL_TO_METERS(new_enemy->w), PIXEL_TO_METERS(new_enemy->h));
//
//	b2FixtureDef fixture;
//	fixture.shape = &box;
//	fixture.density = 1.0f;
//	fixture.friction = 0.0f;
//	b2Fixture* bodyFixture = new_enemy->body->CreateFixture(&fixture);
//	bodyFixture->SetSensor(false);
//	bodyFixture->SetUserData((void*)4);
//
//	// ground sensor
//	box.SetAsBox(PIXEL_TO_METERS((new_enemy->w)), PIXEL_TO_METERS(4), b2Vec2(0, PIXEL_TO_METERS(-20)), 0);
//	fixture.isSensor = true;
//	b2Fixture* sensorFixture = new_enemy->body->CreateFixture(&fixture);
//	sensorFixture->SetUserData((void*)9); // hit sensor
//
//	// stats
//	new_enemy->origin_x = PIXEL_TO_METERS(x);
//	new_enemy->origin_y = PIXEL_TO_METERS(y);
//	new_enemy->x = PIXEL_TO_METERS(x);
//	new_enemy->y = PIXEL_TO_METERS(y);
//	new_enemy->speed = 0.05f;
//
//	new_enemy->type = ENEMY_TYPE::GROUND;
//
//	new_enemy->currentAnimation = &goPigRightAnimation;
//
//	new_enemy->lookLeft = true;
//
//	new_enemy->detectionRange = 5.0f;
//	new_enemy->enemy_spoted = false;
//
//	new_enemy->state = ENEMY_STATE::IDLE;
//	new_enemy->obLeft = false;
//
//	enemies.Insert(*new_enemy, enemies.Count());
//}
//
//void Enemies::CreateAirEnemy(float x, float y)
//{
//	Enemy* new_enemy = new Enemy();
//
//	// body
//	b2BodyDef e_body;
//	e_body.type = b2_dynamicBody;
//	e_body.fixedRotation = true;
//	e_body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//	e_body.gravityScale = 0.0f;
//
//	new_enemy->body = app->physics->world->CreateBody(&e_body);
//	new_enemy->body->SetFixedRotation(true);
//
//	b2PolygonShape box;
//	box.SetAsBox(PIXEL_TO_METERS(new_enemy->w), PIXEL_TO_METERS(new_enemy->h));
//
//	b2FixtureDef fixture;
//	fixture.shape = &box;
//	fixture.density = 1.0f;
//	fixture.friction = 0.0f;
//	b2Fixture* bodyFixture = new_enemy->body->CreateFixture(&fixture);
//	bodyFixture->SetSensor(false);
//	bodyFixture->SetUserData((void*)4); // player collision
//
//	// ground sensor
//	box.SetAsBox(PIXEL_TO_METERS((new_enemy->w)), PIXEL_TO_METERS(4), b2Vec2(0, PIXEL_TO_METERS(-20)), 0);
//	fixture.isSensor = true;
//	b2Fixture* sensorFixture = new_enemy->body->CreateFixture(&fixture);
//	sensorFixture->SetUserData((void*)9); // ground sensor
//
//	// stats
//	new_enemy->origin_x = PIXEL_TO_METERS(x);
//	new_enemy->origin_y = PIXEL_TO_METERS(y);
//	new_enemy->x = PIXEL_TO_METERS(x);
//	new_enemy->y = PIXEL_TO_METERS(y);
//	new_enemy->speed = 0.05f;
//
//	new_enemy->type = ENEMY_TYPE::AIR;
//
//	new_enemy->currentAnimation = &idleBatRightAnimation;
//
//	new_enemy->lookLeft = true;
//
//	new_enemy->detectionRange = 5.0f;
//	new_enemy->enemy_spoted = false;
//
//	new_enemy->state = ENEMY_STATE::IDLE;
//
//	enemies.Insert(*new_enemy, enemies.Count());
//}
//
//
//
//bool Enemies::Load(pugi::xml_node& data)
//{
//	for (size_t i = 0; i < data.attribute("value").as_int(); i++)
//	{
//		std::string p = "position";
//		std::string s = std::to_string(i);
//		std::string t = p + s;
//		const char* c = t.c_str();
//
//		if (data.child(c).attribute("state").as_int() == 0)
//		{
//
//			enemies.At(i)->x = data.child(c).attribute("x").as_int();
//			enemies.At(i)->y = data.child(c).attribute("y").as_int();
//
//			enemies.At(i)->body->SetTransform({ enemies.At(i)->x + PIXEL_TO_METERS(enemies.At(i)->w), enemies.At(i)->y }, enemies.At(i)->body->GetAngle());
//			enemies.At(i)->body->ApplyForceToCenter({ 0, 200 }, true);
//		}
//		else
//		{
//			enemies.At(i)->state = ENEMY_STATE::DEATH;
//		}
//	}
//
//	return true;
//}
//
//bool Enemies::Save(pugi::xml_node& data)
//{
//	for (size_t i = 0; i < enemies.Count(); i++)
//	{
//		std::string p = "position";
//		std::string s = std::to_string(i);
//		std::string t = p + s;
//		const char* c = t.c_str();
//
//		data.child(c).attribute("x").set_value(enemies.At(i)->x);
//		data.child(c).attribute("y").set_value(enemies.At(i)->y);
//
//		if (enemies.At(i)->state != ENEMY_STATE::DEATH)
//		{
//			data.child(c).attribute("state").set_value("0");
//		}
//		else
//		{
//			data.child(c).attribute("state").set_value("1");
//		}
//	}
//
//	data.attribute("value").set_value(enemies.Count());
//
//	return true;
//}
