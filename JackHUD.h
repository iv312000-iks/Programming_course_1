#pragma once
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<string>
#include "JackMoneyUI.h"
#include "JackHealthUI.h"

class JackHUD
{
public:
    JackHUD(SDL_Renderer* renderer, TTF_Font* font, int currHealth, int totalHealth);
    ~JackHUD();
    void update();
    void draw();
    void setCache(int newCache);
    void setHealth(int curr, int total);

private:
    JackMoneyUI* jackMoneyUI;
    JackHealthUI* jackHealthUI;
    SDL_Renderer* renderer;
    TTF_Font* font;
    int currHealth;
    int totalHealth;
};
