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
	SELECT2_MENU_CONTINUE = 0,	// コンティニュー
	SELECT2_MENU_RETRY,			// リトライ
	SELECT2_MENU_MAX
} SELECT2_MENU;

// プロトタイプ宣言
HRESULT InitSelect2(void);
void UninitSelect2(void);
void UpdateSelect2(void);
void DrawSelect2(void);

void SetSelect2Menu( void );
SELECT2_MENU GetSelect2Menu( void );
void ResetSelect2Menu( void );
void ResetSelect2Menu1(void);