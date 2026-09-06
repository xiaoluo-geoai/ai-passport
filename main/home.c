// main/home.c —— 首页:并列大图标入口,上下选择,OK 由 main.c 统一跳转。
// 三个入口:身份卡(预设身份模板)/ Voice AI(语音助手) / Apps(系统应用)。
#include "home.h"
#include "ui_pixel.h"
#include "lvgl.h"
#include "esp_log.h"
#include "esp_system.h"

#define N        HOME_ENTRY_COUNT
#define ICON_X   20
#define ICON_W   200
#define ICON_H   56
#define ROW_H    72
#define ICON_Y0  46

typedef struct {
    const char *glyph;       // 图标大字
    const char *name;        // 入口名
    uint32_t    color;       // 图标底色
} entry_def_t;

static const entry_def_t ENTRIES[N] = {
    { "ID", "IDENTITY", 0x82BE2D },   // 身份卡
    { "V",  "VOICE AI", 0xFFB23E },   // 语音
    { "A",  "APPS",     0x1689E8 },   // 系统应用
};

static lv_obj_t *s_scr;
static lv_obj_t *s_icon[N];
static lv_obj_t *s_glyph[N];
static lv_obj_t *s_name[N];
static lv_obj_t *s_mascot;
static int s_sel;
static home_card_t s_identity = HOME_CARD_STUDENT;

static lv_obj_t *flat(lv_obj_t *parent, int x, int y, int w, int h, uint32_t color)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(color), 0);
    return obj;
}

static void refresh_all(void)
{
    for (int i = 0; i < N; i++) {
        bool sel = (i == s_sel);
        lv_obj_set_style_bg_color(s_icon[i], lv_color_hex(ENTRIES[i].color), 0);
        lv_obj_set_style_border_color(s_icon[i], lv_color_hex(sel ? 0xFFFFFF : UI_INK), 0);
        lv_obj_set_style_border_width(s_icon[i], sel ? 4 : 2, 0);
    }
}

void home_init(void)
{
    s_sel = 0;
    s_scr = ui_pixel_screen_create("AI PASSPORT");

    for (int i = 0; i < N; i++) {
        int y = ICON_Y0 + i * ROW_H;
        s_icon[i] = lv_obj_create(s_scr);
        lv_obj_remove_flag(s_icon[i], LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_pos(s_icon[i], ICON_X, y);
        lv_obj_set_size(s_icon[i], ICON_W, ICON_H);
        lv_obj_set_style_radius(s_icon[i], 10, 0);
        lv_obj_set_style_border_color(s_icon[i], lv_color_hex(UI_INK), 0);
        lv_obj_set_style_border_width(s_icon[i], 2, 0);
        lv_obj_set_style_pad_all(s_icon[i], 0, 0);

        s_glyph[i] = lv_label_create(s_icon[i]);
        lv_obj_set_style_text_font(s_glyph[i], &lv_font_montserrat_20, 0);
        lv_label_set_text(s_glyph[i], ENTRIES[i].glyph);
        lv_obj_align(s_glyph[i], LV_ALIGN_LEFT_MID, 16, 0);

        lv_obj_t *tag = lv_label_create(s_icon[i]);
        lv_obj_set_style_text_font(tag, &lv_font_montserrat_14, 0);
        lv_label_set_text(tag, "ENTER");
        lv_obj_align(tag, LV_ALIGN_RIGHT_MID, -16, 0);

        s_name[i] = lv_label_create(s_scr);
        lv_obj_set_style_text_font(s_name[i], &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(s_name[i], lv_color_hex(UI_INK), 0);
        lv_label_set_text(s_name[i], ENTRIES[i].name);
        lv_obj_align(s_name[i], LV_ALIGN_TOP_LEFT, ICON_X + 2, ICON_Y0 + i * ROW_H + 58);
    }

    s_mascot = ui_pixel_mascot_create(s_scr, 4, 234);
    refresh_all();
    home_mem_report();
}

void home_set_identity(home_card_t id)
{
    if (id >= HOME_CARD_STUDENT && id <= HOME_CARD_EVENT) s_identity = id;
}

void home_show(void)
{
    refresh_all();
    lv_screen_load(s_scr);
}

bool home_key(bsp_btn_t btn, bsp_btn_ev_t ev)
{
    if (ev != BSP_BTN_CLICK) return false;
    if (btn == BSP_BTN_UP) {
        s_sel = (s_sel + N - 1) % N;
        refresh_all();
        return true;
    }
    if (btn == BSP_BTN_DOWN) {
        s_sel = (s_sel + 1) % N;
        refresh_all();
        return true;
    }
    return false;   // OK 一律交 main 跳转(身份卡/Voice/Apps)
}

home_entry_t home_get_selected(void)
{
    return (home_entry_t)s_sel;
}

home_card_t home_get_identity(void)
{
    return s_identity;
}

// 内存自测:打印 LVGL 池与系统堆剩余,用于排查首页内存余量。
void home_mem_report(void)
{
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    ESP_LOGI("[home]",
             "LVGL池: used=%u free=%u frag=%u%%, 系统堆剩余=%uB",
             (unsigned)mon.total_size - (unsigned)mon.free_size,
             (unsigned)mon.free_size, (unsigned)mon.frag_pct,
             (unsigned)esp_get_free_heap_size());
}