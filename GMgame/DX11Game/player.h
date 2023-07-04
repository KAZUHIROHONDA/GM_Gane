//=============================================================================
//
// �v���C���[���� [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//�}�N����`
#define PLAYER_MAX	(1)

enum
{
	E_CAMERA_VIEW_FIXED,		//�Œ�
	E_CAMERA_VIEW_BIRD,			//�o�[�h�r���[
	E_CAMERA_VIEW_BEHIND,		//�r�n�C���h�r���[
	E_CAMERA_VIEW_FPS,			//FPS�r���[
	E_CAMERA_VIEW_CAMERA_DEBUG,	//CAMERA_DEBUG�p
	E_CAMERA_VIEW_DIAGONAL,
	E_CAMERA_VIEW_MAX,		//
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void UpdateStart(void);

bool IsPlayer(int no);
void DestroyPlayer(int no);
XMFLOAT3 GetPlayerPos(int no);
XMFLOAT3 GetPlayerSize(int no);

void DamagePlayer(int damage);
void DrawPlayerGauge();

int GetPlayerHp(int no);
void ResetPos(int no);

void Action(bool af);
void Action();
void Action2(bool af);
void Action2();
void Action3(bool af);

void Action4(bool af);

void CameraFIXED();
void CameraDEBUG();