//=============================================================================
//
// �v���C���[���� [pchimera.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//�}�N����`
#define PCHIMERA_MAX	(1)

enum
{
	CAMERA_VIEW_FIXED,		//�Œ�
	CAMERA_VIEW_BIRD,			//�o�[�h�r���[
	CAMERA_VIEW_BEHIND,		//�r�n�C���h�r���[
	CAMERA_VIEW_FPS,			//FPS�r���[
	CAMERA_VIEW_CAMERA_DEBUG,	//CAMERA_DEBUG�p
	CAMERA_VIEW_DIAGONAL,
	CAMERA_VIEW_MAX,		//
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPChimera(void);
void UninitPChimera(void);
void UpdatePChimera(void);
void DrawPChimera(void);

void Sflag();