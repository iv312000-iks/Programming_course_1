#pragma once
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<string>

class JackMoneyUI
{
public:
	JackMoneyUI(SDL_Renderer* renderer, TTF_Font* font);
	~JackMoneyUI();
	void update();
	void draw();
	void setCache(int newCache) { cache = newCache; };

private:
	void updateCache();
	void updateTexture();
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Texture* cacheTexture;
	int cache;
	std::string cacheText;
	SDL_Color cacheColor;
	SDL_FRect destCache;
};

