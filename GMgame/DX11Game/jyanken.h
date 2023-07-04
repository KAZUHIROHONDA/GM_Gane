#pragma once
#include "main.h"

// ポーズメニュー
typedef enum {
	JYANKEN_MENU_1 = 0,	
	JYANKEN_MENU_2,		
	JYANKEN_MENU_3,
	JYANKEN_MENU_4,
	JYANKEN_MENU_5,
	JYANKEN_MENU_OK,
	JYANKEN_MENU_MAX
} JYANKEN_MENU;


HRESULT InitJyanken();
void UninitJyanken();
void UpdateJyankenStart();
void UpdateJyankenSet();
void UpdateJyankenJadge();
void UpdateJyankenBattle();
void DrawJyankenStart();
void DrawJyankenSet();
void DrawJyankenJadge();


void Jyanken(int no, int *cnt);

void SetJyankenMenu(void);
JYANKEN_MENU GetJyankenMenu(void);
void ResetJyankenMenu(void);
void SetJyankenMenu1(void);
void SetJyankenMenu2(void);
void SetJyankenMenu3(void);
void SetJyankenMenu4(void);
void SetJyankenMenu5(void);

void Cntadd();