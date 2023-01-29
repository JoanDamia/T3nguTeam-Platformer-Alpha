#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	//player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//player->parameters = config.child("player");

	background = (Background*)app->entityManager->CreateEntity(EntityType::BACKGROUND);//creamos entidad background
	background->parameters = config.child("background");
	menu = (Background*)app->entityManager->CreateEntity(EntityType::BACKGROUND);//creamos entidad background
	menu->parameters = config.child("menu");
	menu->Disable();
	menu->alpha = 255.0f;


	background->Disable();
	menu->Enable();
	currentScene = MAINMENU;
	menu->alpha = 0.0f;

	SDL_Rect rect;
	rect.x = 450;
	rect.y = 400;
	rect.w = 55;
	rect.h = 10;
	newGame = new GuiButton(0, rect, "NEWGAME");

	rect.x = 450;
	rect.y = 450;
	rect.w = 55;
	rect.h = 10;
	_continue = new GuiButton(1, rect, "CONTINUE");

	rect.x = 450;
	rect.y = 500;
	rect.w = 55;
	rect.h = 10;
	settings = new GuiButton(2, rect, "SETTINGS");
	settings->text = "settings";
	settings->SetObserver(this);

	rect.x = 450;
	rect.y = 500;
	rect.w = 55;
	rect.h = 10;
	musicVolume = new GuiButton(3, rect, "MUSIC");

	rect.x = 450;
	rect.y = 500;
	rect.w = 55;
	rect.h = 10;
	fxVolume = new GuiButton(4, rect, "FX");

	rect.x = 450;
	rect.y = 500;
	rect.w = 55;
	rect.h = 10;
	fullscreen = new GuiButton(5, rect, "FULLSCREEN");

	rect.x = 450;
	rect.y = 500;
	rect.w = 55;
	rect.h = 10;
	vsync = new GuiButton(6, rect, "VSYNC");

	rect.x = 450;
	rect.y = 550;
	rect.w = 55;
	rect.h = 10;
	back = new GuiButton(7, rect, "BACK");

	rect.x = 450;
	rect.y = 550;
	rect.w = 55;
	rect.h = 10;
	exit = new GuiButton(8, rect, "EXIT");

	rect.x = 450;
	rect.y = 600;
	rect.w = 55;
	rect.h = 10;
	credits = new GuiButton(9, rect, "CREDITS");

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	img = app->tex->Load("Assets/Textures/test.png");
	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->Load();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate(float dt)
{
	switch (currentScene) {
	case LOGO:
		break;
	case MAINMENU:
		break;
	case LVL1:
		break;
	case LVL2:
		break;
	}

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	switch (currentScene) {
	case LOGO:
		background->FadeOut(dt);
		if (background->alpha == 255.0f) {
			currentScene = (Scenes)(currentScene + 1);
			background->alpha = 0.0f;
			background->Disable();
			menu->Enable();
		}
		break;
	case MAINMENUENTRY:
		menu->FadeIn(dt);
		if (menu->alpha == 0.0f) {
			currentScene = (Scenes)(currentScene + 1);
			
		}
	case MAINMENU:
		musicVolume->state = GuiControlState::DISABLED;
		fxVolume->state = GuiControlState::DISABLED;
		fullscreen->state = GuiControlState::DISABLED;
		vsync->state = GuiControlState::DISABLED;

		break;
	case SETTINGS:
		musicVolume->state = GuiControlState::NORMAL;
		fxVolume->state = GuiControlState::NORMAL;
		fullscreen->state = GuiControlState::NORMAL;
		vsync->state = GuiControlState::NORMAL;
		back->state = GuiControlState::NORMAL;
		_continue->state = GuiControlState::DISABLED;
		newGame->state = GuiControlState::DISABLED;
		credits->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;



	}


	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	// L14: DONE 4: Make the camera movement independent of framerate
	float speed = 0.2 * dt;
	/*
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1; 
		*/

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	//Make the camera follow the player
	uint x, y;
	app->win->GetWindowSize(x, y);
	//app->render->camera.x = -(app->scene->player->position.x + (x / 2));

	//app->render->camera.x = -app->scene->player->position.x + (x / 2);

	if (app->render->camera.x > 0) {
		app->render->camera.x = 0;
	}
	if (app->render->camera.x < -5600) {
		app->render->camera.x = -5600;
	}

	
	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate(float dt)
{

	newGame->Update(dt);
	newGame->Draw(app->render);
	_continue->Update(dt);
	_continue->Draw(app->render);
	settings->Update(dt);
	settings->Draw(app->render);
	musicVolume->Update(dt);
	musicVolume->Draw(app->render);
	fxVolume->Update(dt);
	fxVolume->Draw(app->render);
	fullscreen->Update(dt);
	fullscreen->Draw(app->render);
	vsync->Update(dt);
	vsync->Draw(app->render);
	exit->Update(dt);
	exit->Draw(app->render);
	credits->Update(dt);
	credits->Draw(app->render);

	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	int c_x = -app->render->camera.x;
	r.x = c_x;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control) {
	if (control->text == "settings") {
		currentScene = SETTINGS;
	}
	return true;
}