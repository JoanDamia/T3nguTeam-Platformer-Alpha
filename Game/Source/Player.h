#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();

	virtual ~Player();

	bool Awake();

	bool Start();


	bool PreUpdate();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);
	PhysBody* pbody;




public:

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body


	bool djump = true;
	bool inAir = true;
	float jumpForce = 25.0f;
	int w = 20, h = 32;

	int pickCoinFxId;

};

#endif // __PLAYER_H__