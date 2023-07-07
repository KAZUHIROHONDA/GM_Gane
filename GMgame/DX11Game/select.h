//=============================================================================
//
// �|�[�Y���� [select.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// �|�[�Y���j���[
typedef enum {
	SELECT_MENU_CONTINUE = 0,	// �R���e�B�j���[
	SELECT_MENU_RETRY,			// ���g���C
	SELECT_MENU_QUIT,			// �N�C�b�g
	SELECT_MENU_MAX
} SELECT_MENU;

// �v���g�^�C�v�錾
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

void SetSelectMenu( void );
SELECT_MENU GetSelectMenu( void );
void ResetSelectMenu( void );
void ResetSelectMenu1(void);
void ResetSelectMenu2(void);