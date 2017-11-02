#include "Globals.h"
#include "Module.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"

#include "SDL\include\SDL.h"

#include<stdio.h>

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start() {
	bool ret = true;
	font_score=Load("pinball/fonts.png", "0123456789", 1);
	score = 0;
	return ret;
}

bool ModuleUI::CleanUp() {
	bool ret = true;
	UnLoad(font_score);
	return ret;
}

update_status ModuleUI::Update() {

	char score_array[10];
	char lifes_array[2];
	sprintf_s(score_array, "%i", score);
	BlitText(30, 0, font_score, score_array);
	sprintf_s(lifes_array, "%i", App->player->lifes);
	BlitText(SCREEN_WIDTH - 30, 0, font_score, lifes_array);

	return UPDATE_CONTINUE;

}

int ModuleUI::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].graphic == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	fonts[id].graphic = tex; // graphic: pointer to the texture
	fonts[id].rows = rows; // rows: rows of characters in the texture

	fonts[id].len = strlen(characters);

	fonts[id].row_chars = fonts[id].len / fonts[id].rows;

	for (int i = 0; i < strlen(characters); i++)
	{
		fonts[id].table[i] = characters[i];
	}

	uint wTex = 0, hTex = 0;
	App->textures->GetSize(tex, wTex, hTex);

	if (fonts[id].rows > 1) {
		fonts[id].char_h = hTex / rows;
		fonts[id].char_w = wTex / fonts[id].len;
	}
	else {
		fonts[id].char_w = wTex / fonts[id].len;
		fonts[id].char_h = hTex;
	}


	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleUI::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

// Render text using a bitmap font
void ModuleUI::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	int inTable;
	for (uint i = 0; i < len; ++i)
	{
		for (int j = 0; j < strlen(font->table); j++)
		{
			if (text[i] == fonts->table[j]) {
				inTable = j;
				break;
			}
		}
		SDL_Rect charRec;
		charRec.x = inTable * font->char_w;
		charRec.y = 0;//to change if have more rows lol
		charRec.w = font->char_w;
		charRec.h = font->char_h;
		App->renderer->Blit(font->graphic, x, y, &charRec, 0.0f);
		x += font->char_w;

	}
}
