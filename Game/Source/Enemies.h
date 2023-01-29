#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Entity.h"
#include "SDL/include/SDL.h"


class Enemies : public Entity
{
public:

	Enemies();

	virtual ~Enemies();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	float speed = 0.1f;
	float alpha = 0;

	float positionX, positionY;

private:
	SDL_Texture* texture;
	const char* texturePath;
	int
		sizeX,
		sizeY;





};

#endif // __ENEMIES_H__


