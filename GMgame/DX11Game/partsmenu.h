//=============================================================================
//
// �v���C���[���� [partsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// �|�[�Y���j���[
typedef enum {
	PARTS_MENU_KABU = 0,	// �R���e�B�j���[
	PARTS_MENU_WANI,	
	PARTS_MENU_PANDA,
	PARTS_MENU_TORI,
	PARTS_MENU_BUTA,
	PARTS_MENU_USI,// ���g���C
	PARTS_MENU_MAX
} PARTS_MENU;

// �v���g�^�C�v�錾
HRESULT InitParts(void);
void UninitParts(void);
void UpdateParts(void);
void DrawParts(void);

void SetPartsMenu(void);
PARTS_MENU GetPartsMenu(void);
void ResetPartsMenu(void);

Player* PartsGet();
int CharaNo();