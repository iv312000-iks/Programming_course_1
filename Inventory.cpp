#include "Inventory.h"
#include <SDL3_image/SDL_image.h>
#include <sstream>

Inventory::Inventory(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), visible(false) {
}

Inventory::~Inventory() {
    for (auto& item : items) {
        if (item.texture) SDL_DestroyTexture(item.texture);
    }
}

void Inventory::addItem(const std::string& name, const std::string& texturePath, int count) {
    for (auto& item : items) {
        if (item.name == name) {
            item.count += count;
            return;
        }
    }
    InventoryItem newItem;
    newItem.name = name;
    newItem.texturePath = texturePath;
    newItem.count = count;
    newItem.texture = IMG_LoadTexture(renderer, texturePath.c_str());
    if (newItem.texture) SDL_SetTextureScaleMode(newItem.texture, SDL_SCALEMODE_NEAREST);
    items.push_back(newItem);
}

bool Inventory::hasItem(const std::string& name) const {
    for (const auto& item : items)
        if (item.name == name) return true;
    return false;
}

void Inventory::draw() {
    if (!visible) return;

    // Полупрозрачный фон инвентаря
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_FRect bg = { 200, 100, 600, 500 };
    SDL_SetRenderDrawColor(renderer, 15, 15, 25, 230);
    SDL_RenderFillRect(renderer, &bg);

    // Рамка
    SDL_SetRenderDrawColor(renderer, 180, 140, 60, 255);
    SDL_RenderRect(renderer, &bg);

    // Заголовок
    std::string title = "=== ИНВЕНТАРЬ ===";
    SDL_Color titleColor = { 220, 180, 50, 255 };
    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, title.c_str(), title.size(), titleColor);
    if (titleSurf) {
        SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        float tx = 500 - titleSurf->w / 2.0f;
        SDL_FRect titleRect = { tx, 110, (float)titleSurf->w, (float)titleSurf->h };
        SDL_RenderTexture(renderer, titleTex, NULL, &titleRect);
        SDL_DestroyTexture(titleTex);
        SDL_DestroySurface(titleSurf);
    }

    if (items.empty()) {
        std::string empty = "Инвентарь пуст";
        SDL_Color emptyColor = { 150, 150, 150, 255 };
        SDL_Surface* s = TTF_RenderText_Blended(font, empty.c_str(), empty.size(), emptyColor);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FRect r = { 500 - s->w / 2.0f, 300, (float)s->w, (float)s->h };
            SDL_RenderTexture(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);
            SDL_DestroySurface(s);
        }
    }

    // Слоты предметов: 5 в ряд
    int cols = 5;
    float slotSize = 80;
    float startX = 230;
    float startY = 180;
    float padding = 40;

    for (int i = 0; i < (int)items.size(); i++) {
        int col = i % cols;
        int row = i / cols;
        float sx = startX + col * (slotSize + padding);
        float sy = startY + row * (slotSize + padding + 50);

        // Фон слота
        SDL_FRect slot = { sx, sy, slotSize, slotSize };
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderFillRect(renderer, &slot);
        SDL_SetRenderDrawColor(renderer, 100, 100, 140, 255);
        SDL_RenderRect(renderer, &slot);

        // Иконка предмета
        if (items[i].texture) {
            SDL_FRect iconRect = { sx + 5, sy + 5, slotSize - 10, slotSize - 10 };
            SDL_RenderTexture(renderer, items[i].texture, NULL, &iconRect);
        }

        // Название + количество
        std::string label = items[i].name;
        if (items[i].count > 1) label += " x" + std::to_string(items[i].count);
        SDL_Color labelColor = { 200, 200, 200, 255 };
        SDL_Surface* labelSurf = TTF_RenderText_Blended(font, label.c_str(), label.size(), labelColor);
        if (labelSurf) {
            SDL_Texture* labelTex = SDL_CreateTextureFromSurface(renderer, labelSurf);
            float lx = sx + slotSize / 2 - labelSurf->w / 2.0f;
            SDL_FRect labelRect = { lx, sy + slotSize + 20, (float)labelSurf->w, (float)labelSurf->h };
            SDL_RenderTexture(renderer, labelTex, NULL, &labelRect);
            SDL_DestroyTexture(labelTex);
            SDL_DestroySurface(labelSurf);
        }
    }

    // Подсказка закрытия
    std::string hint = "[I] закрыть";
    SDL_Color hintColor = { 120, 120, 120, 255 };
    SDL_Surface* hs = TTF_RenderText_Blended(font, hint.c_str(), hint.size(), hintColor);
    if (hs) {
        SDL_Texture* ht = SDL_CreateTextureFromSurface(renderer, hs);
        SDL_FRect hr = { 760 - hs->w - 10.0f, 540, (float)hs->w, (float)hs->h };
        SDL_RenderTexture(renderer, ht, NULL, &hr);
        SDL_DestroyTexture(ht);
        SDL_DestroySurface(hs);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

std::string Inventory::serialize() const {
    std::ostringstream oss;
    for (const auto& item : items) {
        oss << item.name << "|" << item.texturePath << "|" << item.count << "\n";
    }
    return oss.str();
}

void Inventory::deserialize(const std::string& data, SDL_Renderer* rend) {
    for (auto& item : items) {
        if (item.texture) SDL_DestroyTexture(item.texture);
    }
    items.clear();

    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');
        if (p1 == std::string::npos || p1 == p2) continue;
        InventoryItem item;
        item.name = line.substr(0, p1);
        item.texturePath = line.substr(p1 + 1, p2 - p1 - 1);
        item.count = std::stoi(line.substr(p2 + 1));
        item.texture = IMG_LoadTexture(rend, item.texturePath.c_str());
        if (item.texture) SDL_SetTextureScaleMode(item.texture, SDL_SCALEMODE_NEAREST);
        items.push_back(item);
    }
}
