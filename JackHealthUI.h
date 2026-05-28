#pragma once
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<string>

class JackHealthUI
{
public:
    JackHealthUI(SDL_Renderer* renderer, TTF_Font* font, int currHealth, int totalHealth);
    ~JackHealthUI();
    void update();
    void draw();
    void setHealth(int curr, int total);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    int totalHealth;
    int currHealth;
    SDL_FRect dest_curr_health;
    SDL_FRect dest_total_health;
    SDL_FRect dest_border_health;
    SDL_Texture* texture;
};
