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
	PARTS_MENU_WANI,			// ���g���C
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
void ResetPartsMenu1(void);

Player* PartsGet();