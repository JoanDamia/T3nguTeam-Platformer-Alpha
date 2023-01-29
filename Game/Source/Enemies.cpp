#include "Enemies.h"
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

Enemies::Enemies() : Entity(EntityType::ENEMIES)
{
	name.Create("Enmeies");
}

Enemies::~Enemies() {

}

bool Enemies::Awake() {
	return true;
}

bool Enemies::Start() {
	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);
	SDL_QueryTexture(texture, NULL, NULL, &sizeX, &sizeY);
	sizeX /= 2;
	sizeY /= 2;
	return true;

}





bool Enemies::Update(float dt) {

	SDL_Rect windowsSize = app->render->GetViewPort();
	app->render->DrawRectangle(windowsSize, 0, 0, 0);
	position.x = (windowsSize.w / 2.0f) - sizeX;
	position.y = (windowsSize.h / 2.0f) - sizeY;
	app->render->DrawTexture(texture, position.x, position.y);
	app->render->DrawRectangle(windowsSize, 0, 0, 0, alpha);
	return true;

}

bool Enemies::CleanUp() {
	return true;
}

