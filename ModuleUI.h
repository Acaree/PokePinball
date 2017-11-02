#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleUI : public Module
{
private:
	SDL_Texture* lifes_icon;
	SDL_Rect pokeball_lifes;
	Font	 fonts[MAX_FONTS];
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	int Load(const char* texture_path, const char* characters, uint rows = 1);
	void UnLoad(int font_id); 
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;


	bool Start();
	bool CleanUp();
	update_status Update();

public:
	//Textures
	SDL_Texture* ui = nullptr;


	//Fonts
	int font_score;
	int score;

	bool destroyed = false;
};

#endif // __ModuleUI_H__