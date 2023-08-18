//=============================================================================
//
// プレイヤー処理 [Bpartsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// ポーズメニュー
typedef enum {
	BPARTS_MENU_KABU = 0,	// コンティニュー
	BPARTS_MENU_WANI,			// リトライ
	BPARTS_MENU_PANDA,
	BPARTS_MENU_TORI,
	BPARTS_MENU_MAX
} BPARTS_MENU;

// プロトタイプ宣言
HRESULT InitBParts(void);
void UninitBParts(void);
void UpdateBParts(void);
void DrawBParts(void);

void SetBPartsMenu(void);
BPARTS_MENU GetBPartsMenu(void);
void ResetBPartsMenu(void);
void ResetBPartsMenu1(void);
void ResetBPartsMenu2(void);
void ResetBPartsMenu3(void);

Player* BPartsGet();

