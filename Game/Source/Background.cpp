#include "Background.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "iostream"



Background::Background() : Entity(EntityType::BACKGROUND)
{
	name.Create("background");
}

Background::~Background() {

}



bool Background::Start() {

	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);
	SDL_QueryTexture(texture, NULL, NULL, &sizeX, &sizeY);
	sizeX /= 2;
	sizeY /= 2;
	return true;

}

bool Background::Awake() {
	//initilize textures
	return true;
}

bool Background::PostUpdate(float dt) {

	SDL_Rect windowsSize = app->render->GetViewPort();
	app->render->DrawRectangle(windowsSize, 0,0,0);
	position.x = (windowsSize.w / 2.0f) - sizeX;
	position.y = (windowsSize.h / 2.0f) - sizeY;
	app->render->DrawTexture(texture, position.x, position.y);
	

	return true;

}

bool Background::CleanUp() {
	return true;
}