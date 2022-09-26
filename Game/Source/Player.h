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

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update();

	bool CleanUp();

public:

	//L02: TODO 1: Declare player parameters
	iPoint pos;
	const char* texturePath;
	SDL_Texture* texture;
};

#endif // __PLAYER_H__