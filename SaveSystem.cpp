#define _CRT_SECURE_NO_WARNINGS
#include "SaveSystem.h"
#include <cstdio>
#include <cstring>

// Простой бинарный формат сохранения
// Структура: [int scene][float x][float y][int health][int money][int invLen][char invData...][bool*7]

bool SaveSystem::save(const GameSaveData& data, const char* path) {
    FILE* f = fopen(path, "wb");
    if (!f) return false;

    // Магическое число для проверки файла
    int magic = 0xDEADBEEF;
    fwrite(&magic, sizeof(int), 1, f);

    fwrite(&data.curr_scene, sizeof(int), 1, f);
    fwrite(&data.playerX, sizeof(float), 1, f);
    fwrite(&data.playerY, sizeof(float), 1, f);
    fwrite(&data.playerHealth, sizeof(int), 1, f);
    fwrite(&data.playerMoney, sizeof(int), 1, f);

    // Инвентарь как строка
    int invLen = (int)data.inventoryData.size();
    fwrite(&invLen, sizeof(int), 1, f);
    if (invLen > 0) fwrite(data.inventoryData.c_str(), 1, invLen, f);

    // Флаги мира
    fwrite(&data.talkedBaron_kitchen, sizeof(bool), 1, f);
    fwrite(&data.talkedBaron_bedroom, sizeof(bool), 1, f);
    fwrite(&data.talkedBaron_end, sizeof(bool), 1, f);
    fwrite(&data.pickedMoney, sizeof(bool), 1, f);
    fwrite(&data.pickedMayonnaise, sizeof(bool), 1, f);
    fwrite(&data.foughtBomzhi, sizeof(bool), 1, f);

    fclose(f);
    return true;
}

bool SaveSystem::load(GameSaveData& data, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;

    int magic = 0;
    fread(&magic, sizeof(int), 1, f);
    if (magic != (int)0xDEADBEEF) { fclose(f); return false; }

    fread(&data.curr_scene, sizeof(int), 1, f);
    fread(&data.playerX, sizeof(float), 1, f);
    fread(&data.playerY, sizeof(float), 1, f);
    fread(&data.playerHealth, sizeof(int), 1, f);
    fread(&data.playerMoney, sizeof(int), 1, f);

    int invLen = 0;
    fread(&invLen, sizeof(int), 1, f);
    if (invLen > 0) {
        char* buf = new char[invLen + 1];
        fread(buf, 1, invLen, f);
        buf[invLen] = '\0';
        data.inventoryData = buf;
        delete[] buf;
    }
    else {
        data.inventoryData = "";
    }

    fread(&data.talkedBaron_kitchen, sizeof(bool), 1, f);
    fread(&data.talkedBaron_bedroom, sizeof(bool), 1, f);
    fread(&data.talkedBaron_end, sizeof(bool), 1, f);
    fread(&data.pickedMoney, sizeof(bool), 1, f);
    fread(&data.pickedMayonnaise, sizeof(bool), 1, f);
    fread(&data.foughtBomzhi, sizeof(bool), 1, f);

    fclose(f);
    return true;
}

bool SaveSystem::hasSave(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    fclose(f);
    return true;
}
