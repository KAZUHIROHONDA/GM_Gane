//=============================================================================
//
// �v���C���[���� [Mpartsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// �|�[�Y���j���[
typedef enum {
	MPARTS_MENU_KABU = 0,	
	MPARTS_MENU_WANI,		
	MPARTS_MENU_PANDA,
	MPARTS_MENU_TORI,
	MPARTS_MENU_BUTA,
	MPARTS_MENU_USI,
	MPARTS_MENU_MAX
} MPARTS_MENU;

// �v���g�^�C�v�錾
HRESULT InitMParts(void);
void UninitMParts(void);
void UpdateMParts(void);
void DrawMParts(void);

void SetMPartsMenu(void);
MPARTS_MENU GetMPartsMenu(void);
void ResetMPartsMenu(void);
void ResetMPartsMenu1(void);

Player* MPartsGet();

int MCharaNo();