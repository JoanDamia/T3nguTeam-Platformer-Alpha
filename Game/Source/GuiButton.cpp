/*
#include "guibutton.h"
#include "render.h"
#include "app.h"
#include "audio.h"
#include "log.h"

guibutton::guibutton(uint32 id, sdl_rect bounds, const char* text) : guicontrol(guicontroltype::button, id)
{
	this->bounds = bounds;
	this->text = text;

	canclick = true;
	drawbasic = false;

	audiofxid = app->audio->loadfx("assets/audio/fx/retro-video-game-coin-pickup-38299.ogg");
}

guibutton::~guibutton()
{

}

bool guibutton::update(float dt)
{
	if (state != guicontrolstate::disabled)
	{
		// l15: done 3: update the state of the guibutton according to the mouse position
		app->input->getmouseposition(mousex, mousey);

		guicontrolstate previousstate = state;

		// i'm inside the limitis of the button
		if (mousex >= bounds.x && mousex <= bounds.x + bounds.w &&
			mousey >= bounds.y && mousey <= bounds.y + bounds.h) {

			state = guicontrolstate::focused;
			if (previousstate != state) {
				log("change state from %d to %d", previousstate, state);
				app->audio->playfx(audiofxid);
			}

			if (app->input->getmousebuttondown(sdl_button_left) == keystate::key_repeat) {
				state = guicontrolstate::pressed;
			}

			//
			if (app->input->getmousebuttondown(sdl_button_left) == keystate::key_up) {
				notifyobserver();
			}
		}
		else {
			state = guicontrolstate::normal;
		}
	}

	return false;
}




bool guibutton::draw(render* render)
{
	//l15: done 4: draw the button according the guicontrol state

	switch (state)
	{
	case guicontrolstate::disabled:
		render->drawrectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case guicontrolstate::normal:
		render->drawrectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case guicontrolstate::focused:
		render->drawrectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case guicontrolstate::pressed:
		render->drawrectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	}

	app->render->drawtext(text.getstring(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });

	return false;
}*/