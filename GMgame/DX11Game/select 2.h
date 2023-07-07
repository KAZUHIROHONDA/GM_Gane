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
	SELECT2_MENU_CONTINUE = 0,	// �R���e�B�j���[
	SELECT2_MENU_RETRY,			// ���g���C
	SELECT2_MENU_MAX
} SELECT2_MENU;

// �v���g�^�C�v�錾
HRESULT InitSelect2(void);
void UninitSelect2(void);
void UpdateSelect2(void);
void DrawSelect2(void);

void SetSelect2Menu( void );
SELECT2_MENU GetSelect2Menu( void );
void ResetSelect2Menu( void );
void ResetSelect2Menu1(void);