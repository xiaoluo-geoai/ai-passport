// main/identity.h —— 身份卡应用:预设身份模板选择页。
#pragma once

#include "bsp_button.h"
#include <stdbool.h>

// 进入身份卡首页(创建并加载其屏幕)
void identity_enter(void);

// 退出身份卡(由返回首页时调用,释放屏幕)
void identity_exit(void);

// 按键处理(UP/DOWN 移动,OK 选中身份并立即生效;长按返回由 main 处理)
bool identity_key(bsp_btn_t btn, bsp_btn_ev_t ev);