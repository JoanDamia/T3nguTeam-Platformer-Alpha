#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"

#include "iostream"

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

	this->Disable();
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 12, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	inAir = false; //Check if player is on the ground or not

	currentAnimation = &idleRightAnimation;

	idleRightAnimation.loop = idleLeftAnimation.loop = goRightAnimation.loop = goLeftAnimation.loop = true;
	idleRightAnimation.speed = idleLeftAnimation.speed = 0.05f;
	goRightAnimation.speed = goLeftAnimation.speed = 0.05f;

	for (int i = 0; i < 11; i++)//for porque vamos metiendo sprites (es un array) y le cambiamos el valor de x en cada iteración. Hasta llegar al sprite que queremos
	{
		idleRightAnimation.PushBack({ i * 32,0,24,26 });
	}

	for (int i = 0; i < 11; i++)
	{
		idleLeftAnimation.PushBack({ i * 32,26,24,26 });
	}

	for (int i = 0; i < 11; i++)
	{
	goRightAnimation.PushBack({ i * 32,62,24,28 });
	}

	for (int i = 0; i < 11; i++)
	{
		goLeftAnimation.PushBack({ i * 32,90,24,28 });
	}

	for (int i = 0; i < 2; i++)
	{
		jumpRightAnimation.PushBack({ i * 23,189,24,28 });
	}	
	for (int i = 0; i < 2; i++)
	{
		jumpLeftAnimation.PushBack({ i * 68,188,22,28 });
	}
	for (int i = 0; i < 2; i++)
	{
		fallRightAnimation.PushBack({ i * 22,188,23,28 });
	}
	for (int i = 0; i < 2; i++)
	{
		fallLeftAnimation.PushBack({ i * 45,188,23,28 });
	}

	//oposPlayer = position;

	oposPlayer = pbody->body->GetPosition();
	

	return true;
}



//Uint64 NOW = SDL_GetPerformanceCounter(); //tiempo de la primera ejecución sin el update

bool velNeg = false;
//bool doubleJump = true; 
Uint8 jumps = 2;
bool spacePressed = false;

bool Player::Update(float dt)
{

	Hurt();
	//Uint64 LAST = NOW; //cuando hacemos el update, en que tiempo estamos ahora (tic).
	//NOW = SDL_GetPerformanceCounter();

	//double deltaTime = ((NOW - LAST) / (double)SDL_GetPerformanceFrequency()); //Ene ste caso tenemos el último y el actual con la resta

	//current animation indica la animacion actual y no va a cambiar hasta que se otra animacion diferente

	// L07 DONE 5: Add physics to the player - updated player position using physics
	//atencio, s'ha comentat la segona línia després d'aquesta

	int speed = 0.75 * dt;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	


	//Move left

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

		pbody->body->SetLinearVelocity(b2Vec2(-speed, pbody->body->GetLinearVelocity().y));

		position.x -= 10;

		//lookLeft = true;

		if (currentAnimation != &goLeftAnimation && !inAir)
		{
			goLeftAnimation.Reset();
			currentAnimation = &goLeftAnimation;
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{

		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));

		if (currentAnimation != &idleLeftAnimation && !inAir)
		{
			idleLeftAnimation.Reset();
			currentAnimation = &idleLeftAnimation;
		}
	}



	//Move right

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		pbody->body->SetLinearVelocity(b2Vec2(speed, pbody->body->GetLinearVelocity().y));
		position.x += 10;

		//lookLeft = false;

		if (currentAnimation != &goRightAnimation && !inAir)
		{
			goRightAnimation.Reset();
			currentAnimation = &goRightAnimation;
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y)); //si y esta a 0 significa que esta al suelo

		if (currentAnimation != &idleRightAnimation && !inAir)
		{
			idleRightAnimation.Reset();
			currentAnimation = &idleRightAnimation;
		}
	}

	//el valor de velNeg depende antes de la comparación que cuando comprobamos que es negativo en ell mismo frame. 

	if (inAir && !velNeg && pbody->body->GetLinearVelocity().y == 0.0f) {
		inAir = false;
		jumps = 2;
		pbody->body->SetLinearVelocity(b2Vec2_zero);
		//se ejecuta antes del jump porque al pulsar espacio, no se ha aplicado la fuerza/actualizado la velocidad, por lo tanto es 0 y hay que esperar al siguiente frame. 
	}

	velNeg = pbody->body->GetLinearVelocity().y < 0.0f; //comprobamos si la velocidad es negativa para evitar que cuando el jugador se pare al chocar contra un muro superior continuie saltando continuamente mientras tengamos pulsado el espacio

	//jump
	//if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	//{
	//	if (!inAir)
	//	{
	//		pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x , 0 });
	//		pbody->body->ApplyForceToCenter({ 0, -jumpForce * (float32) deltaTime }, true); //cuando convetrimos podemos hacerlo como con el dt pero en c++ se puede hacer como float32(variable)
	//		//app->audio->PlayFx(jump_sound);
	//

	//		inAir = true;
	//	}

	//	else if (doubleJump) {
	//		pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x , 0 });
	//		pbody->body->ApplyForceToCenter({ 0, -jumpForce * (float32)deltaTime }, true); 
	//		//app->audio->PlayFx(jump_sound);
	//		doubleJump = false;
	//	}
	//
	//	if (currentAnimation != &jumpRightAnimation && !inAir)
	//	{
	//		jumpRightAnimation.Reset();
	//		currentAnimation = &jumpRightAnimation;
	//	}
	//}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (jumps != 0) {
			jumps--;
			pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x , 0 });
		    pbody->body->ApplyForceToCenter({ 0, -jumpForce * (float32) dt }, true); //cuando convetrimos podemos hacerlo como con el dt pero en c++ se puede hacer como float32(variable)
			//app->audio->PlayFx(jump_sound);
			inAir = true;
			jumpRightAnimation.Reset();
			currentAnimation = &jumpRightAnimation;
		}

	}

	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));

		//if (currentAnimation != &idleRightAnimation && !inAir)
		//{
		//	idleRightAnimation.Reset();
		//	currentAnimation = &idleRightAnimation;
		//}
	}

	

	if (pbody->body->GetLinearVelocity().x > -0.5f && pbody->body->GetLinearVelocity().x < 0.5f) {
		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));
	}

	//Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	SDL_Rect currentSprite = currentAnimation->GetCurrentFrame(); //dona el fram actual de l'animación actual. Después se debe actualizar al siguiente frame
	app->render->DrawTexture(texture, position.x, position.y, &currentSprite); //añadimos el current sprite para decirle que dibuje x rectangulo, current sprite lo pasamos en referencia.
	currentAnimation->Update(dt);//pasar al render
	

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
	case ColliderType::CHECKPOINT:
		LOG("Collision CHECKPOINT");
		lastCheckpoint = pbody->body->GetPosition();
			break;

	}



}

void Player::Hurt() {

	if (healthPoints == 0) {


	}

	if (position.y > 800) {
		pbody->body->SetTransform(oposPlayer, pbody->body->GetAngle());
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	}

	
}

bool Player::Load(pugi::xml_node& data)
{

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	position.x = data.child("player").child("position").attribute("x").as_int();
	position.y = data.child("player").child("position").attribute("y").as_int();

	pbody->body->SetTransform({ position.x + METERS_TO_PIXELS(w), position.y }, pbody->body->GetAngle());
	pbody->body->ApplyForceToCenter({ 0, 1 }, true);

	return true;
}

bool Player::Save(pugi::xml_node& data)
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	data.child("player").child("position").attribute("x").set_value(position.x);
	data.child("player").child("position").attribute("y").set_value(position.y);

	return true;
}