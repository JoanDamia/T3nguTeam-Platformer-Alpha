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
	return true;

}

bool Background::Awake() {
	//initilize textures
	return true;
}

bool Background::PostUpdate(float dt) {

	//position.x = app->render->GetViewPort().w / 2.0f;
	//position.y = app->render->GetViewPort().h / 2.0f;
	//app->render->DrawTexture(texture, position.x, position.y);
	

	return true;

}

bool Background::CleanUp() {
	return true;
}