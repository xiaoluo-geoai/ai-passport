// main/identity.c —— 身份卡应用:预设身份模板(学生/开发者/活动),只能选不能改。
// 选中后立即通过 home_set_identity 写入,语音助手按此身份应答。
#include "identity.h"
#include "home.h"
#include "ui_pixel.h"
#include "lvgl.h"
#include <string.h>

#define ID_COUNT 3   // == HOME_CARD_COUNT

typedef struct {
    const char *name;     // 身份名
    const char *line;     // 副说明
    uint32_t    color;    // 卡面色
} id_def_t;

static const id_def_t IDS[ID_COUNT] = {
    { "STUDENT",  "CHANG'AN  GEOLOGY", 0x82BE2D },
    { "DEVELOPER","TRAE x ESP32",      0x7557D9 },
    { "EVENT",    "AI PASSPORT",       0xE43B2F },
};

static lv_obj_t *s_scr;
static lv_obj_t *s_icon[ID_COUNT];
static lv_obj_t *s_mark[ID_COUNT];   // "ACTIVE" 标记
static int s_sel;

static lv_obj_t *flat(lv_obj_t *parent, int x, int y, int w, int h, uint32_t color);

static void refresh_all(void)
{
    home_card_t cur = home_get_identity();
    for (int i = 0; i < ID_COUNT; i++) {
        bool sel = (i == s_sel);
        lv_obj_set_style_border_color(s_icon[i], lv_color_hex(sel ? 0xFFFFFF : UI_INK), 0);
        lv_obj_set_style_border_width(s_icon[i], sel ? 4 : 2, 0);
        if (cur == i) {
            lv_obj_t *t = s_mark[i];
            lv_obj_set_style_bg_color(t, lv_color_hex(0xF44336), 0);
            lv_label_set_text(t, "ACTIVE");
        } else {
            lv_obj_set_style_bg_color(s_mark[i], lv_color_hex(UI_INK), 0);
            lv_label_set_text(s_mark[i], "");
        }
    }
}

static lv_obj_t *mk_icon(int i)
{
    lv_obj_t *card = lv_obj_create(s_scr);
    lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(card, 20, 50 + i * 62);
    lv_obj_set_size(card, 200, 48);
    lv_obj_set_style_radius(card, 10, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(UI_INK), 0);
    lv_obj_set_style_border_width(card, 2, 0);
    lv_obj_set_style_pad_all(card, 0, 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(IDS[i].color), 0);

    lv_obj_t *nm = lv_label_create(card);
    lv_obj_set_style_text_font(nm, &lv_font_montserrat_20, 0);
    lv_label_set_text(nm, IDS[i].name);
    lv_obj_align(nm, LV_ALIGN_LEFT_MID, 12, -6);

    lv_obj_t *ln = lv_label_create(card);
    lv_obj_set_style_text_font(ln, &lv_font_montserrat_14, 0);
    lv_label_set_text(ln, IDS[i].line);
    lv_obj_align(ln, LV_ALIGN_LEFT_MID, 12, 10);

    lv_obj_t *tag = lv_obj_create(card);
    lv_obj_remove_flag(tag, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(tag, 118, 8);
    lv_obj_set_size(tag, 74, 20);
    lv_obj_set_style_radius(tag, 6, 0);
    lv_obj_set_style_border_width(tag, 0, 0);
    lv_obj_set_style_pad_all(tag, 0, 0);
    lv_obj_t *tl = lv_label_create(tag);
    lv_obj_set_style_text_font(tl, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(tl, lv_color_white(), 0);
    lv_label_set_text(tl, "ACTIVE");
    lv_obj_center(tl);
    s_mark[i] = tag;

    return card;
}

void identity_enter(void)
{
    s_sel = (int)home_get_identity();
    s_scr = ui_pixel_screen_create("IDENTITY");
    for (int i = 0; i < ID_COUNT; i++) s_icon[i] = mk_icon(i);
    ui_pixel_mascot_create(s_scr, 4, 300);
    refresh_all();
    lv_screen_load(s_scr);
}

void identity_exit(void)
{
    if (s_scr) { lv_obj_delete(s_scr); s_scr = NULL; }
}

bool identity_key(bsp_btn_t btn, bsp_btn_ev_t ev)
{
    if (ev != BSP_BTN_CLICK) return false;
    if (btn == BSP_BTN_UP) {
        s_sel = (s_sel + ID_COUNT - 1) % ID_COUNT;
        refresh_all();
        return true;
    }
    if (btn == BSP_BTN_DOWN) {
        s_sel = (s_sel + 1) % ID_COUNT;
        refresh_all();
        return true;
    }
    if (btn == BSP_BTN_OK) {
        home_set_identity((home_card_t)s_sel);   // 立即生效
        refresh_all();
        return true;
    }
    return false;
}