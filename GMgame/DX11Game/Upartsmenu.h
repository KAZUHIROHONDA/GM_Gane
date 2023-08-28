//=============================================================================
//
// �v���C���[���� [Upartsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// �|�[�Y���j���[
typedef enum {
	UPARTS_MENU_KABU = 0,	// �R���e�B�j���[
	UPARTS_MENU_WANI,			// ���g���C
	UPARTS_MENU_PANDA,
	UPARTS_MENU_TORI,
	UPARTS_MENU_BUTA,
	UPARTS_MENU_USI,
	UPARTS_MENU_MAX
} UPARTS_MENU;

// �v���g�^�C�v�錾
HRESULT InitUParts(void);
void UninitUParts(void);
void UpdateUParts(void);
void DrawUParts(void);

void SetUPartsMenu(void);
UPARTS_MENU GetUPartsMenu(void);
void ResetUPartsMenu(void);
void ResetUPartsMenu1(void);

Player* UPartsGet();