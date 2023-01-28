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

	bool PostUpdate(float dt);

	bool CleanUp();
private:
	SDL_Texture* texture;
	const char* texturePath;


};

#endif // __BACKGROUND_H__


