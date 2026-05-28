#include "DialogueSystem.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <sstream>

DialogueSystem::DialogueSystem(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), currentLine(0), active(false) {
}

DialogueSystem::~DialogueSystem() {}

void DialogueSystem::startDialogue(const std::vector<DialogueLine>& lines) {
    dialogue = lines;
    currentLine = 0;
    active = !lines.empty();
}

void DialogueSystem::advance() {
    if (!active) return;
    currentLine++;
    if (currentLine >= (int)dialogue.size()) {
        active = false;
        currentLine = 0;
    }
}

void DialogueSystem::renderWrappedText(const std::string& text, float x, float y, float maxWidth, SDL_Color color) {
    // Разбиваем текст на слова и рендерим построчно
    std::istringstream stream(text);
    std::string word, line;
    float lineY = y;
    float lineH = 0;

    auto renderLine = [&](const std::string& l) {
        if (l.empty()) return;
        SDL_Surface* surf = TTF_RenderText_Blended(font, l.c_str(), l.size(), color);
        if (!surf) return;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        float tw = (float)surf->w, th = (float)surf->h;
        lineH = th;
        SDL_DestroySurface(surf);
        SDL_FRect dst = { x, lineY, tw, th };
        SDL_RenderTexture(renderer, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
        lineY += th + 2;
        };

    while (stream >> word) {
        std::string test = line.empty() ? word : line + " " + word;
        SDL_Surface* surf = TTF_RenderText_Blended(font, test.c_str(), test.size(), color);
        if (surf) {
            float tw = (float)surf->w;
            SDL_DestroySurface(surf);
            if (tw > maxWidth && !line.empty()) {
                renderLine(line);
                line = word;
            }
            else {
                line = test;
            }
        }
    }
    renderLine(line);
}

void DialogueSystem::draw() {
    if (!active || currentLine >= (int)dialogue.size()) return;

    const DialogueLine& dl = dialogue[currentLine];

    // Фон диалогового окна
    SDL_FRect box = { 20, 520, 960, 160 };
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 210);
    SDL_RenderFillRect(renderer, &box);

    // Рамка
    SDL_SetRenderDrawColor(renderer, 180, 140, 60, 255);
    SDL_RenderRect(renderer, &box);

    // Внутренняя рамка (декор)
    SDL_FRect innerBox = { 24, 524, 952, 152 };
    SDL_SetRenderDrawColor(renderer, 120, 90, 30, 180);
    SDL_RenderRect(renderer, &innerBox);

    // Имя говорящего
    std::string nameLabel = "[" + dl.speaker + "]";
    SDL_Color nameColor;
    if (dl.speaker == "Джек") {
        nameColor = { 100, 200, 255, 255 };
    }
    else if (dl.speaker == "Барон") {
        nameColor = { 220, 180, 50, 255 };
    }
    else if (dl.speaker.find("Бомж") != std::string::npos) {
        nameColor = { 180, 100, 60, 255 };
    }
    else {
        nameColor = { 200, 200, 200, 255 };
    }

    SDL_Surface* nameSurf = TTF_RenderText_Blended(font, nameLabel.c_str(), nameLabel.size(), nameColor);
    if (nameSurf) {
        SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
        SDL_FRect nameRect = { 35, 528, (float)nameSurf->w, (float)nameSurf->h };
        SDL_RenderTexture(renderer, nameTex, NULL, &nameRect);
        SDL_DestroyTexture(nameTex);
        SDL_DestroySurface(nameSurf);
    }

    // Текст диалога
    SDL_Color textColor = { 240, 240, 230, 255 };
    renderWrappedText(dl.text, 35, 575, 920, textColor);

    // Подсказка "E - продолжить"
    std::string hint = "[E] продолжить";
    SDL_Color hintColor = { 120, 120, 120, 255 };
    SDL_Surface* hintSurf = TTF_RenderText_Blended(font, hint.c_str(), hint.size(), hintColor);
    if (hintSurf) {
        SDL_Texture* hintTex = SDL_CreateTextureFromSurface(renderer, hintSurf);
        SDL_FRect hintRect = { 960 - hintSurf->w - 10.0f, 630, (float)hintSurf->w, (float)hintSurf->h };
        SDL_RenderTexture(renderer, hintTex, NULL, &hintRect);
        SDL_DestroyTexture(hintTex);
        SDL_DestroySurface(hintSurf);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
