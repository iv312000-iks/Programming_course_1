#include "Jack.h"
#include "Animation.h"
#include <SDL3_image/SDL_image.h>

Jack::Jack(SDL_Renderer* renderer, std::string texturePath, TTF_Font* font)
    : renderer(renderer), font(font), cache(0), inDialogue(false)
{
    flip = SDL_FLIP_NONE;
    isAttacked = false;
    isUpgreaded = false;
    isAttack = false;
    isWalk = false;
    initAnimations();
    sizeSprite = 64;
    idleY = 64;
    idleBloodX = 192;
    texture = IMG_LoadTexture(renderer, texturePath.c_str());
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    dest = { 0, 250, sizeSprite * 6, sizeSprite * 6 };
    src = { 0, 64, sizeSprite, sizeSprite };
    speed = 5;
    currHealth = 100;
    totalHealth = 100;
    jackHUD = new JackHUD(renderer, font, currHealth, totalHealth);
}

Jack::~Jack() {
    SDL_DestroyTexture(texture);
    delete jackHUD;
}

void Jack::draw() {
    SDL_RenderTextureRotated(renderer, texture, &src, &dest, 0, NULL, flip);
    jackHUD->draw();
}

void Jack::addCache(int addedCache) {
    cache += addedCache;
    jackHUD->setCache(cache);
}

void Jack::setHealth(int hp) {
    currHealth = hp < 0 ? 0 : (hp > totalHealth ? totalHealth : hp);
    jackHUD->setHealth(currHealth, totalHealth);
}

void Jack::takeDamage(int dmg) {
    setHealth(currHealth - dmg);
}

void Jack::update(int curr_scene) {
    const bool* keys = SDL_GetKeyboardState(NULL);

    // В диалоге движение заблокировано
    if (!inDialogue) {
        defineLook(keys);
        if (isAttack) {
            attackHandler();
        }
        else {
            moveHandler(keys);
        }
    }
    else {
        // Стоим на месте в диалоге
        src.x = isUpgreaded ? idleBloodX : 0;
        src.y = idleY;
        walkAnim.reset();
    }

    jackHUD->update();

    if (curr_scene == 0 && dest.x < 0) {
        dest.x = 0;
    }
}

void Jack::handleEvents(SDL_Event* event) {
    if (inDialogue) return; // В диалоге не атакуем
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_F && !isAttack) {
            isAttack = true;
            const animationProperties& curr_attack = isUpgreaded ? animations.attackBlood : animations.attack;
            attackAnim.start(curr_attack, src, sizeSprite);
            if (!isAttacked) isAttacked = true;
        }
        break;
    }
}

void Jack::initAnimations() {
    animations.walk = { 6, 115, 0 };
    animations.walkBlood = { 6, 115, 256 };
    animations.attack = { 6, 140, 384 };
    animations.attackBlood = { 6, 140, 448 };
}

void Jack::defineLook(const bool* keys) {
    if (keys[SDL_SCANCODE_A]) flip = SDL_FLIP_HORIZONTAL;
    if (keys[SDL_SCANCODE_D]) flip = SDL_FLIP_NONE;
}

void Jack::attackHandler() {
    const animationProperties& curr_attack = isUpgreaded ? animations.attackBlood : animations.attack;
    bool finished = attackAnim.update(curr_attack, src, (int)sizeSprite);
    if (finished) {
        isAttack = false;
        if (isAttacked && !isUpgreaded) isUpgreaded = true;
        src.x = isUpgreaded ? idleBloodX : 0;
        src.y = idleY;
    }
}

void Jack::moveHandler(const bool* keys) {
    if (keys[SDL_SCANCODE_A]) {
        dest.x -= speed;
        const animationProperties& curr_walk = isUpgreaded ? animations.walkBlood : animations.walk;
        walkAnim.showAnimation(curr_walk, src, (int)sizeSprite);
    }
    else if (keys[SDL_SCANCODE_D]) {
        dest.x += speed;
        const animationProperties& curr_walk = isUpgreaded ? animations.walkBlood : animations.walk;
        walkAnim.showAnimation(curr_walk, src, (int)sizeSprite);
    }
    else {
        src.x = isUpgreaded ? idleBloodX : 0;
        src.y = idleY;
        walkAnim.reset();
    }
}

void Jack::setPosition(float x, float y) {
    dest.x = x;
    dest.y = y;
}
