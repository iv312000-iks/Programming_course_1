#include "JackMoneyUI.h"

JackMoneyUI::JackMoneyUI(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font), cache(0)
{
	destCache = { 15, 15, 0, 0 };
	updateCache();
}

JackMoneyUI::~JackMoneyUI()
{
}

void JackMoneyUI::update()
{
	updateCache();
}

void JackMoneyUI::draw()
{
	SDL_RenderTexture(renderer, cacheTexture, NULL, &destCache);
	SDL_DestroyTexture(cacheTexture);
}

void JackMoneyUI::updateCache()
{
	cacheColor = { 255, 255, 255, 255 };
	cacheText = std::to_string(cache) + "$";
	SDL_Surface* cacheSurface = TTF_RenderText_Solid(font, cacheText.c_str(), strlen(cacheText.c_str()), cacheColor);
	cacheTexture = SDL_CreateTextureFromSurface(renderer, cacheSurface);
	SDL_GetTextureSize(cacheTexture, &destCache.w, &destCache.h);
	SDL_DestroySurface(cacheSurface);
}

void JackMoneyUI::updateTexture()
{
}
