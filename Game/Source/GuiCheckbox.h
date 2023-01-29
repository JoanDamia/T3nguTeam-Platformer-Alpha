
#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckbox : public GuiControl
{
public:

	GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text, const char* texturePath);
	virtual ~GuiCheckbox();

	bool Start();
	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	const char* texturePath;

	uint audioFxId;
	bool check = false;
private:
	SDL_Texture* texture;
	
};

#endif // __GUIBUTTON_H__