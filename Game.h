#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Jack.h"
#include "Resource.h"
#include "JackHealthUI.h"
#include "DialogueSystem.h"
#include "Inventory.h"
#include "SaveSystem.h"

class Game
{
public:
    Game();
    ~Game();
    SDL_AppResult SDL_AppInit();
    SDL_AppResult SDL_AppEvent(SDL_Event* event);
    SDL_AppResult SDL_AppIterate();
    void SDL_AppQuite(SDL_AppResult result);

    struct NPC {
        SDL_FRect rect;
        SDL_Texture* texture;
        std::vector<DialogueLine> dialogue;
        bool talked;
        SDL_FlipMode flip;
        bool visible;
        // Для бомжей — враги
        bool isEnemy;
        int health;
        int maxHealth;
        bool defeated;
    };

    struct PickupItem {
        SDL_FRect rect;
        SDL_Texture* texture;
        std::string name;
        std::string texturePath;
        int moneyValue;
        bool picked;
        int scene;
    };

private:
    // Инициализация сцен
    void initNPCs();
    void initPickups();

    // Отрисовка
    void renderScene();
    void renderNPCs();
    void renderPickups();
    void renderBattleUI();
    void renderSaveNotification();

    // Логика
    void handleSceneTransition();
    void checkPickups();
    void checkNPCInteraction();
    void updateBomzhFight();
    void autoSave();

    // Данные
    std::vector<NPC> npcs[13];
    std::vector<PickupItem> pickups;

    bool isFlashing;
    float flashAlpha;
    bool hasAnimationEnded;
    IMG_Animation* bgAnimation;
    SDL_Texture** bgFrames;
    int curr_frame;
    Uint64 lastFrameTime;
    int curr_scene;
    SDL_Texture* backgrounds[13];
    SDL_Window* window;
    SDL_Renderer* renderer;
    Jack* player;
    TTF_Font* font;

    DialogueSystem* dialogue;
    Inventory* inventory;

    // Флаги состояния мира
    bool talkedBaron_kitchen;
    bool talkedBaron_bedroom;
    bool talkedBaron_end;
    bool pickedMoney;
    bool pickedMayonnaise;
    bool foughtBomzhi;

    // Бой с бомжами
    bool inBomzhFight;
    int currentBomzhIndex;
    Uint64 lastDamageTime;
    Uint64 lastPlayerDamageTime;
    bool showBattleHint;

    // Уведомление о сохранении
    bool showSaveNotif;
    Uint64 saveNotifTime;
};
