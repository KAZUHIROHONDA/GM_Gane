//=============================================================================
//
// ポーズ処理 [select.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// ポーズメニュー
typedef enum {
	SELECT_MENU_CONTINUE = 0,	// コンティニュー
	SELECT_MENU_RETRY,			// リトライ
	SELECT_MENU_QUIT,			// クイット
	SELECT_MENU_MAX
} SELECT_MENU;

// プロトタイプ宣言
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

void SetSelectMenu( void );
SELECT_MENU GetSelectMenu( void );
void ResetSelectMenu( void );
void ResetSelectMenu1(void);
void ResetSelectMenu2(void);