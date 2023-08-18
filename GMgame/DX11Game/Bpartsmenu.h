//=============================================================================
//
// �v���C���[���� [Bpartsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// �|�[�Y���j���[
typedef enum {
	BPARTS_MENU_KABU = 0,	// �R���e�B�j���[
	BPARTS_MENU_WANI,			// ���g���C
	BPARTS_MENU_PANDA,
	BPARTS_MENU_TORI,
	BPARTS_MENU_MAX
} BPARTS_MENU;

// �v���g�^�C�v�錾
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

