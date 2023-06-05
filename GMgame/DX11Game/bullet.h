//=============================================================================
//
// 弾処理 [bullet.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "Camera.h"

// マクロ定義
#define BULLET_MAX (3)

//*****************************************************************************
// プロトタイプ宣言
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
