//=============================================================================
//
// ポーズ処理 [tab.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// ポーズメニュー
typedef enum {
	TAB_MENU_ATAMA = 0,	
	TAB_MENU_KARADA,			
	TAB_MENU_MAE,	
	TAB_MENU_USIRO,
	TAB_MENU_MAX
} TAB_MENU;

// プロトタイプ宣言
HRESULT InitTab(void);
void UninitTab(void);
void UpdateTab(void);
void DrawTab(void);

void SetTabMenu( void );
TAB_MENU GetTabMenu( void );
void ResetTabMenu( void );
void ResetTabMenu1(void);
void ResetTabMenu2(void);
void ResetTabMenu3(void);