#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	inAir = false; //Check if player is on the ground or not

	currentAnimation = &idleRightAnimation;

	idleRightAnimation.loop = idleLeftAnimation.loop = goRightAnimation.loop = goLeftAnimation.loop = true;
	idleRightAnimation.speed = idleLeftAnimation.speed = 16.0f;
	goRightAnimation.speed = goLeftAnimation.speed = 25.0f;

	for (int i = 0; i < 11; i++)
	{
		idleRightAnimation.PushBack({ i * 32,0,22,26 });
	}

	for (int i = 0; i < 11; i++)
	{
		idleLeftAnimation.PushBack({ i * 32,26,22,26 });
	}

	for (int i = 0; i < 11; i++)
	{
	goRightAnimation.PushBack({ i * 32,62,24,28 });
	}

	for (int i = 0; i < 11; i++)
	{
		goLeftAnimation.PushBack({ i * 32,90,24,28 });
	}

	jumpRightAnimation.PushBack({ 0,188,22,28 });

	jumpLeftAnimation.PushBack({ 68,188,22,28 });

	fallRightAnimation.PushBack({ 22,188,23,28 });

	fallLeftAnimation.PushBack({ 45,188,23,28 });

	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	position.x = pbody->body->GetPosition().x;
	position.y = pbody->body->GetPosition().y;


	return true;
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics
	//atencio, s'ha comentat la segona línia després d'aquesta

	int speed = 5;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	/*
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}*/

	//Move left
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed, -GRAVITY_Y);

		if (currentAnimation != &goLeftAnimation && !inAir)
		{
			goLeftAnimation.Reset();
			currentAnimation = &goLeftAnimation;
		}
	}

	/*if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		pbody->body->SetLinearVelocity({ -speed, pbody->body->GetLinearVelocity().y });

	}*/

	//MoveRight
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed, -GRAVITY_Y);

		if (currentAnimation != &goRightAnimation && !inAir)
		{
			goRightAnimation.Reset();
			currentAnimation = &goRightAnimation;
		}
	}

	//Jump
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (!inAir)
		{
			pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0));
			pbody->body->ApplyForceToCenter({ 0, -100 }, true);
			//app->audio->PlayFx(jump_sound);
			inAir = true;
			djump = true;
		}

		if (currentAnimation != &jumpRightAnimation && !inAir)
		{
			jumpRightAnimation.Reset();
			currentAnimation = &jumpRightAnimation;
		}
	}

	if (pbody->body->GetLinearVelocity().x > -0.5f && pbody->body->GetLinearVelocity().x < 0.5f) {
		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}

bool Player::Load(pugi::xml_node& data)
{

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	//position.x = data.child("player").child("position").attribute("x").as_int();
	//position.y = data.child("player").child("position").attribute("y").as_int();

	//body->SetTransform({ position.x + METERS_TO_PIXELS(w), position.y }, body->GetAngle());
	//body->ApplyForceToCenter({ 0, 1 }, true);

	return true;
}

bool Player::Save(pugi::xml_node& data)
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	//data.child("player").child("position").attribute("x").set_value(position.x);
	//data.child("player").child("position").attribute("y").set_value(position.y);

	return true;
}