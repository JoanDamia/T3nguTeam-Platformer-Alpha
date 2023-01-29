#include "GuiCheckbox.h"
#include "render.h"
#include "app.h"
#include "audio.h"
#include "log.h"


GuiCheckbox::GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* texture) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	audioFxId = app->audio->LoadFx("assets/audio/fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiCheckbox::~GuiCheckbox()
{

}

bool GuiCheckbox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// l15: done 3: update the state of the guibutton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousstate = state;


		// i'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			if (previousstate != state) {
				LOG("change state from %d to %d", previousstate, state);
				app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				check = !check;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}




bool GuiCheckbox::Draw(Render* render)
{
	//l15: done 4: draw the button according the guicontrol state

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		render->DrawTexture(texture, bounds.x - 20, bounds.y);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	}

	//app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });
	app->render->DrawText(bounds.x, bounds.y, text.GetString());

	return false;
}