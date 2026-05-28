#include "Game.h"
#include <string>

// ========== Диалоги ==========

// Кухня: Барон
static std::vector<DialogueLine> baronKitchenDialogue() {
    return {
        {"Барон", "А, Джек... Ты снова здесь. Опять шарашишься по чужим кухням?"},
        {"Джек",  "Я просто... проходил мимо. Что тут вообще происходит?"},
        {"Барон", "Ничего, ничего. Просто готовлю себе завтрак. Майонез — это философия, Джек."},
        {"Джек",  "Ты всегда был немного странным, Барон."},
        {"Барон", "Странным? Нет. Я единственный нормальный человек в этом доме. Остальные — это проблема."},
        {"Джек",  "...Ладно. Пойду дальше."},
        {"Барон", "Не трогай мой майонез. Я серьёзно."},
    };
}

// Спальня: голос Барона (его нет, но он как будто говорит из-за двери или из головы)
static std::vector<DialogueLine> baronBedroomDialogue() {
    return {
        {"Барон", "...Джек. Ты меня слышишь?"},
        {"Джек",  "Барон? Ты где? Тебя тут нет..."},
        {"Барон", "Я везде, Джек. Я — часть этого дома. Эти стены помнят всё."},
        {"Джек",  "Это уже пугает."},
        {"Барон", "Пугает? Хорошо. Значит ты ещё живой. Иди дальше. Тебя ждут великие дела."},
        {"Джек",  "Какие ещё дела..."},
        {"Барон", "Или не великие. Я точно не помню. Просто иди."},
    };
}

// Переулок: бомж 1 (начало драки)
static std::vector<DialogueLine> bomzhStartDialogue() {
    return {
        {"Бомж #1", "Стой, гражданин! Это наша территория!"},
        {"Джек",    "Эй, я просто иду мимо..."},
        {"Бомж #2", "Мимо не ходят. Здесь платный проход."},
        {"Бомж #3", "Кошелёк или лицо, выбирай!"},
        {"Джек",    "...Ладно. Вы сами напросились. [F] — атака!"},
    };
}

// Переулок: после победы
static std::vector<DialogueLine> bomzhAfterDialogue() {
    return {
        {"Джек",    "Следующий раз выбирайте жертву поаккуратнее."},
    };
}

// Финал: the_end
static std::vector<DialogueLine> endDialogue() {
    return {
        {"Барон", "Ну вот. Ты дошёл."},
        {"Джек",  "До чего? До этой... могилы?"},
        {"Барон", "Читай, что написано."},
        {"Джек",  "«Здесь покоятся мечты автора о классной игре»... Барон, это что за..."},
        {"Барон", "Он старался, Джек. Он правда старался. Целый месяц."},
        {"Джек",  "Кто старался?"},
        {"Барон", "Тот, кто нас создал. У него было мало времени. Дедлайн через два дня."},
        {"Джек",  "...Погоди. Мы что, персонажи в студенческой РГР?"},
        {"Барон", "Именно. И знаешь что? Я горжусь этим. Не каждую игру делают с такой душой."},
        {"Джек",  "Ну... думаю, он справился."},
        {"Барон", "Да. Справился. Почти."},
        {"Джек",  "...Почти?"},
        {"Барон", "Майонез в инвентаре до сих пор ни на что не влияет. Но это уже детали."},
        {"Джек",  "Классика."},
    };
}

// ========== Game ==========

Game::Game() : window(nullptr), renderer(nullptr), player(nullptr), font(nullptr),
dialogue(nullptr), inventory(nullptr),
talkedBaron_kitchen(false), talkedBaron_bedroom(false), talkedBaron_end(false),
pickedMoney(false), pickedMayonnaise(false), foughtBomzhi(false),
inBomzhFight(false), currentBomzhIndex(0), lastDamageTime(0), lastPlayerDamageTime(0),
showBattleHint(false), showSaveNotif(false), saveNotifTime(0),
curr_scene(0), isFlashing(false), flashAlpha(0), hasAnimationEnded(false),
bgAnimation(nullptr), bgFrames(nullptr), curr_frame(0), lastFrameTime(0)
{
    for (int i = 0; i < 13; i++) backgrounds[i] = nullptr;
}

Game::~Game() {
    for (int i = 0; i < 13; i++) {
        if (backgrounds[i]) SDL_DestroyTexture(backgrounds[i]);
    }
    for (int i = 0; i < 13; i++) {
        for (auto& npc : npcs[i]) {
            if (npc.texture) SDL_DestroyTexture(npc.texture);
        }
    }
    for (auto& p : pickups) {
        if (p.texture) SDL_DestroyTexture(p.texture);
    }
    if (bgAnimation) {
        for (int i = 0; i < bgAnimation->count; i++) {
            if (bgFrames && bgFrames[i]) SDL_DestroyTexture(bgFrames[i]);
        }
        delete[] bgFrames;
        IMG_FreeAnimation(bgAnimation);
    }
    delete dialogue;
    delete inventory;
    if (player) delete player;
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_AppResult Game::SDL_AppInit() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer("SDL3 Game", 1000, 700, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, 1000, 700, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // Загрузка фонов
    const char* bgPaths[13] = {
        "assets/nightmare_1.png", "assets/nightmare_2.png", "assets/nightmare_3.png",
        "assets/nightmare_4.png", "assets/bedroom_1(2).png", "assets/bedroom_2.png",
        "assets/koridor.png",     "assets/kitchen_1.png",    "assets/dvor_2.png",
        "assets/town.png",        "assets/pereylok.png",     "assets/forest.png",
        "assets/the_end.png"
    };
    for (int i = 0; i < 13; i++) {
        backgrounds[i] = IMG_LoadTexture(renderer, bgPaths[i]);
        if (backgrounds[i]) SDL_SetTextureScaleMode(backgrounds[i], SDL_SCALEMODE_NEAREST);
    }

    // GIF анимация сцены 3
    bgAnimation = IMG_LoadAnimation("assets/merged.gif");
    if (bgAnimation) {
        bgFrames = new SDL_Texture * [bgAnimation->count];
        for (int i = 0; i < bgAnimation->count; i++) {
            bgFrames[i] = SDL_CreateTextureFromSurface(renderer, bgAnimation->frames[i]);
            if (bgFrames[i]) SDL_SetTextureScaleMode(bgFrames[i], SDL_SCALEMODE_NEAREST);
        }
    }
    curr_frame = 0;
    lastFrameTime = SDL_GetTicks();

    font = TTF_OpenFont("assets/Tiny5.ttf", 30);
    player = new Jack(renderer, "assets/jack_sprites.png", font);
    dialogue = new DialogueSystem(renderer, font);
    inventory = new Inventory(renderer, font);

    initNPCs();
    initPickups();


    // Я ТУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУТ
    // Загрузка сохранения если есть
    //if (SaveSystem::hasSave()) {
    //    GameSaveData save;
    //    if (SaveSystem::load(save)) {
    //        curr_scene = save.curr_scene;
    //        player->setPosition(save.playerX, save.playerY);
    //        player->setHealth(save.playerHealth);
    //        player->setMoney(save.playerMoney);
    //        talkedBaron_kitchen = save.talkedBaron_kitchen;
    //        talkedBaron_bedroom = save.talkedBaron_bedroom;
    //        talkedBaron_end = save.talkedBaron_end;
    //        pickedMoney = save.pickedMoney;
    //        pickedMayonnaise = save.pickedMayonnaise;
    //        foughtBomzhi = save.foughtBomzhi;

    //        // Восстанавливаем состояние предметов
    //        for (auto& p : pickups) {
    //            if (p.name == "Деньги" && pickedMoney)      p.picked = true;
    //            if (p.name == "Майонез" && pickedMayonnaise) p.picked = true;
    //        }
    //        // Восстанавливаем инвентарь
    //        if (!save.inventoryData.empty()) {
    //            inventory->deserialize(save.inventoryData, renderer);
    //        }
    //    }
    //}

    return SDL_APP_CONTINUE;
}

void Game::initNPCs() {
    // ============ Сцена 7 — Кухня ============
    {
        NPC baron;
        baron.rect = { 500, 200, 64 * 6, 64 * 6 };
        baron.texture = IMG_LoadTexture(renderer, "assets/Baron.png");
        if (baron.texture) SDL_SetTextureScaleMode(baron.texture, SDL_SCALEMODE_NEAREST);
        baron.dialogue = baronKitchenDialogue();
        baron.talked = false;
        baron.flip = SDL_FLIP_HORIZONTAL;
        baron.visible = true;
        baron.isEnemy = false;
        baron.health = 0; baron.maxHealth = 0;
        baron.defeated = false;
        npcs[7].push_back(baron);
    }

    // ============ Сцена 4 — Спальня (bedroom_1(2)) ============
    // Барона нет визуально, но диалог есть — реализуем через невидимый триггер
    {
        NPC baronVoice;
        baronVoice.rect = { 400, 200, 1, 1 }; // невидимый триггер
        baronVoice.texture = nullptr;
        baronVoice.dialogue = baronBedroomDialogue();
        baronVoice.talked = false;
        baronVoice.flip = SDL_FLIP_NONE;
        baronVoice.visible = false;
        baronVoice.isEnemy = false;
        baronVoice.health = 0; baronVoice.maxHealth = 0;
        baronVoice.defeated = false;
        npcs[4].push_back(baronVoice);
    }

    // ============ Сцена 10 — Переулок (bомжи) ============
    auto loadBomzh = [&](const char* path, float x, int idx) {
        NPC bomzh;
        bomzh.rect = { x, 250, 64 * 6, 64 * 6 };
        bomzh.texture = IMG_LoadTexture(renderer, path);
        if (bomzh.texture) SDL_SetTextureScaleMode(bomzh.texture, SDL_SCALEMODE_NEAREST);
        bomzh.dialogue = {}; // диалог запускается через систему боя
        bomzh.talked = false;
        bomzh.flip = (x > 500) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        bomzh.visible = true;
        bomzh.isEnemy = true;
        bomzh.health = 50;
        bomzh.maxHealth = 50;
        bomzh.defeated = false;
        npcs[10].push_back(bomzh);
        };
    loadBomzh("assets/bomj_1.png", 150, 0);
    loadBomzh("assets/bomj_2.png", 420, 1);
    loadBomzh("assets/bomj_3.png", 680, 2);

    // ============ Сцена 12 — Финал (the_end) ============
    {
        NPC baronEnd;
        baronEnd.rect = { 700, 200, 64 * 6, 64 * 6 };
        baronEnd.texture = IMG_LoadTexture(renderer, "assets/Baron.png");
        if (baronEnd.texture) SDL_SetTextureScaleMode(baronEnd.texture, SDL_SCALEMODE_NEAREST);
        baronEnd.dialogue = endDialogue();
        baronEnd.talked = false;
        baronEnd.flip = SDL_FLIP_HORIZONTAL;
        baronEnd.visible = true;
        baronEnd.isEnemy = false;
        baronEnd.health = 0; baronEnd.maxHealth = 0;
        baronEnd.defeated = false;
        npcs[12].push_back(baronEnd);
    }

    // ============ Сцена 9 — Город (town): добавим Дэвида ============
    {
        NPC david;
        david.rect = { 300, 220, 64 * 6, 64 * 6 };
        david.texture = IMG_LoadTexture(renderer, "assets/David.png");
        if (david.texture) SDL_SetTextureScaleMode(david.texture, SDL_SCALEMODE_NEAREST);
        david.dialogue = {
            {"Дэвид", "О! Джек! Давно не виделись, брат."},
            {"Джек",  "Дэвид? Ты что тут делаешь?"},
            {"Дэвид", "Да вот... стою. Думаю. Смотрю на людей."},
            {"Джек",  "И что, помогает?"},
            {"Дэвид", "Не особо. Но по крайней мере время идёт."},
            {"Джек",  "...Глубокая мысль, Дэвид."},
            {"Дэвид", "Я знаю. Я такой."},
        };
        david.talked = false;
        david.flip = SDL_FLIP_NONE;
        david.visible = true;
        david.isEnemy = false;
        david.health = 0; david.maxHealth = 0;
        david.defeated = false;
        npcs[9].push_back(david);
    }

    // ============ Сцена 11 — Лес (forest): Флойд ============
    {
        NPC floyd;
        floyd.rect = { 200, 220, 64 * 6, 64 * 6 };
        floyd.texture = IMG_LoadTexture(renderer, "assets/Floyd.png");
        if (floyd.texture) SDL_SetTextureScaleMode(floyd.texture, SDL_SCALEMODE_NEAREST);
        floyd.dialogue = {
            {"Флойд", "Стой... Ты слышишь это?"},
            {"Джек",  "Что? Я ничего не слышу."},
            {"Флойд", "Именно. Тишина. В лесу — тишина. Понимаешь?"},
            {"Джек",  "Ну... да?"},
            {"Флойд", "Нет, не понимаешь. Тишина — это не отсутствие звука. Это присутствие покоя."},
            {"Джек",  "Флойд, ты в порядке?"},
            {"Флойд", "Абсолютно. Иди своей дорогой, Джек. Лес тебя отпускает."},
        };
        floyd.talked = false;
        floyd.flip = SDL_FLIP_NONE;
        floyd.visible = true;
        floyd.isEnemy = false;
        floyd.health = 0; floyd.maxHealth = 0;
        floyd.defeated = false;
        npcs[11].push_back(floyd);
    }

    // ============ Сцена 6 — Коридор: Ласли ============
    {
        NPC lasly;
        lasly.rect = { 500, 240, 64 * 6, 64 * 6 };
        lasly.texture = IMG_LoadTexture(renderer, "assets/Laсly.png");
        if (lasly.texture) SDL_SetTextureScaleMode(lasly.texture, SDL_SCALEMODE_NEAREST);
        lasly.dialogue = {
            {"Лэсли", "Куда идёшь?"},
            {"Джек",  "Вперёд. Куда-то вперёд."},
            {"Лэсли", "Типичный ответ. Все идут «вперёд», а потом удивляются где оказались."},
            {"Джек",  "А ты не идёшь никуда?"},
            {"Лэсли", "Я уже пришла. Это и есть моё место."},
            {"Джек",  "Коридор?"},
            {"Лэсли", "У каждого свой коридор, Джек."},
        };
        lasly.talked = false;
        lasly.flip = SDL_FLIP_HORIZONTAL;
        lasly.visible = true;
        lasly.isEnemy = false;
        lasly.health = 0; lasly.maxHealth = 0;
        lasly.defeated = false;
        npcs[6].push_back(lasly);
    }
}

void Game::initPickups() {
    // Деньги на кухне (сцена 7)
    {
        PickupItem money;
        money.texture = IMG_LoadTexture(renderer, "assets/money.png");
        if (money.texture) SDL_SetTextureScaleMode(money.texture, SDL_SCALEMODE_NEAREST);
        float w = 48, h = 48;
        if (money.texture) {
            SDL_GetTextureSize(money.texture, &w, &h);
            w *= 6; h *= 6;
        }
        money.rect = { 20, 240, w, h };
        money.name = "Деньги";
        money.texturePath = "assets/money.png";
        money.moneyValue = 50;
        money.picked = false;
        money.scene = 7;
        pickups.push_back(money);
    }

    // Майонез на кухне (сцена 7)
    {
        PickupItem mayo;
        mayo.texture = IMG_LoadTexture(renderer, "assets/mayonnaise.png");
        if (mayo.texture) SDL_SetTextureScaleMode(mayo.texture, SDL_SCALEMODE_NEAREST);
        float w = 48, h = 48;
        if (mayo.texture) {
            SDL_GetTextureSize(mayo.texture, &w, &h);
            w *= 6; h *= 6;
        }
        mayo.rect = { 350, 170, w, h };
        mayo.name = "Майонез";
        mayo.texturePath = "assets/mayonnaise.png";
        mayo.moneyValue = 0;
        mayo.picked = false;
        mayo.scene = 7;
        pickups.push_back(mayo);
    }
}

SDL_AppResult Game::SDL_AppEvent(SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        autoSave();
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            autoSave();
            return SDL_APP_SUCCESS;
        }
        // E — диалог / подбор предмета
        if (event->key.scancode == SDL_SCANCODE_E) {
            if (dialogue->isActive()) {
                dialogue->advance();
                // Если диалог закончился
                if (!dialogue->isActive()) {
                    player->setDialogue(false);
                }
            }
            else {
                checkNPCInteraction();
                checkPickups();
            }
        }
        // I — инвентарь
        if (event->key.scancode == SDL_SCANCODE_I) {
            inventory->toggle();
        }
        // F5 — быстрое сохранение
        if (event->key.scancode == SDL_SCANCODE_F5) {
            autoSave();
        }
        break;
    }
    player->handleEvents(event);
    return SDL_APP_CONTINUE;
}

void Game::checkNPCInteraction() {
    SDL_FRect playerRect = player->getDest();
    float interactRange = 200.0f;

    for (auto& npc : npcs[curr_scene]) {
        if (!npc.visible && npc.texture != nullptr) continue; // только невидимые триггеры пропускаем, если есть текстура
        if (npc.isEnemy && !inBomzhFight) continue;
        if (npc.dialogue.empty()) continue;

        // Проверяем расстояние
        float npcCenterX = npc.rect.x + npc.rect.w / 2;
        float playerCenterX = playerRect.x + playerRect.w / 2;
        float dist = SDL_fabsf(npcCenterX - playerCenterX);

        if (dist < interactRange) {
            if (!npc.talked || &npc == &npcs[curr_scene].back()) {
                dialogue->startDialogue(npc.dialogue);
                player->setDialogue(true);

                // Отмечаем флаги
                if (curr_scene == 7 && !talkedBaron_kitchen)  talkedBaron_kitchen = true;
                if (curr_scene == 4 && !talkedBaron_bedroom)  talkedBaron_bedroom = true;
                if (curr_scene == 12 && !talkedBaron_end)     talkedBaron_end = true;

                npc.talked = true;
                break;
            }
        }
    }

    // Спальня — невидимый триггер срабатывает автоматически при входе
    if (curr_scene == 4 && !npcs[4].empty() && !npcs[4][0].talked) {
        // Триггер по центру экрана
        float playerCenterX = playerRect.x + playerRect.w / 2;
        if (playerCenterX > 300 && playerCenterX < 700) {
            dialogue->startDialogue(npcs[4][0].dialogue);
            player->setDialogue(true);
            npcs[4][0].talked = true;
            talkedBaron_bedroom = true;
        }
    }
}

void Game::checkPickups() {
    SDL_FRect playerRect = player->getDest();
    for (auto& item : pickups) {
        if (item.picked || item.scene != curr_scene) continue;
        SDL_FRect result;
        if (SDL_GetRectIntersectionFloat(&playerRect, &item.rect, &result)) {
            item.picked = true;
            inventory->addItem(item.name, item.texturePath);
            if (item.moneyValue > 0) {
                player->addCache(item.moneyValue);
            }
            if (item.name == "Деньги")    pickedMoney = true;
            if (item.name == "Майонез")   pickedMayonnaise = true;

            // Уведомление
            showSaveNotif = true;
            saveNotifTime = SDL_GetTicks();
        }
    }
}

void Game::updateBomzhFight() {
    if (curr_scene != 10) return;

    // Если бомжи ещё не побеждены и не начат бой — проверяем триггер
    if (!foughtBomzhi && !inBomzhFight) {
        SDL_FRect playerRect = player->getDest();
        float playerCenterX = playerRect.x + playerRect.w / 2;
        if (playerCenterX > 200 && playerCenterX < 800) {
            // Начать бой — сначала диалог
            if (!npcs[10].empty() && !npcs[10][0].talked) {
                dialogue->startDialogue(bomzhStartDialogue());
                player->setDialogue(true);
                npcs[10][0].talked = true;
                inBomzhFight = true;
                currentBomzhIndex = 0;
                showBattleHint = true;
            }
        }
        return;
    }

    if (!inBomzhFight) return;
    if (dialogue->isActive()) return; // Ждём конца диалога

    // Найти текущего живого бомжа
    int aliveCount = 0;
    for (auto& n : npcs[10]) {
        if (n.isEnemy && !n.defeated) aliveCount++;
    }

    if (aliveCount == 0) {
        // Все побеждены
        inBomzhFight = false;
        foughtBomzhi = true;
        showBattleHint = false;
        // Запускаем диалог после победы
        if (!dialogue->isActive()) {
            dialogue->startDialogue(bomzhAfterDialogue());
            player->setDialogue(true);
        }
        // Восстанавливаем немного здоровья
        int hp = player->getHealth();
        if (hp < 50) player->setHealth(hp + 20);
        return;
    }

    // Урон от игрока (нажатие F)
    const bool* keys = SDL_GetKeyboardState(NULL);
    Uint64 now = SDL_GetTicks();

    // Находим ближайшего живого бомжа
    SDL_FRect playerRect = player->getDest();
    NPC* target = nullptr;
    float minDist = 300.0f;
    for (auto& n : npcs[10]) {
        if (!n.isEnemy || n.defeated) continue;
        float cx = n.rect.x + n.rect.w / 2;
        float px = playerRect.x + playerRect.w / 2;
        float d = SDL_fabsf(cx - px);
        if (d < minDist) {
            minDist = d;
            target = &n;
        }
    }

    if (target && minDist < 250.0f) {
        // Урон от игрока (F)
        if (keys[SDL_SCANCODE_F] && (now - lastDamageTime > 800)) {
            target->health -= 25;
            lastDamageTime = now;
            if (target->health <= 0) {
                target->health = 0;
                target->defeated = true;
            }
        }

        // Урон от бомжа игроку (автоматически каждые 1.5 секунды)
        if (now - lastPlayerDamageTime > 1500) {
            player->takeDamage(10);
            lastPlayerDamageTime = now;
        }
    }
}

void Game::renderBattleUI() {
    if (!inBomzhFight || foughtBomzhi) return;
    if (dialogue->isActive()) return;

    // Полоски здоровья бомжей
    int idx = 0;
    for (auto& n : npcs[10]) {
        if (!n.isEnemy) continue;
        float barW = 120;
        float barH = 10;
        float barX = n.rect.x + n.rect.w / 2 - barW / 2;
        float barY = n.rect.y - 20;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_FRect bg = { barX - 1, barY - 1, barW + 2, barH + 2 };
        SDL_RenderFillRect(renderer, &bg);

        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_FRect full = { barX, barY, barW, barH };
        SDL_RenderFillRect(renderer, &full);

        if (!n.defeated) {
            float ratio = n.maxHealth > 0 ? (float)n.health / n.maxHealth : 0;
            SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
            SDL_FRect curr = { barX, barY, barW * ratio, barH };
            if (curr.w > 0) SDL_RenderFillRect(renderer, &curr);
        }
        idx++;
    }

    // Подсказка [F] атака
    if (showBattleHint && font) {
        std::string hint = "[F] - АТАКА  |  [A/D] - ДВИЖЕНИЕ";
        SDL_Color hc = { 255, 255, 100, 255 };
        SDL_Surface* s = TTF_RenderText_Blended(font, hint.c_str(), hint.size(), hc);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FRect r = { 500 - s->w / 2.0f, 660, (float)s->w, (float)s->h };
            SDL_RenderTexture(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);
            SDL_DestroySurface(s);
        }
    }
}

void Game::renderSaveNotification() {
    //if (!showSaveNotif) return;
    //Uint64 now = SDL_GetTicks();
    //if (now - saveNotifTime > 2000) {
    //    showSaveNotif = false;
    //    return;
    //}
    //// Полупрозрачная плашка
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    //SDL_FRect box = { 750, 20, 240, 50 };
    //SDL_SetRenderDrawColor(renderer, 20, 20, 20, 180);
    //SDL_RenderFillRect(renderer, &box);
    //SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    //SDL_RenderRect(renderer, &box);

    //std::string msg = "Предмет подобран!";
    //SDL_Color mc = { 100, 230, 100, 255 };
    //SDL_Surface* s = TTF_RenderText_Blended(font, msg.c_str(), msg.size(), mc);
    //if (s) {
    //    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
    //    SDL_FRect r = { 760, 28, (float)s->w, (float)s->h };
    //    SDL_RenderTexture(renderer, t, NULL, &r);
    //    SDL_DestroyTexture(t);
    //    SDL_DestroySurface(s);
    //}
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Game::renderPickups() {
    for (auto& item : pickups) {
        if (item.picked || item.scene != curr_scene) continue;
        if (item.texture) SDL_RenderTexture(renderer, item.texture, NULL, &item.rect);

        // Подсказка "[E] взять"
        SDL_FRect playerRect = player->getDest();
        float playerCenterX = playerRect.x + playerRect.w / 2;
        float itemCenterX = item.rect.x + item.rect.w / 2;
        if (SDL_fabsf(playerCenterX - itemCenterX) < 150 && font) {
            std::string hint = "[E] взять " + item.name;
            SDL_Color hc = { 255, 255, 200, 255 };
            SDL_Surface* s = TTF_RenderText_Blended(font, hint.c_str(), hint.size(), hc);
            if (s) {
                SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
                SDL_FRect r = { item.rect.x + item.rect.w / 2 - s->w / 2.0f, item.rect.y - s->h + 150, (float)s->w, (float)s->h };
                SDL_RenderTexture(renderer, t, NULL, &r);
                SDL_DestroyTexture(t);
                SDL_DestroySurface(s);
            }
        }
    }
}

void Game::renderNPCs() {
    for (auto& npc : npcs[curr_scene]) {
        if (!npc.visible || !npc.texture) continue;
        if (npc.isEnemy && npc.defeated) continue;
        SDL_RenderTextureRotated(renderer, npc.texture, NULL, &npc.rect, 0.0, NULL, npc.flip);

        // Подсказка [E] поговорить
        if (!npc.isEnemy && font) {
            SDL_FRect playerRect = player->getDest();
            float px = playerRect.x + playerRect.w / 2;
            float nx = npc.rect.x + npc.rect.w / 2;
            if (SDL_fabsf(px - nx) < 200) {
                std::string hint = "[E] поговорить";
                SDL_Color hc = { 255, 255, 200, 255 };
                SDL_Surface* s = TTF_RenderText_Blended(font, hint.c_str(), hint.size(), hc);
                if (s) {
                    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
                    SDL_FRect r = { npc.rect.x + npc.rect.w / 2 - s->w / 2.0f, npc.rect.y - s->h - 5, (float)s->w, (float)s->h };
                    SDL_RenderTexture(renderer, t, NULL, &r);
                    SDL_DestroyTexture(t);
                    SDL_DestroySurface(s);
                }
            }
        }
    }
}

void Game::renderScene() {
    // Сцена 3 — GIF
    if (curr_scene == 3) {
        if (!hasAnimationEnded && bgAnimation) {
            Uint64 now = SDL_GetTicks();
            if (now - lastFrameTime >= (Uint64)bgAnimation->delays[curr_frame]) {
                curr_frame++;
                if (curr_frame >= bgAnimation->count) {
                    hasAnimationEnded = true;
                    isFlashing = true;
                    curr_frame = bgAnimation->count - 1;
                }
                lastFrameTime = now;
            }
            if (bgFrames && bgFrames[curr_frame])
                SDL_RenderTexture(renderer, bgFrames[curr_frame], NULL, NULL);
        }
        else {
            if (bgFrames && bgFrames[bgAnimation->count - 1])
                SDL_RenderTexture(renderer, bgFrames[bgAnimation->count - 1], NULL, NULL);
        }
        return;
    }

    // Финальная сцена — надпись на могиле
    if (curr_scene == 12 && backgrounds[12]) {
        SDL_RenderTexture(renderer, backgrounds[12], NULL, NULL);
        // Надпись на надгробии
        if (font) {
            std::string grave1 = "здесь покоятся";
            std::string grave2 = "мечты автора";
            std::string grave3 = "о классной игре";
            SDL_Color gc = { 200, 200, 200, 255 };
            auto drawGraveLine = [&](const std::string& t, float x, float y) {
                SDL_Surface* s = TTF_RenderText_Blended(font, t.c_str(), t.size(), gc);
                if (!s) return;
                SDL_Texture* tx = SDL_CreateTextureFromSurface(renderer, s);
                SDL_FRect r = { x - s->w / 2.0f, y, (float)s->w, (float)s->h };
                SDL_RenderTexture(renderer, tx, NULL, &r);
                SDL_DestroyTexture(tx);
                SDL_DestroySurface(s);
                };
            // Позиция надгробия (подбирается визуально, примерно по центру)
            drawGraveLine(grave1, 500, 320);
            drawGraveLine(grave2, 500, 360);
            drawGraveLine(grave3, 500, 400);
        }
        return;
    }

    if (backgrounds[curr_scene])
        SDL_RenderTexture(renderer, backgrounds[curr_scene], NULL, NULL);
}

void Game::handleSceneTransition() {
    if (dialogue->isActive() || inventory->isVisible()) return;

    SDL_FRect dest = player->getDest();

    // Сцена 3: переход через вспышку
    if (curr_scene == 3) {
        if (isFlashing) {
            flashAlpha += 8.0f;
            if (flashAlpha >= 255.0f) {
                flashAlpha = 255.0f;
                player->setPosition(1, player->getDest().y);
                curr_scene = 4;
                isFlashing = false;
                hasAnimationEnded = false;
                flashAlpha = 0.0f;
                curr_frame = 0;
                lastFrameTime = SDL_GetTicks();
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, (Uint8)flashAlpha);
            SDL_FRect fr = { 0, 0, 1000, 700 };
            SDL_RenderFillRect(renderer, &fr);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
        return;
    }

    // Переулок: блокировка перехода вправо пока не победил бомжей
    if (curr_scene == 10 && !foughtBomzhi) {
        if (dest.x >= 1000 - dest.w) {
            player->setPosition(1000 - dest.w - 5, dest.y);
        }
        return;
    }

    // Общая логика переходов
    if (curr_scene != 12 && dest.x >= 1000 - dest.w) {
        curr_scene++;
        player->setPosition(1, dest.y);
        autoSave();
    }
    if (curr_scene != 0 && curr_scene != 4 && dest.x <= 0) {
        curr_scene--;
        player->setPosition(1000 - dest.w - 1, dest.y);
    }
    if (curr_scene == 0 && dest.x < 0) {
        player->setPosition(0, dest.y);
    }
}

void Game::autoSave() {
    GameSaveData save;
    save.curr_scene = curr_scene;
    save.playerX = player->getDest().x;
    save.playerY = player->getDest().y;
    save.playerHealth = player->getHealth();
    save.playerMoney = player->getMoney();
    save.inventoryData = inventory->serialize();
    save.talkedBaron_kitchen = talkedBaron_kitchen;
    save.talkedBaron_bedroom = talkedBaron_bedroom;
    save.talkedBaron_end = talkedBaron_end;
    save.pickedMoney = pickedMoney;
    save.pickedMayonnaise = pickedMayonnaise;
    save.foughtBomzhi = foughtBomzhi;
    SaveSystem::save(save);

    showSaveNotif = true;
    saveNotifTime = SDL_GetTicks();
}

SDL_AppResult Game::SDL_AppIterate() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // Отрисовка фона
    renderScene();

    // NPC
    renderNPCs();

    // Предметы
    renderPickups();

    // Игрок
    if (curr_scene != 10 || foughtBomzhi || !inBomzhFight || !dialogue->isActive()) {
        player->update(curr_scene);

        // Проверка смерти
        if (player->getHealth() <= 0) {
            curr_scene = 9;
            player->setPosition(1, 250);
            player->setHealth(100);
            foughtBomzhi = false;
            inBomzhFight = false;
            for (auto& npc : npcs[10]) {
                if (npc.isEnemy) {
                    npc.health = npc.maxHealth;
                    npc.defeated = false;
                    npc.talked = false;
                }
            }
        }
    }
    player->draw();

    // Логика боя
    updateBomzhFight();
    renderBattleUI();

    // Переходы между сценами
    handleSceneTransition();

    // Диалог (поверх всего)
    dialogue->draw();

    // Инвентарь
    inventory->draw();

    // Уведомление (сохранение / подбор)
    renderSaveNotification();

    // Подсказки управления (сцена 0)
    if (curr_scene == 0 && font) {
        std::string tips = "[A/D] движение  [E] взаимодействие  [I] инвентарь  [F5] сохранить";
        SDL_Color tc = { 180, 180, 180, 200 };
        SDL_Surface* s = TTF_RenderText_Blended(font, tips.c_str(), tips.size(), tc);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FRect r = { 500 - s->w / 2.0f, 660, (float)s->w, (float)s->h };
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderTexture(renderer, t, NULL, &r);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_DestroyTexture(t);
            SDL_DestroySurface(s);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    return SDL_APP_CONTINUE;
}

void Game::SDL_AppQuite(SDL_AppResult result) {
    autoSave();
}
