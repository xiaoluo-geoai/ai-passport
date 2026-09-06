// main/home.h —— 首页:并列大图标入口,选择后进入身份卡/Voice/Apps。
#pragma once

#include "bsp_button.h"
#include "lvgl.h"
#include <stdbool.h>

// 身份(学生/开发者/活动)。保留前三个值做 voice 后端的身份映射(demo_voice.c),
// 旧功能卡 APPS/VOICE 已改为首页并列入口,不再作为身份卡。
typedef enum {
    HOME_CARD_STUDENT = 0,   // 身份:学生
    HOME_CARD_DEV,           // 身份:开发者
    HOME_CARD_EVENT,         // 身份:活动
    HOME_CARD_COUNT
} home_card_t;

// 首页并列入口
typedef enum {
    HOME_ENTRY_IDENTITY = 0,   // 身份卡(预设身份模板)
    HOME_ENTRY_VOICE,          // Voice AI(语音助手)
    HOME_ENTRY_APPS,           // Apps(系统应用集合)
    HOME_ENTRY_COUNT
} home_entry_t;

// 创建首页屏幕(在 app_main 里、外设初始化后调用一次)
void home_init(void);

// 显示首页
void home_show(void);

// 设置当前身份(学生/开发者/活动),由身份卡应用调用
void home_set_identity(home_card_t id);

// 首页按键处理(UP/DOWN 移动;OK 命中入口返回 false 交由 main 跳转,返回 true 表示已消费)
bool home_key(bsp_btn_t btn, bsp_btn_ev_t ev);

// 当前选中的入口
home_entry_t home_get_selected(void);

// 当前身份,语音助手按此应答
home_card_t home_get_identity(void);

// 内存自测:打印 LVGL 池与系统堆剩余(用于排查首页内存余量)
void home_mem_report(void);