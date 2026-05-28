#include "unity.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Логика здоровья                                                     */
/* ------------------------------------------------------------------ */

static int clamp_health(int hp, int total) {
    if (hp < 0) return 0;
    if (hp > total) return total;
    return hp;
}

static int take_damage(int curr, int dmg, int total) {
    return clamp_health(curr - dmg, total);
}

void test_health_clamp_normal(void) { TEST_ASSERT_EQUAL(100, clamp_health(100, 100)); }
void test_health_clamp_zero(void) { TEST_ASSERT_EQUAL(0, clamp_health(0, 100)); }
void test_health_clamp_negative(void) { TEST_ASSERT_EQUAL(0, clamp_health(-10, 100)); }
void test_health_clamp_overflow(void) { TEST_ASSERT_EQUAL(100, clamp_health(150, 100)); }

void test_damage_normal(void) { TEST_ASSERT_EQUAL(70, take_damage(100, 30, 100)); }
void test_damage_exact(void) { TEST_ASSERT_EQUAL(0, take_damage(20, 20, 100)); }
void test_damage_overkill(void) { TEST_ASSERT_EQUAL(0, take_damage(10, 50, 100)); }
void test_damage_zero(void) { TEST_ASSERT_EQUAL(100, take_damage(100, 0, 100)); }

/* ------------------------------------------------------------------ */
/* Логика денег                                                        */
/* ------------------------------------------------------------------ */

void test_money_add(void) { int c = 0; c += 50; TEST_ASSERT_EQUAL(50, c); }
void test_money_accumulate(void) { int c = 50; c += 25; TEST_ASSERT_EQUAL(75, c); }
void test_money_add_zero(void) { int c = 75; c += 0;  TEST_ASSERT_EQUAL(75, c); }
void test_money_subtract(void) { int c = 75; c += -10; TEST_ASSERT_EQUAL(65, c); }

/* ------------------------------------------------------------------ */
/* Инвентарь                                                          */
/* ------------------------------------------------------------------ */

#define MAX_ITEMS 16
#define MAX_STR   256

typedef struct { char name[MAX_STR]; char path[MAX_STR]; int count; } Item;
typedef struct { Item items[MAX_ITEMS]; int size; } Inv;

static void inv_add(Inv* inv, const char* name, const char* path, int count) {
    for (int i = 0; i < inv->size; i++) {
        if (strcmp(inv->items[i].name, name) == 0) { inv->items[i].count += count; return; }
    }
    if (inv->size >= MAX_ITEMS) return;
    strncpy(inv->items[inv->size].name, name, MAX_STR - 1);
    strncpy(inv->items[inv->size].path, path, MAX_STR - 1);
    inv->items[inv->size].count = count;
    inv->size++;
}

static bool inv_has(const Inv* inv, const char* name) {
    for (int i = 0; i < inv->size; i++)
        if (strcmp(inv->items[i].name, name) == 0) return true;
    return false;
}

static void inv_serialize(const Inv* inv, char* out, int outsize) {
    out[0] = '\0';
    for (int i = 0; i < inv->size; i++) {
        char line[MAX_STR * 2 + 16];
        snprintf(line, sizeof(line), "%s|%s|%d\n",
            inv->items[i].name, inv->items[i].path, inv->items[i].count);
        strncat(out, line, outsize - strlen(out) - 1);
    }
}

static void inv_deserialize(Inv* inv, const char* data) {
    inv->size = 0;
    char buf[4096];
    strncpy(buf, data, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    char* line = strtok(buf, "\n");
    while (line) {
        char* p1 = strchr(line, '|');
        char* p2 = strrchr(line, '|');
        if (!p1 || p1 == p2) { line = strtok(NULL, "\n"); continue; }
        Item it;
        int nlen = (int)(p1 - line), plen = (int)(p2 - p1 - 1);
        strncpy(it.name, line, nlen); it.name[nlen] = '\0';
        strncpy(it.path, p1 + 1, plen); it.path[plen] = '\0';
        it.count = atoi(p2 + 1);
        if (inv->size < MAX_ITEMS) inv->items[inv->size++] = it;
        line = strtok(NULL, "\n");
    }
}

void test_inv_add_new(void) {
    Inv inv = { 0 };
    inv_add(&inv, "Mayonnaise", "assets/mayo.png", 1);
    TEST_ASSERT_EQUAL(1, inv.size);
    TEST_ASSERT_TRUE(inv_has(&inv, "Mayonnaise"));
}

void test_inv_has_missing(void) {
    Inv inv = { 0 };
    TEST_ASSERT_FALSE(inv_has(&inv, "Knife"));
}

void test_inv_stack(void) {
    Inv inv = { 0 };
    inv_add(&inv, "Mayonnaise", "assets/mayo.png", 1);
    inv_add(&inv, "Mayonnaise", "assets/mayo.png", 2);
    TEST_ASSERT_EQUAL(1, inv.size);
    TEST_ASSERT_EQUAL(3, inv.items[0].count);
}

void test_inv_multiple(void) {
    Inv inv = { 0 };
    inv_add(&inv, "Mayonnaise", "assets/mayo.png", 1);
    inv_add(&inv, "Key", "assets/key.png", 1);
    TEST_ASSERT_EQUAL(2, inv.size);
    TEST_ASSERT_TRUE(inv_has(&inv, "Key"));
}

void test_inv_serialize_roundtrip(void) {
    Inv inv = { 0 };
    inv_add(&inv, "Mayonnaise", "assets/mayo.png", 3);
    inv_add(&inv, "Key", "assets/key.png", 1);
    char serial[4096];
    inv_serialize(&inv, serial, sizeof(serial));
    Inv inv2 = { 0 };
    inv_deserialize(&inv2, serial);
    TEST_ASSERT_EQUAL(inv.size, inv2.size);
    TEST_ASSERT_TRUE(inv_has(&inv2, "Mayonnaise"));
    TEST_ASSERT_TRUE(inv_has(&inv2, "Key"));
    TEST_ASSERT_EQUAL(inv.items[0].count, inv2.items[0].count);
}

void test_inv_serialize_empty(void) {
    Inv inv = { 0 };
    char s[256];
    inv_serialize(&inv, s, sizeof(s));
    TEST_ASSERT_EQUAL(0, strlen(s));
}

void test_inv_deserialize_empty(void) {
    Inv inv = { 0 };
    inv_deserialize(&inv, "");
    TEST_ASSERT_EQUAL(0, inv.size);
}

/* ------------------------------------------------------------------ */
/* SaveSystem                                                          */
/* ------------------------------------------------------------------ */

#define MAGIC 0xDEADBEEF

typedef struct {
    int   curr_scene;
    float playerX, playerY;
    int   playerHealth, playerMoney;
    char  inventoryData[2048];
    bool  talkedBaron_kitchen, talkedBaron_bedroom, talkedBaron_end;
    bool  pickedMoney, pickedMayonnaise, foughtBomzhi;
} SaveData;

static bool save_to_file(const SaveData* d, const char* path) {
    FILE* f = fopen(path, "wb"); if (!f) return false;
    int magic = MAGIC;
    fwrite(&magic, sizeof(int), 1, f);
    fwrite(&d->curr_scene, sizeof(int), 1, f);
    fwrite(&d->playerX, sizeof(float), 1, f);
    fwrite(&d->playerY, sizeof(float), 1, f);
    fwrite(&d->playerHealth, sizeof(int), 1, f);
    fwrite(&d->playerMoney, sizeof(int), 1, f);
    int invLen = (int)strlen(d->inventoryData);
    fwrite(&invLen, sizeof(int), 1, f);
    if (invLen > 0) fwrite(d->inventoryData, 1, invLen, f);
    fwrite(&d->talkedBaron_kitchen, sizeof(bool), 1, f);
    fwrite(&d->talkedBaron_bedroom, sizeof(bool), 1, f);
    fwrite(&d->talkedBaron_end, sizeof(bool), 1, f);
    fwrite(&d->pickedMoney, sizeof(bool), 1, f);
    fwrite(&d->pickedMayonnaise, sizeof(bool), 1, f);
    fwrite(&d->foughtBomzhi, sizeof(bool), 1, f);
    fclose(f); return true;
}

static bool load_from_file(SaveData* d, const char* path) {
    FILE* f = fopen(path, "rb"); if (!f) return false;
    int magic = 0; fread(&magic, sizeof(int), 1, f);
    if (magic != (int)MAGIC) { fclose(f); return false; }
    fread(&d->curr_scene, sizeof(int), 1, f);
    fread(&d->playerX, sizeof(float), 1, f);
    fread(&d->playerY, sizeof(float), 1, f);
    fread(&d->playerHealth, sizeof(int), 1, f);
    fread(&d->playerMoney, sizeof(int), 1, f);
    int invLen = 0; fread(&invLen, sizeof(int), 1, f);
    if (invLen > 0 && invLen < (int)sizeof(d->inventoryData)) {
        fread(d->inventoryData, 1, invLen, f);
        d->inventoryData[invLen] = '\0';
    }
    else { d->inventoryData[0] = '\0'; }
    fread(&d->talkedBaron_kitchen, sizeof(bool), 1, f);
    fread(&d->talkedBaron_bedroom, sizeof(bool), 1, f);
    fread(&d->talkedBaron_end, sizeof(bool), 1, f);
    fread(&d->pickedMoney, sizeof(bool), 1, f);
    fread(&d->pickedMayonnaise, sizeof(bool), 1, f);
    fread(&d->foughtBomzhi, sizeof(bool), 1, f);
    fclose(f); return true;
}

static bool has_save(const char* path) {
    FILE* f = fopen(path, "rb"); if (!f) return false; fclose(f); return true;
}

#define SAVE_PATH "unity_test_save.dat"

void setUp(void) { remove(SAVE_PATH); }
void tearDown(void) { remove(SAVE_PATH); }

void test_save_no_file(void) {
    TEST_ASSERT_FALSE(has_save(SAVE_PATH));
}

void test_save_roundtrip(void) {
    SaveData orig = { 0 };
    orig.curr_scene = 2; orig.playerX = 123.5f; orig.playerY = 250.0f;
    orig.playerHealth = 80; orig.playerMoney = 150;
    orig.talkedBaron_kitchen = true; orig.foughtBomzhi = true;
    strncpy(orig.inventoryData, "Mayonnaise|assets/mayo.png|1\n",
        sizeof(orig.inventoryData) - 1);

    TEST_ASSERT_TRUE(save_to_file(&orig, SAVE_PATH));
    TEST_ASSERT_TRUE(has_save(SAVE_PATH));

    SaveData loaded = { 0 };
    TEST_ASSERT_TRUE(load_from_file(&loaded, SAVE_PATH));
    TEST_ASSERT_EQUAL(orig.curr_scene, loaded.curr_scene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, orig.playerX, loaded.playerX);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, orig.playerY, loaded.playerY);
    TEST_ASSERT_EQUAL(orig.playerHealth, loaded.playerHealth);
    TEST_ASSERT_EQUAL(orig.playerMoney, loaded.playerMoney);
    TEST_ASSERT_TRUE(loaded.talkedBaron_kitchen);
    TEST_ASSERT_TRUE(loaded.foughtBomzhi);
    TEST_ASSERT_EQUAL_STRING(orig.inventoryData, loaded.inventoryData);
}

void test_save_bad_magic(void) {
    FILE* f = fopen(SAVE_PATH, "wb");
    int bad = 0x12345678;
    fwrite(&bad, sizeof(int), 1, f);
    fclose(f);
    SaveData d = { 0 };
    TEST_ASSERT_FALSE(load_from_file(&d, SAVE_PATH));
}

/* ------------------------------------------------------------------ */
/* DialogueSystem                                                      */
/* ------------------------------------------------------------------ */

typedef struct { char speaker[64]; char text[256]; } DLine;
typedef struct { DLine lines[32]; int count, current; bool active; } Dialogue;

static void dlg_start(Dialogue* d, const DLine* lines, int n) {
    d->count = n < 32 ? n : 32;
    d->current = 0; d->active = (n > 0);
    for (int i = 0; i < d->count; i++) d->lines[i] = lines[i];
}
static void dlg_advance(Dialogue* d) {
    if (!d->active) return;
    if (++d->current >= d->count) { d->active = false; d->current = 0; }
}

void test_dlg_start_active(void) {
    DLine lines[] = { {"Джек","Привет!"},{"Барон","Здарова."} };
    Dialogue d = { 0 }; dlg_start(&d, lines, 2);
    TEST_ASSERT_TRUE(d.active);
    TEST_ASSERT_EQUAL(0, d.current);
}

void test_dlg_advance(void) {
    DLine lines[] = { {"Джек","Привет!"},{"Барон","Здарова."} };
    Dialogue d = { 0 }; dlg_start(&d, lines, 2);
    dlg_advance(&d);
    TEST_ASSERT_EQUAL(1, d.current);
    TEST_ASSERT_EQUAL_STRING("Барон", d.lines[d.current].speaker);
}

void test_dlg_end(void) {
    DLine lines[] = { {"Джек","Пока."} };
    Dialogue d = { 0 }; dlg_start(&d, lines, 1);
    dlg_advance(&d);
    TEST_ASSERT_FALSE(d.active);
    TEST_ASSERT_EQUAL(0, d.current);
}

void test_dlg_advance_inactive(void) {
    Dialogue d = { 0 };
    dlg_advance(&d); /* не должен упасть */
    TEST_ASSERT_FALSE(d.active);
}

void test_dlg_empty(void) {
    Dialogue d = { 0 }; dlg_start(&d, NULL, 0);
    TEST_ASSERT_FALSE(d.active);
}

/* ------------------------------------------------------------------ */
/* Runner                                                              */
/* ------------------------------------------------------------------ */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_health_clamp_normal);
    RUN_TEST(test_health_clamp_zero);
    RUN_TEST(test_health_clamp_negative);
    RUN_TEST(test_health_clamp_overflow);
    RUN_TEST(test_damage_normal);
    RUN_TEST(test_damage_exact);
    RUN_TEST(test_damage_overkill);
    RUN_TEST(test_damage_zero);

    RUN_TEST(test_money_add);
    RUN_TEST(test_money_accumulate);
    RUN_TEST(test_money_add_zero);
    RUN_TEST(test_money_subtract);

    RUN_TEST(test_inv_add_new);
    RUN_TEST(test_inv_has_missing);
    RUN_TEST(test_inv_stack);
    RUN_TEST(test_inv_multiple);
    RUN_TEST(test_inv_serialize_roundtrip);
    RUN_TEST(test_inv_serialize_empty);
    RUN_TEST(test_inv_deserialize_empty);

    RUN_TEST(test_save_no_file);
    RUN_TEST(test_save_roundtrip);
    RUN_TEST(test_save_bad_magic);

    RUN_TEST(test_dlg_start_active);
    RUN_TEST(test_dlg_advance);
    RUN_TEST(test_dlg_end);
    RUN_TEST(test_dlg_advance_inactive);
    RUN_TEST(test_dlg_empty);

    return UNITY_END();
}
