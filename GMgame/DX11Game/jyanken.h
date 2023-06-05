#pragma once
#include "main.h"

// ポーズメニュー
typedef enum {
	JYANKEN_MENU_GU = 0,	
	JYANKEN_MENU_TYOKI,		
	JYANKEN_MENU_PA,		
	JYANKEN_MENU_MAX
} JYANKEN_MENU;


HRESULT InitJyanken();
void UninitJyanken();
void UpdateJyanken();
void DrawJyanken();

void Jyanken(int no);

void SetJyankenMenu(void);
JYANKEN_MENU GetJyankenMenu(void);
void ResetJyankenMenu(void);