#pragma once
#include<SDL3/SDL.h>

class Resource
{
public:
	Resource(SDL_Renderer* ren, SDL_FPoint pos, const char* pathTexture, float scale = 1.0f);
	~Resource();
	void draw();
	int getCache() { return cache; };
	void setPosition(float x, float y);
	SDL_FRect getDest() { return dest; };
	SDL_FRect getHoverTextureSize() { return hoverTextureSize; };

private:
	int cache = 15;
	SDL_Texture* texture;
	SDL_FRect dest;
	SDL_FRect hoverTextureSize = { 0, 0, 270, 244 };
	SDL_Renderer* renderer;
	float scale;
};

