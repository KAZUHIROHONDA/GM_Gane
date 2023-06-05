//=============================================================================
//
// ポーズ処理 [select1.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// ポーズメニュー
typedef enum {
	SELECT1_MENU_CONTINUE = 0,	// コンティニュー
	SELECT1_MENU_RETRY,			// リトライ
	SELECT1_MENU_QUIT,			// クイット
	SELECT1_MENU_MAX
} SELECT1_MENU;

// プロトタイプ宣言
HRESULT InitSelect1(void);
void UninitSelect1(void);
void UpdateSelect1(void);
void DrawSelect1(void);
void DrawSelect11(void);

void SetSelect1Menu( void );
SELECT1_MENU GetSelect1Menu( void );
void ResetSelect1Menu( void );
