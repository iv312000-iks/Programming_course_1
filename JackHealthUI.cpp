#include "JackHealthUI.h"

JackHealthUI::JackHealthUI(SDL_Renderer* renderer, TTF_Font* font, int currHealth, int totalHealth)
    : renderer(renderer), font(font), currHealth(currHealth), totalHealth(totalHealth), texture(nullptr)
{
    dest_border_health = { 15, 98, 250, 15 };
    dest_curr_health = { 13, 96, 248, 13 };
    dest_total_health = { 13, 96, 248, 13 };
    float healthPercent = static_cast<float>(currHealth) / totalHealth;
    dest_curr_health.w = static_cast<int>(248 * healthPercent);
}

JackHealthUI::~JackHealthUI() {}

void JackHealthUI::setHealth(int curr, int total) {
    currHealth = curr < 0 ? 0 : curr;
    totalHealth = total;
    float healthPercent = totalHealth > 0 ? static_cast<float>(currHealth) / totalHealth : 0.0f;
    dest_curr_health.w = 248 * healthPercent;
}

void JackHealthUI::update() {}

void JackHealthUI::draw() {
    // Рамка
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &dest_border_health);

    // Фон (пустая шкала)
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
    SDL_RenderFillRect(renderer, &dest_total_health);

    // Текущее здоровье — цвет меняется в зависимости от уровня
    float ratio = totalHealth > 0 ? (float)currHealth / totalHealth : 0.0f;
    if (ratio > 0.5f) {
        SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);  // зелёный
    }
    else if (ratio > 0.25f) {
        SDL_SetRenderDrawColor(renderer, 220, 180, 0, 255);  // жёлтый
    }
    else {
        SDL_SetRenderDrawColor(renderer, 220, 40, 40, 255);  // красный
    }
    if (dest_curr_health.w > 0) {
        SDL_RenderFillRect(renderer, &dest_curr_health);
    }

    // Надпись HP
    if (font) {
        std::string hpText = "HP: " + std::to_string(currHealth) + "/" + std::to_string(totalHealth);
        SDL_Color hpColor = { 255, 255, 255, 255 };
        SDL_Surface* s = TTF_RenderText_Blended(font, hpText.c_str(), hpText.size(), hpColor);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FRect r = { 15, 60, (float)s->w, (float)s->h };
            SDL_RenderTexture(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);
            SDL_DestroySurface(s);
        }
    }
}
