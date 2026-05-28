#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

struct InventoryItem {
    std::string name;
    std::string texturePath;
    SDL_Texture* texture;
    int count;
};

class Inventory {
public:
    Inventory(SDL_Renderer* renderer, TTF_Font* font);
    ~Inventory();

    void addItem(const std::string& name, const std::string& texturePath, int count = 1);
    bool hasItem(const std::string& name) const;
    void draw();
    void toggle() { visible = !visible; }
    bool isVisible() const { return visible; }

    // Сериализация для сохранения
    std::string serialize() const;
    void deserialize(const std::string& data, SDL_Renderer* renderer);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<InventoryItem> items;
    bool visible;
};
