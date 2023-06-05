//=============================================================================
//
// �e���� [bullet.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "Camera.h"

// �}�N����`
#define BULLET_MAX (3)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int FireBullet(XMFLOAT3 pos, XMFLOAT3 dir,int no);

bool IsBullet(int no);
void DestroyBullet(int no);
XMFLOAT3 GetBulletPos(int no);
XMFLOAT3 GetBulletSize(int no);
