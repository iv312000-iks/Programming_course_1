#include "JackHUD.h"

JackHUD::JackHUD(SDL_Renderer* renderer, TTF_Font* font, int currHealth, int totalHealth)
    : renderer(renderer), font(font), currHealth(currHealth), totalHealth(totalHealth)
{
    jackMoneyUI = new JackMoneyUI(renderer, font);
    jackHealthUI = new JackHealthUI(renderer, font, currHealth, totalHealth);
}

JackHUD::~JackHUD() {
    delete jackMoneyUI;
    delete jackHealthUI;
}

void JackHUD::update() {
    jackMoneyUI->update();
    jackHealthUI->update();
}

void JackHUD::draw() {
    jackMoneyUI->draw();
    jackHealthUI->draw();
}

void JackHUD::setCache(int newCache) {
    jackMoneyUI->setCache(newCache);
}

void JackHUD::setHealth(int curr, int total) {
    currHealth = curr;
    totalHealth = total;
    jackHealthUI->setHealth(curr, total);
}
