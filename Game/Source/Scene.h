#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Background.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckbox.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);
	

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	Background* background;
	Background* menu;
	Background* _credits;
	Background* death;
	Background* victory;

	enum Scenes {
		LOGO = 0,
		MAINMENUENTRY,
		MAINMENU,
		SETTINGS,
		CREDITS,
		PAUSEMENU,
		DEADSCREEN,
		LVL1,
		LVL2,
		VICTORY
	};
		 
	//SDL_Texture* title_screen;
	//int current_lvl = 0;
	//float x = 50, y = 500;
	Scenes currentScene = (Scenes)0;

private:
	SDL_Texture* img;
	SDL_Rect r;
	Uint8 a;
	pugi::xml_node& config = pugi::xml_node();

	SDL_Texture* flag;

	GuiButton* newGame;
	GuiButton* _continue;
	GuiButton* resume;
	GuiButton* backToTitle;
	GuiButton* inGameSettings;
	GuiButton* settings;
	GuiSlider* musicVolume;
	GuiSlider* fxVolume;
	GuiCheckbox* fullscreen;
	GuiButton* back;
	GuiCheckbox* vsync;
	GuiButton* exit;
	GuiButton* credits;

	PerfTimer timer;

	bool showMap = true;

	//bool go_black = false;
	//bool return_black = false;
	//int fade_speed = 2;

	//int destination_level = -1;
	//pugi::xml_node node;

};

#endif // __SCENE_H__