#include "Resource.h"
#include<SDL3_image/SDL_image.h>
#include<stdio.h>

Resource::Resource(SDL_Renderer* ren, SDL_FPoint pos, const char* pathTexture, float scale) : renderer(ren), scale(scale)
{
	texture = IMG_LoadTexture(renderer, pathTexture);
	float w, h;
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	SDL_GetTextureSize(texture, &w, &h);

	dest.x = pos.x;
	dest.y = pos.y;
	dest.w = w * scale;
	dest.h = h * scale;

}

Resource::~Resource()
{
	SDL_DestroyTexture(texture);
}

void Resource::draw()
{
	SDL_RenderTexture(renderer, texture, NULL, &dest);
}

void Resource::setPosition(float x, float y) {
	dest.x = x;
	dest.y = y;
}