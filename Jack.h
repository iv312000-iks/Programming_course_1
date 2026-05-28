#pragma once
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<string>
#include "Animation.h"
#include "JackHUD.h"
#include "structs.h"

typedef struct {
    animationProperties walk;
    animationProperties walkBlood;
    animationProperties attack;
    animationProperties attackBlood;
    animationProperties curr_attack;
    animationProperties curr_walk;
} animationData;

class Jack
{
public:
    Jack(SDL_Renderer* renderer, std::string texturePath, TTF_Font* font);
    ~Jack();
    void handleEvents(SDL_Event* event);
    void update(int curr_scene);
    void draw();
    SDL_FRect getDest() { return dest; };
    void addCache(int addedCache);
    void setPosition(float x, float y);

    // Новые методы
    int getHealth() const { return currHealth; }
    void setHealth(int hp);
    void takeDamage(int dmg);
    int getMoney() const { return cache; }
    void setMoney(int m) { cache = m; jackHUD->setCache(cache); }
    bool isInDialogue() const { return inDialogue; }
    void setDialogue(bool d) { inDialogue = d; }

private:
    void defineLook(const bool* keys);
    void attackHandler();
    void moveHandler(const bool* keys);

    int cache;
    void initAnimations();

    SDL_Renderer* renderer;
    SDL_FRect src;
    SDL_FRect dest;
    SDL_Texture* texture;
    Animation animationHandler;
    Animation attackAnim;
    Animation walkAnim;
    animationData animations;

    int speed;
    float sizeSprite;

    bool isWalk;
    bool isAttack;
    bool isUpgreaded;
    bool isAttacked;
    bool inDialogue;

    int idleY;
    int idleBloodX;
    SDL_FlipMode flip;

    TTF_Font* font;
    JackHUD* jackHUD;

    int currHealth;
    int totalHealth;
};
