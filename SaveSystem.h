#pragma once
#include <string>

struct GameSaveData {
    int curr_scene;
    float playerX;
    float playerY;
    int playerHealth;
    int playerMoney;
    std::string inventoryData;
    bool talkedBaron_kitchen;
    bool talkedBaron_bedroom;
    bool talkedBaron_end;
    bool pickedMoney;
    bool pickedMayonnaise;
    bool foughtBomzhi;
};

class SaveSystem {
public:
    static bool save(const GameSaveData& data, const char* path = "save.dat");
    static bool load(GameSaveData& data, const char* path = "save.dat");
    static bool hasSave(const char* path = "save.dat");
};
