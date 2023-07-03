//=============================================================================
//
// �|�[�Y���� [selectstage.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// �|�[�Y���j���[
typedef enum {
	SELECT_STAGE_1 = 0,	
	SELECT_STAGE_2,		

	SELECT_MAX
} SELECT_STAGE;

// �v���g�^�C�v�錾
HRESULT InitSelectStage(void);
void UninitSelectStage(void);
void UpdateSelectStage(void);
void DrawSelectStage(void);

void SetSelectStageMenu( void );
SELECT_STAGE GetSelectStageMenu( void );
void ResetSelectStageMenu( void );

void SetSelectStageMenu1(void);
void SetSelectStageMenu2(void);