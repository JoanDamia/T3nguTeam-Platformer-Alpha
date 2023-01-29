#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "Entity.h"
#include "SDL/include/SDL.h"


class Background : public Entity
{
public:

	Background();

	virtual ~Background();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void FadeOut(float dt),
		FadeIn(float dt);



	float speed = 0.1f;
	float alpha = 0;

private:
	SDL_Texture* texture;
	const char* texturePath;
	int
		sizeX,
		sizeY;
	




};

#endif // __BACKGROUND_H__


