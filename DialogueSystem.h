#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

struct DialogueLine {
    std::string speaker; // "Джек" или "Барон" и т.д.
    std::string text;
};

class DialogueSystem {
public:
    DialogueSystem(SDL_Renderer* renderer, TTF_Font* font);
    ~DialogueSystem();

    void startDialogue(const std::vector<DialogueLine>& lines);
    void advance();       // переход к следующей реплике
    bool isActive() const { return active; }
    void draw();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    std::vector<DialogueLine> dialogue;
    int currentLine;
    bool active;

    // Вспомогательный рендер текста с переносом строк
    void renderWrappedText(const std::string& text, float x, float y, float maxWidth, SDL_Color color);
};
