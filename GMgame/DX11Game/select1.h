//=============================================================================
//
// �|�[�Y���� [select1.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// �|�[�Y���j���[
typedef enum {
	SELECT1_MENU_CONTINUE = 0,	// �R���e�B�j���[
	SELECT1_MENU_RETRY,			// ���g���C
	SELECT1_MENU_QUIT,			// �N�C�b�g
	SELECT1_MENU_MAX
} SELECT1_MENU;

// �v���g�^�C�v�錾
HRESULT InitSelect1(void);
void UninitSelect1(void);
void UpdateSelect1(void);
void DrawSelect1(void);
void DrawSelect11(void);

void SetSelect1Menu( void );
SELECT1_MENU GetSelect1Menu( void );
void ResetSelect1Menu( void );
