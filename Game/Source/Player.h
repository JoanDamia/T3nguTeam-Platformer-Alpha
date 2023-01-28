#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);
	PhysBody* pbody;


	Animation idleRightAnimation;
	Animation idleLeftAnimation;
	Animation goRightAnimation;
	Animation goLeftAnimation;
	Animation jumpRightAnimation;
	Animation jumpLeftAnimation;
	Animation fallRightAnimation;
	Animation fallLeftAnimation;


public:

private:

	void Hurt();

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	//Current animation
	Animation* currentAnimation = nullptr;

	// L07 DONE 5: Add physics to the player - declare a Physics body

	
	Uint8 healthPoints = 3;
	bool djump = true;
	bool inAir = true;
	float jumpForce = 20.0f;
	int w = 20, h = 32;
	b2Vec2
		oposPlayer,
		lastCheckpoint;


	int pickCoinFxId;

};

#endif // __PLAYER_H__